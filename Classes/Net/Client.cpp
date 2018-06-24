#include "Client.h"
#include <Boost/asio.hpp>
#include <sstream>
#include <thread>

Client::Client() :service_(), local_command_buffer_(), local_command_lock_(), player_()
{
	boost::asio::ip::address add;
	add = boost::asio::ip::address::from_string("127.0.0.1");
	server_.address(add);
	server_.port(PORT);
	message_server_.address(add);
	message_server_.port(MESSAGE_PORT);
}

Client::~Client()
{
	this->clear();
}

Client * Client::getInstance()
{
	static Client instance;
	return &instance;
}


std::vector<CommandImformation> Client::getLocalCommand()
{
	local_command_lock_.lock();
	auto command = local_command_buffer_;
	local_command_buffer_.clear();
	local_command_lock_.unlock();
	return command;

}

bool Client::connect()
{
	try
	{
		if (player_.sock == nullptr)
		{
			player_.sock = std::make_shared<Player::socket>(service_);
		}
		if (player_.message_sock == nullptr)
		{
			player_.message_sock = std::make_shared<Player::socket>(service_);
		}
		player_.sock->connect(server_);
		std::vector<CommandImformation> buf;
		CommandImformation command;
		command.command = NEW_PLAYER;
		buf.push_back(command);
		player_.sock->send(boost::asio::buffer(buf));
		player_.message_sock->connect(message_server_);
		std::thread t(&Client::wait, this);
		t.detach();
		return true;
	} catch (std::exception& e)
	{
		player_.sock = nullptr;
		player_.message_sock = nullptr;
		std::ostringstream os;
		std::string temp;
		os.str(temp);
		os << e.what() << std::endl;
		log_ += temp;
		return false;
	}
}

void Client::getCommand()
{
	while(is_in_game_)
	{
		get_command_lock_.lock();
		if (player_.sock != nullptr)
		{
			if (player_.sock->available())
			{
				std::vector<CommandImformation> buf(player_.sock->available() / sizeof(CommandImformation));
				player_.sock->receive(boost::asio::buffer(buf));
				for (auto i = buf.begin(); i != buf.end(); ++i)
				{
					if (!excuteCommand(*i))
					{
						break;
					}
				}
			}
		}
		get_command_lock_.unlock();
		
	}
}


void Client::sendCommand(CommandImformation command)
{
	std::vector<CommandImformation> buf;
	buf.push_back(command);
	send_command_lock_.lock();
	if (player_.sock != nullptr)
	{
		player_.sock->send(boost::asio::buffer(buf));
	}
	send_command_lock_.unlock();
}

std::string Client::getMessage()
{
	get_message_lock_.lock();
	if (player_.message_sock != nullptr)
	{
		if (player_.message_sock->available())
		{
			auto text = std::string();
			auto buf = std::string(1, '\0');
			for (;;)
			{
				player_.message_sock->receive(boost::asio::buffer(buf));
				if (*buf.begin() == '\n')
				{
					break;
				}
				text += buf;
			}
			get_message_lock_.unlock();
			return text;
		}
		else
		{
			get_message_lock_.unlock();
			return std::string();
		}
	}
	get_message_lock_.unlock();
	return std::string();
}

void Client::sendMessage(std::string text)
{
	text += "\n";
	send_message_lock_.lock();
	if (player_.message_sock!= nullptr)
	{
		player_.message_sock->send(boost::asio::buffer(text));
	}
	send_message_lock_.unlock();
}

void Client::startGame()
{
	is_in_game_ = true;
}

void Client::endGame()
{
	is_in_game_ = true;
}

void Client::setServerIp(std::string ip)
{
	server_.address(boost::asio::ip::address::from_string(ip));
}

bool Client::excuteCommand(CommandImformation command)
{
	if (command.command == -1)
	{
		return false;
	}
	switch (command.command)
	{
	case REPLY_NEW_PLAYER:
		player_.id = command.id;
		break;
	case END_GAME:
	{
		std::vector<CommandImformation> buf;
		CommandImformation temp;
		temp.command = REPLY_END;
		buf.push_back(temp);
		player_.sock->send(boost::asio::buffer(buf));
		this->closeSocket();
	}
	case DIRECTION: case DIRECTION_BY_KEY: case NEW_MANAGER: case NEW_FOOD: case INIT_END: case DIVIDE: case NEW_VIRUS:  default:
		local_command_lock_.lock();
		local_command_buffer_.push_back(command);
		local_command_lock_.unlock();
		break;
	}
	return true;
}

void Client::clear()
{
	is_in_game_ = false;
	if (player_.sock)
	{
		CommandImformation command;
		command.command = EXIT_GAME;
		command.id = Client::getId();
		auto buf = std::vector<CommandImformation>();
		buf.push_back(command);
		player_.sock->send(boost::asio::buffer(buf));
		for (;;)
		{
			int state = 0;
			if (player_.sock->available())
			{
				buf.resize(player_.sock->available() / sizeof(CommandImformation));
				player_.sock->receive(boost::asio::buffer(buf));
				for (auto i = buf.begin(); i != buf.end(); ++i)
				{
					if (i->command == REPLY_EXIT)
					{
						state = 1;
						break;
					}
				}
			}
			if (state)
			{
				break;
			}
		}
		send_command_lock_.lock();
		get_command_lock_.lock();
		player_.sock->close();
		player_.sock = nullptr;
		send_command_lock_.unlock();
		get_command_lock_.unlock();
		
	}
	if (player_.message_sock)
	{
		get_message_lock_.lock();
		send_message_lock_.lock();
		player_.message_sock->close();
		player_.message_sock = nullptr;
		get_message_lock_.unlock();
		send_message_lock_.unlock();
	}

	local_command_lock_.lock();
	local_command_buffer_.clear();
	local_command_lock_.unlock();

	this->setServerIp("127.0.0.1");
}

int Client::getId() const
{
	return player_.id;
}

void Client::wait()
{
	for (;;)
	{
		if (player_.sock->available())
		{
			this->startGame();
			break;
		}
	}
}

bool Client::isStart() const
{
	return is_in_game_;
}

void Client::closeSocket()
{
	player_.message_sock->close();
	player_.sock->close();
	player_.message_sock = nullptr;
	player_.sock = nullptr;
}
