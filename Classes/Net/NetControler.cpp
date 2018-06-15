#include "NetControler.h"
#include "../Ball/ControledBallManager.h"
#include "NetData.h"

USING_NS_CC;

NetControler::~NetControler()
{
	
}

bool NetControler::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

NetControler * NetControler::createControler()
{
	auto controler = new NetControler();
	if (controler&&controler->init())
	{
		controler->autorelease();
		return controler;
	}
	CC_SAFE_DELETE(controler);
	return nullptr;
}

void NetControler::addCommand(CommandImformation command)
{
	command_lock_.lock();
	command_container_.push_back(command);
	command_lock_.unlock();
}

void NetControler::addManager(ControledBallManager* manager)
{
	manager_container_.push_back(manager);
}

void NetControler::update(float dt)
{
	command_lock_.lock();
	for (auto command = command_container_.begin(); command != command_container_.end(); ++command)
	{
		switch (command->command)
		{
		case DIRECTION:
			for (auto manager = manager_container_.begin(); manager != manager_container_.end(); ++manager)
			{
				if ((*manager)->getId() == command->id)
				{
					(*manager)->moveTo(Director::getInstance()->getDeltaTime(), Vec2(command->x, command->y));
					break;
				}
				break;
			}
		default:
			break;
		}
	}
	command_container_.clear();
	command_lock_.unlock();
}
