#ifndef BUBG_CLASSES_NET_SERVER_H_
#define BUBG_CLASSES_NET_SERVER_H_

#include <vector>
#include <string>
#include <thread>
#include "NetData.h"
#include "Boost/asio.hpp"
class Server
{
	typedef boost::asio::ip::address address;
public:
	static Server* getInstance();
	void claer();
	void getCommand();
	void replayCommand();
	void changeStartState();
	std::vector<CommandImformation> getLocalCommand();
	void addNetCommand(CommandImformation coammand);
	void addNetCommand(std::vector<CommandImformation> command);
protected:
	Server();
	~Server();
	Server& operator=(Server& )=delete;
	static Server* instance;
	bool is_start_;
	std::vector<Player> players_data_;
	std::vector<CommandImformation> net_command_buffer_;
	std::vector<CommandImformation> local_command_buffer_;
	address ip_;
	std::string log_;
	std::mutex net_command_lock_;
	std::mutex local_command_lock_;
};

#endif // !BUBG_CLASSES_NET_SERVER_H_

