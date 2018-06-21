#include "ControledBall.h"
#include "FoodBall.h"
#include "BaseBall.h"
#include "../Tool/MathTool.h"
#include <string>
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
		controled_ball->autorelease();
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
	is_divided_ = false;
	is_swallow_virus_ = false;
	time_count_ = 0;
	divide_direction_ = Vec2::ZERO;
	direction_when_swallow_virus_ = Vec2::ZERO;
	this->setScale(size_ / this->getContentSize().width);
	this->setZOrder(score_);
}


double ControledBall::getSpeed() const
{
	return speed_;
}

int ControledBall::checkSwallowBall(std::list<FoodBall*> &food_ball_list)
{
	int count = 0;
	auto position = this->getPosition();
	for (auto i = food_ball_list.begin(); i != food_ball_list.end();)
	{
		if ((*i)->isUsed())
		{
			auto food_position = (*i)->getPosition();
			if (calDistence(position, food_position) < 0.9*size_*size_/4)
			{
				(*i)->changeUsedState();
				(*i)->removeFromParent();
				temp_ball_storage_.push_back(*i);
				count++;
				(*i) = nullptr;
				i = food_ball_list.erase(i);
				continue;
			}
		}
		++i;
	}
	return count;
}

void ControledBall::checkSwallowBall(const std::list<ControledBall*> &controled_ball_list)
{
	auto position = this->getPosition();
	for (auto i = controled_ball_list.begin(); i != controled_ball_list.end(); ++i)
	{
		auto ball_position = (*i)->getPosition();
		
		if (std::sqrt(calDistence(position, ball_position))+((*i)->size_)/2 < size_/2)
		{
			if (!(*i)->is_delete_&&!((*i)->isDivided()&&(*i)->getParent()==this->getParent()))
			{
				temp_ball_storage_.push_back(*i);
				(*i)->is_delete_ = true;
			}
		}

	}
}



int ControledBall::checkSwallowVirus(const std::list<VirusBall*>& virus_ball_list)
{
	auto position = this->getPosition();
	int count = 0;
	for (auto i = virus_ball_list.begin(); i != virus_ball_list.end(); ++i)
	{
		auto ball_position = (*i)->getPosition();
		if (std::sqrt(calDistence(position, ball_position)) + ((*i)->getSize()) / 2 < size_ / 2)
		{	
			if (!(*i)->isDelete())
			{
				//temp_ball_storage_.push_back(*i);
				(*i)->deleteVirus();
				if (!is_swallow_virus_)
				{
					is_swallow_virus_ = true;
				}
				count++;
			}
		}
	}
	return count;
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
	this->setScale(size_ / this->getContentSize().width);
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

void ControledBall::changeDividedState()
{
	is_divided_ = 1 - is_divided_;
}

bool ControledBall::isDivided() const
{
	return is_divided_;
}

bool ControledBall::isSwallowVirus() const
{
	return is_swallow_virus_;
}


void ControledBall::changeSwallowVirusState()
{
	is_swallow_virus_ = 1 - is_swallow_virus_;
}

void ControledBall::setDirectionWhenSwallowVirus(cocos2d::Vec2 direction)
{
	direction_when_swallow_virus_ = direction;
}

cocos2d::Vec2 ControledBall::getDirectionWhenSwallow() const
{
	return direction_when_swallow_virus_;
}

void ControledBall::countTime()
{
	++time_count_;
}

int ControledBall::getTimeCount() const
{
	return time_count_;
}

void ControledBall::resetTimeCount()
{
	time_count_ = 0;
}

cocos2d::Vec2 ControledBall::getDivideDirection() const
{
	return divide_direction_;
}

void ControledBall::setDivideDirection(cocos2d::Vec2 direction)
{
	divide_direction_ = direction;
}

