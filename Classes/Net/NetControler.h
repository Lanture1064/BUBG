#ifndef BUBG_CLASSES_NET_NET_CONTROLER_H_
#define BUBG_CLASSES_NET_NET_CONTROLER_H_

#include "cocos2d.h"
#include <thread>
#include <vector>

struct CommandImformation;
class ControledBallManager;

class NetControler : public cocos2d::Layer
{
public:
	NetControler() = default;
	~NetControler();
	virtual bool init();
	static NetControler* createControler();
	CREATE_FUNC(NetControler);
	void addCommand(CommandImformation command);
	void addManager(ControledBallManager* manager);
	void update(float dt);
protected:
	std::vector<ControledBallManager*> manager_container_;
	std::vector<CommandImformation> command_container_;
	std::mutex command_lock_;
};

#endif // !BUBG_CLASSES_NET_NET_CONTROLER_H_
