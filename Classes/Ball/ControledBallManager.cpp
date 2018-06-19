#include "SimpleAudioEngine.h"
#include "../SettingHeader.h"
#include "ControledBallManager.h"
#include "ControledBall.h"
#include "FoodBall.h"
#include "../Tool/MathTool.h"
USING_NS_CC;

using namespace CocosDenshion;

std::vector<bool> ControledBallManager::kUsedColor(kColorNumber, false);

int ControledBallManager::getId() const
{
	return id_;
}

void ControledBallManager::setId(int id)
{
	id_ = id;
}

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

void ControledBallManager::removeBall(ControledBall * ball)
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if ((*i) == ball)
		{
			(*i)->removeFromParent();
			delete (*i);
			*i = nullptr;
			i = controled_ball_list_.erase(i);
			return;
		}
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

		double ratio = 1.0;

		if (distence <(*i)->getSize())
		{
			ratio = distence / (*i)->getSize();
		}
		if (distence)
		{
			cos_val = (target.x - position.x) / distence;
			sin_val = (target.y - position.y) / distence;
		}
		else
		{
			cos_val = 0;
			sin_val = 0;
		}
		auto x_rate = speed_ * cos_val;
		auto y_rate = speed_ * sin_val;

		if ((*i)->isDivided())
		{
			//the ball dash when ball divided;
			ratio = 3.0 - 2.0 * (*i)->getTimeCount() / 60.0;
			if ((*i)->getTimeCount() == 0)
			{
				(*i)->setDivideDirection(Vec2(cos_val, sin_val));
			}
			auto direction = (*i)->getDivideDirection();
			x_rate = speed_ * direction.x;
			y_rate = speed_ * direction.y;
			if ((*i)->getTimeCount() == 30)
			{
				(*i)->changeDividedState();
				(*i)->resetTimeCount();
			}
			else
			{
				(*i)->countTime();
			}
		}
		auto move = MoveBy::create(time, Vec2(x_rate*time * ratio, y_rate*time * ratio));
		(*i)->runAction(move);
	}
}

void ControledBallManager::moveByKey(double time, cocos2d::Vec2 direction_count)
{
	auto x_speed = direction_count.x * speed_ / 60.0;
	auto y_speed = direction_count.y * speed_ / 60.0;
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if ((*i)->isDivided())
		{
			auto ratio = 3.0 - 2.0 * (*i)->getTimeCount() / 60.0;
			if ((*i)->getTimeCount() == 0)
			{
				auto cos_val = x_speed / std::pow((x_speed*x_speed + y_speed * y_speed), 0.5);
				auto sin_val = y_speed / std::pow((x_speed*x_speed + y_speed * y_speed), 0.5);
				(*i)->setDivideDirection(Vec2(cos_val, sin_val));
			}
			auto direction = (*i)->getDivideDirection();
			x_speed = speed_ * direction.x * ratio;
			y_speed = speed_ * direction.y * ratio;
			if ((*i)->getTimeCount() == 30)
			{
				(*i)->changeDividedState();
				(*i)->resetTimeCount();
			}
			else
			{
				(*i)->countTime();
			}
		}
		auto move = MoveBy::create(time, Vec2(x_speed*time, y_speed*time));
		(*i)->runAction(move);		
	}
}

void ControledBallManager::divideBall(cocos2d::Vec2 target)
{
	auto position = this->getPosition();
	std::list<ControledBall*> append_list;
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if ((*i)->getScore() > 400)
		{
			(*i)->divide();
			auto ball = ControledBall::createControledBall((*i)->getScore(), color_directory_);
			ball->setManager(this);
			ball->changeDividedState();
			father_->addChild(ball);
			ball->setPosition((*i)->getPosition());
			append_list.push_back(ball);
		}
	}
	controled_ball_list_.insert(controled_ball_list_.end(),append_list.begin(),append_list.end());
	all_controled_ball_list_->insert(all_controled_ball_list_->end(), append_list.begin(), append_list.end());
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/bubble.mp3");
	}
}

const std::list<ControledBall*> &ControledBallManager::getBallList() const
{
	return controled_ball_list_;
}

std::pair<unsigned int,unsigned int> ControledBallManager::swallow(std::list<FoodBall*> &food_ball_list, std::list<ControledBall*> &controled_ball_list)
{
	checkSwallowBall(food_ball_list);
	checkSwallowBall(controled_ball_list);
	this->updateState();
	std::pair<unsigned int, unsigned int> result = std::make_pair<unsigned int, unsigned>(0, 0);
	for (auto i = food_ball_list.begin(); i != food_ball_list.end();)
	{
		if (!(*i)->isUsed())
		{
			(*i)->removeFromParent();
			(*i) = nullptr;
			i = food_ball_list.erase(i);
			result.first++;
			continue;
		}
		++i;
	}
	for (auto i = controled_ball_list.begin(); i != controled_ball_list.end();)
	{
		if ((*i)->isDelete())
		{
			(*i)->getManager()->removeBall(*i);
			(*i) = nullptr;
			i = controled_ball_list.erase(i);
			result.second++;
			continue;
		}
		++i;
	}
	return result;
}

void ControledBallManager::checkSwallowBall(const std::list<ControledBall*> &ball_list)
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if (!(*i)->isDelete())
		{
			(*i)->checkSwallowBall(ball_list);
		}
	}
}
void ControledBallManager::checkSwallowBall(const std::list<FoodBall*> &ball_list)
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		if (!(*i)->isDelete())
		{
			(*i)->checkSwallowBall(ball_list);
		}
	}
}

unsigned int ControledBallManager::isDead()
{
	return controled_ball_list_.size();
}


ControledBallManager * ControledBallManager::createManager(std::list<ControledBall*> *all_controled_ball_list)
{
	return createManager(all_controled_ball_list, Vec2::ZERO);
}
ControledBallManager * ControledBallManager::createManager(std::list<ControledBall*> *all_controled_ball_list, cocos2d::Vec2 position)
{
	std::string name = "guest";
	return createManager(all_controled_ball_list, position, name);
}

ControledBallManager * ControledBallManager::createManager(std::list<ControledBall*> *all_controled_ball_list, cocos2d::Vec2 position, std::string name)
{
	std::string color_directory;
	for (auto i = 0; i != kUsedColor.size(); ++i)
	{
		if (!kUsedColor[i])
		{
			kUsedColor[i] = true;
			color_directory = BaseBall::kColorDirectoryVec[i];
			break;
		}
		if (i == kUsedColor.size() - 1)
		{
			color_directory = BaseBall::kColorDirectoryVec[0];
		}
	}
	return createManager(all_controled_ball_list, position, name,color_directory);
}


ControledBallManager * ControledBallManager::createManager(std::list<ControledBall*> *all_controled_ball_list, cocos2d::Vec2 position,
														   std::string name,std::string color_directory)
{
	auto manager = new ControledBallManager();

	if (manager&&manager->init())
	{
		manager->initManager(all_controled_ball_list,position, name, color_directory);
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

void ControledBallManager::initManager(std::list<ControledBall*> *all_controled_ball_list, cocos2d::Vec2 position,
									   std::string name, std::string color_directory)
{
	auto ball = ControledBall::createControledBall(color_directory);
	ball->setPosition(position);
	this->setPosition(position);
	name_ = name;
	ball->setManager(this);
	controled_ball_list_.push_back(ball);
	all_controled_ball_list_ = all_controled_ball_list;
	all_controled_ball_list_->push_back(ball);
	speed_ = ball->getSpeed();
	color_directory_ = color_directory;
	id_ = -1;
}

ControledBallManager::~ControledBallManager()
{
	for (auto i = controled_ball_list_.begin(); i != controled_ball_list_.end(); ++i)
	{
		delete (*i);
		(*i) = nullptr;
	}
	for (auto i = 0; i < kColorNumber; ++i)
	{
		if (color_directory_ == BaseBall::kColorDirectoryVec[i])
		{
			kUsedColor[i] = false;
		}
	}
}

