#ifndef BUBU_CLASSES_NET_NETDATA_H_
#define BUBU_CLASSES_NET_NETDATA_H_

#include <Boost/asio.hpp>
#include <memory>

#define PORT 2018
//the command means the server end the game;
#define EXIT 0x0000

//the command is means finish the init and start the game;
#define START 0x0001

//the command is provide what position will be set for the new food;
#define NEW_FOOD 0x0002
//the command is used to add a new controled ball manager for a player;
#define NEW_MANAGER 0x0003

//the command is used to provide the direction for the controled ball manager;
#define DIRECTION 0x0004

//the command is used to join new player;
#define NEW_PLAYER 0x0005
//the command is used to return the id to the new player;
#define REPLAY_NEW_PLAYER 0x0006

#define END_GAME 0x0007

#define COMMAND_END 0x0008

#define WAITTING 0x0009
struct CommandImformation
{
	CommandImformation() :command(-1), id(0), x(0), y(0){ }
	int command;
	int id;
	float x;
	float y;
};
struct Player
{
	typedef boost::asio::ip::tcp::socket socket;
	int id;
	std::shared_ptr<socket> sock;
};
#endif // !BUBU_CLASSES_NET_NETDATA_H_

