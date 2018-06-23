#ifndef BUBG_CLASSES_NET_CLIENT_H_
#define BUBG_CLASSES_NET_CLIENT_H_

#include <vector>
#include <thread>
#include <string>
#include "NetData.h"
#include <boost/asio.hpp>

class Client
{
	typedef boost::asio::ip::tcp::endpoint endpoint;
	typedef boost::asio::io_service service;
protected:
	Client();
	~Client();
	Client& operator=(const Client &) = delete;
public:
	static Client* getInstance();
	void addNetCommand(CommandImformation command);
	std::vector<CommandImformation> getLocalCommand();
	bool connect();
	void getCommand();
	//this function send te command by,command buffer,which is influenced by mutex;
	//should be instead of sendCommand;
	void replayCommand();
	//this function send the command directly
	void sendCommand(CommandImformation command);
	std::string getMessage();
	void sendMessage(std::string text);
	void startGame();
	void endGame();
	void setServerIp(std::string ip);
	bool excuteCommand(CommandImformation command);
	void clear();
	int getId() const;
	void wait();
	bool isStart() const;
protected:
	bool is_in_game_;
	service service_;
	Player player_;
	endpoint server_;
	endpoint message_server_;
	std::vector<CommandImformation> local_command_buffer_;
	std::vector<CommandImformation> net_command_buffer_;
	std::mutex local_command_lock_;
	std::mutex net_command_lock_;
	std::string log_;
};

#endif // !BUBG_CLASSES_NET_CLIENT_H_


