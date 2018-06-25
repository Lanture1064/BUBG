#ifndef BUBG_CLASSES_NET_SERVER_H_
#define BUBG_CLASSES_NET_SERVER_H_

#include <vector>
#include <string>
#include <thread>
#include "NetData.h"
#include <Boost/asio.hpp>
class Server
{
	typedef boost::asio::io_service io_service;
	typedef boost::asio::ip::tcp::endpoint endpoint;
public:
	static Server* getInstance();
	//clear the data when game end;
	void clear();
	//get connect with the clients and add to players_data;
	void connect();
	//get the command from other client;
	void getCommand();

	std::vector<std::string> getMessage();
	void sendMessage(std::string);
	void beginWait();
	void endWait();
	void startGame();
	void endGame();
	bool excuteCommand(CommandImformation command);
	std::vector<CommandImformation> getLocalCommand();
	//this function send the command directly
	void sendCommand(CommandImformation command);
	const std::vector<Player>& getPlayer() const;
	std::string getIp() const;
protected:
	Server();
	~Server();
	Server& operator=(const Server& )=delete;
	static int new_id;
	bool is_wait_;
	bool is_in_game_;
	std::vector<Player> players_data_;
	std::vector<CommandImformation> local_command_buffer_;
	std::string log_;
	std::mutex local_command_lock_;
	std::mutex close_sock_lock_;
	std::mutex close_message_lock_;
	std::mutex clear_sock_lock_;
	std::mutex clear_message_lock_;
	io_service service_;
	endpoint endpoint_;
	endpoint message_endpoint_;
	std::shared_ptr<boost::asio::ip::tcp::acceptor> aceptor_;
};

#endif // !BUBG_CLASSES_NET_SERVER_H_