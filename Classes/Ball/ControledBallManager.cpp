#include "ControledBallManager.h"
#include "ControledBall.h"
USING_NS_CC;

void ControledBallManager::addFatherScene(cocos2d::Node * father)
{
	father_ = father;
	father->addChild(this);
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		father->addChild(*i);
	}
}

void ControledBallManager::removeBall(ControledBall * ball)
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if (*i == ball)
		{
			(*i)->removeFromParent();
		}
	}
}

void ControledBallManager::updateState()
{}

void ControledBallManager::moveTo(cocos2d::Vec2 target)
{}

void ControledBallManager::divideBall()
{}

ControledBallManager * ControledBallManager::createManager()
{
	return nullptr;
}


bool ControledBallManager::init()
{
	return false;
}

ControledBallManager::~ControledBallManager()
{}
