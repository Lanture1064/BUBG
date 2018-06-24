#ifndef BUBU_CLASSES_NET_NETDATA_H_
#define BUBU_CLASSES_NET_NETDATA_H_

#include <Boost/asio.hpp>
#include <memory>

#define PORT 2018
#define MESSAGE_PORT 1998

#define USE_SERVER 1
#define USE_CLIENT 2

//the command means the server end the game;
#define EXIT_GAME 0x0000

//the command is means finish the init and start the game;
#define INIT_END 0x0001

//the command is provide what position will be set for the new food;
#define NEW_FOOD 0x0002
//the command is used to add a new controled ball manager for a player;
#define NEW_MANAGER 0x0003

//the command is used to provide the direction for the controled ball manager;
#define DIRECTION 0x0004

//the command is used to join new player;
#define NEW_PLAYER 0x0005
//the command is used to return the id to the new player;
#define REPLY_NEW_PLAYER 0x0006

#define END_GAME 0x0007

#define DIVIDE 0x0008

#define NEW_VIRUS 0x0009

#define DIRECTION_BY_KEY 0x000A

#define REPLY_EXIT 0x000B
#define REPLY_END 0x000C
class Client;
class Server;
struct CommandImformation
{
public:
	CommandImformation() :command(-1), id(0), x(0), y(0){ }
	int command;
	int id;
	float x;
	float y;
};

struct Player
{
	friend class Client;
	friend class Server;
public:
	typedef boost::asio::ip::tcp::socket socket;
	Player():id(-1),sock(nullptr) { }
	int id;
private:
	std::shared_ptr<socket> sock;
	std::shared_ptr<socket> message_sock;
};
#endif // !BUBU_CLASSES_NET_NETDATA_H_

