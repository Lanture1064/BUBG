#ifndef BUBG_CLASSES_NET_SERVER_H_
#define BUBG_CLASSES_NET_SERVER_H_

#include <vector>
#include <string>
#include "NetData.h"
#include <Boost/asio.hpp>
class Server
{
	typedef boost::asio::io_service io_service;
	typedef boost::asio::ip::tcp::endpoint endpoint;
public:
	static Server* getInstance();
	void claer();
	void connect();
	void getCommand();
	void replayCommand();
	void beginWait();
	void endWait();
	bool excuteCommand(CommandImformation command);
	std::vector<CommandImformation> getLocalCommand();
	void addNetCommand(CommandImformation coammand);
	void addNetCommand(std::vector<CommandImformation> command);
protected:
	Server();
	~Server();
	Server& operator=(const Server& )=delete;
	static int new_id;
	bool is_wait_;
	std::vector<Player> players_data_;
	std::vector<CommandImformation> net_command_buffer_;
	std::vector<CommandImformation> local_command_buffer_;
	std::string log_;
	std::mutex net_command_lock_;
	std::mutex local_command_lock_;
	std::mutex player_data_lock_;
	io_service service_;
	endpoint endpoint_;
};

#endif // !BUBG_CLASSES_NET_SERVER_H_