#include <boost\asio.hpp>
#include <iostream>
#include <thread>
#include "Client.h"
#include "Server.h"
#include "NetData.h"


int main()
{
	int temp;
	std::cin >> temp;
	if (temp == 1)
	{
		Server::getInstance()->beginWait();
		std::thread t1(&Server::connect, (Server::getInstance()));
		t1.detach();
		for (;;)
		{
			if (Server::getInstance()->getPlayer().size())
			{
				Server::getInstance()->endWait();
				break;
			}
		}
		std::thread t2(&Server::getCommand, (Server::getInstance()));
		std::thread t3(&Server::replayCommand,(Server::getInstance()));
		t2.detach();
		t3.detach();
		for (;;)
		{
			auto i = Server::getInstance()->getLocalCommand();
			for (auto j = i.begin(); j != i.end(); ++j)
			{
				std::cout << j->command << std::endl;
			}
		}
	}
	else if (temp == 2)
	{
		Client::getInstance()->connect();
		//std::thread t1(&Client::getCommand,(Client::getInstance()));
		std::thread t2(&Client::replayCommand,(Client::getInstance()));
		//t1.detach();
		t2.detach();
		for (;;)
		{
			auto i = Client::getInstance()->getLocalCommand();
			for (auto j = i.begin(); j != i.end(); ++j)
			{
				std::cout << j->command << std::endl;
			}
			CommandImformation command;
			command.command = DIRECTION;

			Client::getInstance()->addNetCommand(command);
		}
		int l = 0;
	}
	return 0;
}
