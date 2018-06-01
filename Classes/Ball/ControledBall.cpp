#include "ControledBall.h"
#include "FoodBall.h"
#include "BaseBall.h"
#include <string>
#include <cmath>
USING_NS_CC;


ControledBall::~ControledBall()
{

}

ControledBall * ControledBall::createControledBall()
{
	std::string color_directory = "ball/gray_ball.png";
	return createControledBall(color_directory);
}

ControledBall * ControledBall::createControledBall(int score, std::string color_directory)
{
	ControledBall* controled_ball = new ControledBall();
	if (controled_ball&&controled_ball->init())
	{
		controled_ball->initControledBall(score, color_directory);
		return controled_ball;
	}
	CC_SAFE_DELETE(controled_ball);
	return nullptr;
}

ControledBall * ControledBall::createControledBall(std::string color_directory)
{
	const unsigned int kInitScore = 100;
	return createControledBall(kInitScore, color_directory);
}


bool ControledBall::init()
{
	if (!BaseBall::init())
	{
		return false;
	}
	return true;
}

void ControledBall::initControledBall(int score,std::string color_directory)
{
	this->initWithFile(color_directory);
	color_directory_ = color_directory;
	score_ = score;
	size_ = ScoreToSize(score_);
	speed_ = ScoreToSpeed(score_);
	manager_ = nullptr;
	is_delete_ = false;
	auto visible_size = Director::getInstance()->getVisibleSize();
	this->setScale(size_ / visible_size.width);
	this->setZOrder(score_);
}


double ControledBall::getSpeed() const
{
	return speed_;
}

std::list<FoodBall*> ControledBall::checkSwallowBall(const std::list<FoodBall*> &food_ball_list)
{
	std::list<FoodBall*> result_list;
	auto position = this->getPosition();
	for (auto i = food_ball_list.begin(); i != food_ball_list.end(); ++i)
	{
		auto food_position = (*i)->getPosition();
		if (calDistence(position, food_position) < size_*size_)
		{
			result_list.push_back(*i);
			temp_ball_storage_.push_back(*i);
		}
	}
	return result_list;
}

std::list<ControledBall*> ControledBall::checkoutSwallowBall(const std::list<ControledBall*> &controled_ball_list)
{
	std::list<ControledBall*> result_list;
	auto position = this->getPosition();
	for (auto i = controled_ball_list.begin(); i != controled_ball_list.end(); ++i)
	{
		auto ball_position = (*i)->getPosition();
		if (std::sqrt(calDistence(position, ball_position))+(*i)->size_ < size_)
		{
			result_list.push_back(*i);
			temp_ball_storage_.push_back(*i);
			(*i)->is_delete_ = true;
		}
	}
	return result_list;
}

ControledBallManager * ControledBall::getManager() const
{
	return manager_;
}

void ControledBall::setManager(ControledBallManager * manager)
{
	manager_ = manager;
}

void ControledBall::divide()
{
	score_ /= 2;
	this->updateState();
}

void ControledBall::updateState()
{
	for (auto i = temp_ball_storage_.begin(); i != temp_ball_storage_.end(); ++i)
	{
		score_ += (*i)->getScore();
	}
	size_ = ScoreToSize(score_);
	speed_ = ScoreToSpeed(score_);
	auto visible_size = Director::getInstance()->getVisibleSize();
	this->setScale(size_ / visible_size.width);
	this->setZOrder(score_);
	temp_ball_storage_.clear();
}

bool ControledBall::isDelete() const
{
	return is_delete_;
}

std::string ControledBall::getColorDirectory() const
{
	return color_directory_;
}

double calDistence(const cocos2d::Vec2 & i, const cocos2d::Vec2 & j)
{
	return (i.x - j.x)*(i.x - j.x) + (i.y - j.y)*(i.y - j.y);
}

double ScoreToSpeed(int score)
{
	return 10000/score;
}
