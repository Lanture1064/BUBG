#include "Server.h"
#include <memory>

int Server::new_id = 0x0000;
Server * Server::getInstance()
{
	static Server instance;
	return &instance;
}

void Server::claer()
{
	is_wait_ = false;
	player_data_lock_.lock();
	players_data_.clear();
	player_data_lock_.unlock();
	net_command_lock_.lock();
	net_command_buffer_.clear();
	local_command_lock_.lock();
	local_command_buffer_.clear();
	local_command_lock_.unlock();
}

void Server::connect()
{
	boost::asio::ip::tcp::acceptor aceptor(service_, endpoint_);
	std::shared_ptr<Player::socket>  sock;
	Player new_player;
	for (;;)
	{
		sock = std::make_shared<Player::socket>(service_);
		aceptor.accept(*sock);
		new_player.id = new_id;
		new_id++;
		new_player.sock = sock;
		if (is_wait_)
		{
			player_data_lock_.lock();
			players_data_.push_back(new_player);
			player_data_lock_.unlock();
		}
	}
}

void Server::getCommand()
{
	while (is_wait_)
	{
		player_data_lock_.lock();
		for (auto i = players_data_.begin(); i != players_data_.end(); ++i)
		{
			if ((*i).sock->available())
			{
				std::vector<CommandImformation> buf((*i).sock->available()/sizeof(CommandImformation));
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
		player_data_lock_.unlock();
	}
}

void Server::replayCommand()
{
	for (;;)
	{

		net_command_lock_.lock();
		for (auto i = net_command_buffer_.begin(); i != net_command_buffer_.end(); ++i)
		{
			switch (i->command)
			{
			case NEW_PLAYER:
				player_data_lock_.lock();
				for (auto j = players_data_.begin(); j != players_data_.end(); ++j)
				{
					std::vector<CommandImformation> buf;
					buf.clear();
					if (j->id == i->id)
					{
						CommandImformation replay;
						replay.command = REPLAY_NEW_PLAYER;
						replay.id = j->id;
						buf.push_back(replay);
						break;
					}
				}
				player_data_lock_.unlock();
				break;
			case DIRECTION:
				player_data_lock_.lock();
				for (auto j = players_data_.begin(); j != players_data_.end(); ++j)
				{
					std::vector<CommandImformation> buf;
					buf.clear();
					if (j->id != i->id)
					{
						buf.push_back(*i);
						j->sock->send(boost::asio::buffer(buf));
					}
				}
				player_data_lock_.unlock();
				break;
			default:
				break;
			}
		}
		net_command_lock_.unlock();
	}
}

void Server::beginWait()
{
	player_data_lock_.lock();
	is_wait_ = true;
	player_data_lock_.unlock();
}

void Server::endWait()
{
	player_data_lock_.lock();
	is_wait_ = false;
	player_data_lock_.unlock();
}



bool Server::excuteCommand(CommandImformation command)
{
	if (command.command == -1)
	{
		return false;
	}
	local_command_lock_.lock();
	local_command_buffer_.push_back(command);
	local_command_lock_.unlock();
	net_command_lock_.lock();
	net_command_buffer_.push_back(command);
	net_command_lock_.unlock();
	return true;
}

std::vector<CommandImformation> Server::getLocalCommand()
{
	local_command_lock_.lock();
	auto temp = local_command_buffer_;
	local_command_buffer_.clear();
	local_command_lock_.unlock();
	return temp;
}

void Server::addNetCommand(CommandImformation command)
{
	net_command_lock_.lock();
	net_command_buffer_.push_back(command);
	net_command_lock_.unlock();
}

void Server::addNetCommand(std::vector<CommandImformation> command)
{
	net_command_lock_.lock();
	net_command_buffer_.insert(net_command_buffer_.end(),command.begin(), command.end());
	net_command_lock_.unlock();
}

Server::Server():is_wait_(false),players_data_(),net_command_buffer_(),local_command_buffer_(),
                 net_command_lock_(),local_command_lock_(), player_data_lock_(),service_(),endpoint_(boost::asio::ip::tcp::v4(),PORT),log_()
{
	
}

Server::~Server()
{
	
}
