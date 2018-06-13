#ifndef BUBG_CLASSES_NET_CLIENT_H_
#define BUBG_CLASSES_NET_CLIENT_H_

#include <vector>
#include <thread>
#include "NetData.h"
#include <boost/asio.hpp>

class Client
{
	typedef boost::asio::ip::tcp::endpoint endpoint;

protected:
	Client();
	~Client();
	Client& operator=(const Client &) = delete;
public:
	static Client* getInstance();
	void addNetCommand(CommandImformation command);
	void getLocalCommand();
	void connect();
	void replayCommand();
	void setServerIp();
protected:
	Player player_;
	endpoint server_;
	std::vector<CommandImformation> local_command_buffer_;
	std::vector<CommandImformation> net_command_buffer_;
	std::mutex local_command_lock_;
	std::mutex local_command_lock_;
};

#endif // !BUBG_CLASSES_NET_CLIENT_H_


