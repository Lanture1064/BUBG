#include "Server.h"
#include "Iphlpapi.h"
#include <memory>

int Server::new_id = 0x0001;
Server * Server::getInstance()
{
	static Server instance;
	return &instance;
}

void Server::clear()
{
	this->endGame();
	this->endWait();
	new_id = 0x0001;
	CommandImformation command;
	command.command = END_GAME;
	this->sendCommand(command);
	
	
	for (auto i = players_data_.begin(); i != players_data_.end(); ++i)
	{
		if (i->sock != nullptr)
		{
			clear_sock_lock_.lock();
			i->sock->close();
			i->sock = nullptr;
			clear_sock_lock_.unlock();
		}
		if (i->message_sock != nullptr)
		{
			clear_message_lock_.lock();
			i->message_sock->close();
			i->sock = nullptr;
			clear_message_lock_.unlock();
		}
	}
	players_data_.clear();


	local_command_lock_.lock();
	local_command_buffer_.clear();
	local_command_lock_.unlock();
}

void Server::connect()
{
	aceptor_=std::make_shared<boost::asio::ip::tcp::acceptor>(service_, endpoint_);
	std::shared_ptr<Player::socket> sock;
	std::shared_ptr<Player::socket> message_sock;
	boost::asio::ip::tcp::acceptor message_aceptor(service_, message_endpoint_);
	Player new_player;
	while (is_wait_)
	{
		sock = std::make_shared<Player::socket>(service_);
		try
		{
			aceptor_->accept(*sock);
			auto buf = std::vector<CommandImformation>(1);
			sock->receive(boost::asio::buffer(buf));
			new_player.id = new_id;
			new_id++;
			new_player.sock = sock;
			if (is_wait_&&buf.begin()->command == NEW_PLAYER)
			{
				buf.clear();
				CommandImformation temp;
				temp.command = REPLY_NEW_PLAYER;
				temp.id = new_player.id;
				buf.push_back(temp);
				new_player.sock->send(boost::asio::buffer(buf));
				buf.clear();
				temp.command = NEW_PLAYER;

				//send the server player imformation;
				temp.id = 0x0000;
				buf.push_back(temp);
				new_player.sock->send(boost::asio::buffer(buf));
				buf.clear();

				for (auto player = players_data_.begin(); player != players_data_.end(); ++player)
				{
					//send the imformation of the player who was connected;
					temp.id = player->id;
					buf.push_back(temp);
					new_player.sock->send(boost::asio::buffer(buf));
					buf.clear();
					//send the new player imformation;
					temp.id = new_player.id;
					buf.push_back(temp);
					player->sock->send(boost::asio::buffer(buf));
					buf.clear();
				}
				message_sock = std::make_shared<Player::socket>(service_);
				message_aceptor.accept(*message_sock);
				new_player.message_sock = message_sock;
				players_data_.push_back(new_player);
			}

		} catch (std::exception &e)
		{
		}
	}
}



void Server::getCommand()
{
	while (is_in_game_)
	{
		clear_sock_lock_.lock();
		for (auto i = players_data_.begin(); i != players_data_.end(); ++i)
		{
			if (i->sock != nullptr)
			{
				if ((*i).sock->available())
				{
					std::vector<CommandImformation> buf((*i).sock->available() / sizeof(CommandImformation));
					(*i).sock->read_some(boost::asio::buffer(buf));
					for (auto j = buf.begin(); j != buf.end(); ++j)
					{
						if (!excuteCommand(*j))
						{
							break;
						}
					}
				}
			}
		}
		clear_sock_lock_.unlock();
	}
}


std::vector<std::string> Server::getMessage()
{
	auto temp = std::vector<std::string>();
	clear_message_lock_.lock();
	for (auto player = players_data_.begin(); player != players_data_.end(); ++player)
	{
		if (player->message_sock != nullptr)
		{
			if (player->message_sock->available())
			{
				auto text = std::string();
				auto buf = std::string(1, '\0');
				for (;;)
				{
					player->message_sock->receive(boost::asio::buffer(buf));
					if (*buf.begin() == '\n')
					{
						break;
					}
					text += buf;
				}
				temp.push_back(text);
				text += "\n";
				for (auto i = players_data_.begin(); i != players_data_.end(); ++i)
				{
					if (i->id != player->id)
					{
						i->message_sock->send(boost::asio::buffer(text));
					}
				}
			}
		}
	}
	clear_message_lock_.unlock();
	return temp;
}

void Server::sendMessage(std::string text)
{
	text += "\n";
	for (auto i = players_data_.begin(); i != players_data_.end(); ++i)
	{
		close_message_lock_.lock();
		if (i->message_sock != nullptr)
		{
			i->message_sock->send(boost::asio::buffer(text));
		}
		close_message_lock_.unlock();
	}
}

void Server::beginWait()
{
	is_wait_ = true;
}
void Server::endWait()
{
	is_wait_ = false;
	if (aceptor_ != nullptr)
	{
		aceptor_->close();
		aceptor_ = nullptr;
	}
}

void Server::startGame()
{
	is_in_game_ = true;
}

void Server::endGame()
{
	is_in_game_ = false;
}



bool Server::excuteCommand(CommandImformation command)
{
	if (command.command == -1)
	{
		return false;
	}
	if (command.command != EXIT_GAME)
	{
		local_command_lock_.lock();
		local_command_buffer_.push_back(command);
		local_command_lock_.unlock();
	}
	else
	{
		for (auto j = players_data_.begin(); j != players_data_.end(); ++j)
		{
			if (j->id == command.id)
			{
				command.command = REPLY_EXIT;
				std::vector<CommandImformation> buf;
				buf.push_back(command);
				j->sock->send(boost::asio::buffer(buf));
				close_sock_lock_.lock();
				j->sock->close();
				j->sock = nullptr;
				close_sock_lock_.unlock();
				close_message_lock_.lock();
				j->message_sock->close();
				j->message_sock = nullptr;
				close_message_lock_.unlock();

				
			}
		}
	}
	return true;
}

std::vector<CommandImformation> Server::getLocalCommand()
{
	if (local_command_buffer_.size())
	{
		local_command_lock_.lock();
		auto temp = local_command_buffer_;
		local_command_buffer_.clear();
		local_command_lock_.unlock();
		return temp;
	}
	else
	{
		return std::vector<CommandImformation>();
	}
}


void Server::sendCommand(CommandImformation command)
{
	std::vector<CommandImformation> buf;
	switch (command.command)
	{
	case DIRECTION: case DIRECTION_BY_KEY:
		for (auto j = players_data_.begin(); j != players_data_.end(); ++j)
		{
			buf.clear();
			close_sock_lock_.lock();
			if (j->sock != nullptr)
			{
				if (j->id != command.id)
				{
					
					buf.push_back(command);
					j->sock->send(boost::asio::buffer(buf));
				}
			}
			close_sock_lock_.unlock();
		}
		break;
	case NEW_FOOD: case NEW_MANAGER: case INIT_END: case NEW_VIRUS: case END_GAME: default:
		for (auto j = players_data_.begin(); j != players_data_.end(); ++j)
		{
			buf.clear();
			buf.push_back(command);
			close_sock_lock_.lock();
			if (j->sock != nullptr)
			{				
				j->sock->send(boost::asio::buffer(buf));
			}
			close_sock_lock_.unlock();
		}
		break;
	
	}
}


const std::vector<Player>& Server::getPlayer() const
{
	return players_data_;
}

std::string Server::getIp() const
{
	return endpoint_.address().to_string();
}

Server::Server():is_wait_(false),is_in_game_(false),players_data_(), local_command_buffer_(),
				 local_command_lock_(), service_(),endpoint_(boost::asio::ip::tcp::v4(),PORT),
				 message_endpoint_(boost::asio::ip::tcp::v4(),MESSAGE_PORT),log_()
{
	aceptor_ = nullptr;
}

Server::~Server()
{
	this->clear();
}
