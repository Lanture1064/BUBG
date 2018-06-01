#include "ControledBallManager.h"
#include "ControledBall.h"
#include <cmath>
USING_NS_CC;

std::vector<bool> ControledBallManager::kUsedColor(kColorNumber, false);

void ControledBallManager::addFatherScene(cocos2d::Node * father)
{
	father_ = father;
	father->addChild(this);
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		father->addChild(*i);
	}
}

void ControledBallManager::removeBall()
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end();)
	{
		if ((*i)->isDelete())
		{
			(*i)->removeFromParent();
			delete (*i);
			*i = nullptr;
			i = controled_ball_list_.erase(i);
			continue;
		}
		++i;
	}
}

void ControledBallManager::updateState()
{
	auto position = Vec2::ZERO;
	speed_ = 0;
	int count = 0;
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if (!(*i)->isDelete())
		{
			(*i)->updateState();
			position += (*i)->getPosition();
			speed_ += (*i)->getSpeed();
			count++;
		}
	}
	position.x /= count;
	position.y /= count;
	this->setPosition(position);
	speed_ /= count;
}

void ControledBallManager::moveTo(double time,cocos2d::Vec2 target)
{	
	//auto position = this->getPosition();
	
	 for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		auto position = (*i)->getPosition();
		double cos_val, sin_val;
		double distence = pow(calDistence(position, target), 0.5);
		if (distence == 0)
		{
			return;
		}
		cos_val = (target.x - position.x) / distence;
		sin_val = (target.y - position.y) / distence;

		auto x_rate = speed_ * cos_val;
		auto y_rate = speed_ * sin_val;

		auto move = MoveBy::create(time, Vec2(x_rate*time, y_rate*time));
		(*i)->runAction(move);
	}
}

void ControledBallManager::divideBall(cocos2d::Vec2 target)
{
	auto position = this->getPosition();
	auto angle = std::atan((target.x - position.x) / (target.y - position.y));
	std::list<ControledBall*> append_list;
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		(*i)->divide();
		auto ball = ControledBall::createControledBall((*i)->getScore(), color_directory_);
		append_list.push_back(ball);
		auto x_rate = (*i)->getSpeed()*std::cos(angle)*5;
		auto y_rate = (*i)->getSpeed()*std::sin(angle)*5;
		auto move = MoveBy::create(1.0f, Vec2(x_rate, y_rate));
		ball->runAction(move);
	}
	controled_ball_list_.insert(controled_ball_list_.end(),append_list.begin(),append_list.end());
}

bool ControledBallManager::isDead()
{
	return is_dead_;
}

ControledBallManager * ControledBallManager::createManager()
{
	std::string color_directory;
	for (auto i = 0; i != kUsedColor.size(); ++i)
	{
		if (!kUsedColor[i])
		{
			kUsedColor[i] = true;
			color_directory = BaseBall::kColorDirectoryVec[i];
		}
	}
	return createManager(color_directory);
}


ControledBallManager * ControledBallManager::createManager(std::string color_directory)
{
	auto manager = new ControledBallManager();

	if (manager&&manager->init())
	{
		manager->initManager(color_directory);
		manager->autorelease();
		return manager;
	}
	CC_SAFE_DELETE(manager);
	return nullptr;
}


bool ControledBallManager::init()
{
	if (!Node::init())
	{
		return false;
	}
	return true;
}

void ControledBallManager::initManager(std::string color_directory)
{
	auto ball = ControledBall::createControledBall(color_directory);
	controled_ball_list_.push_back(ball);
	speed_ = ball->getSpeed();
	color_directory_ = color_directory;
	is_dead_ = false;
}

ControledBallManager::~ControledBallManager()
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		delete (*i);
		(*i) = nullptr;
	}
}
