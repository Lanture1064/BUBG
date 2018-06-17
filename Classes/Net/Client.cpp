#include "Client.h"
#include <Boost/asio.hpp>
#include <sstream>

Client::Client() :service_(), local_command_buffer_(), net_command_buffer_(), local_command_lock_(), net_command_lock_(), player_()
{
	boost::asio::ip::address add;
	add = boost::asio::ip::address::from_string("127.0.0.1");
	server_.address(add);
	server_.port(PORT);
}

Client::~Client()
{

}

Client * Client::getInstance()
{
	static Client instance;
	return &instance;
}

void Client::addNetCommand(CommandImformation command)
{
	net_command_lock_.lock();
	net_command_buffer_.push_back(command);
	net_command_lock_.unlock();
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
		player_.sock->connect(server_);
		std::vector<CommandImformation> buf;
		CommandImformation command;
		command.command = NEW_PLAYER;
		net_command_lock_.lock();
		net_command_buffer_.push_back(command);
		net_command_lock_.unlock();
		return true;
	} catch (std::exception& e)
	{
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
}

void Client::replayCommand()
{
	while(is_in_game_)
	{
		net_command_lock_.lock();
		for (auto i = net_command_buffer_.begin(); i != net_command_buffer_.end(); ++i)
		{
			std::vector<CommandImformation> buf;
			buf.push_back(*i);
			player_.sock->send(boost::asio::buffer(buf));
		}
		net_command_buffer_.clear();
		net_command_lock_.unlock();
	}
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
	case REPLAY_NEW_PLAYER:
		player_.id = command.id;
		break;
	case DIRECTION: case NEW_MANAGER:
		local_command_lock_.lock();
		local_command_buffer_.push_back(command);
		local_command_lock_.unlock();
		break;
	default:
		break;
	}
	return true;
}

void Client::clear()
{
	is_in_game_ = false;
	Sleep(10);

	net_command_lock_.lock();
	net_command_buffer_.clear();
	net_command_lock_.unlock();

	local_command_lock_.lock();
	local_command_buffer_.clear();
	local_command_lock_.unlock();

	this->setServerIp("127.0.0.1");
}
