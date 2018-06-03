#include "BaseBall.h"
#include <cmath>
USING_NS_CC;

const std::vector<std::string>BaseBall::kColorDirectoryVec = { "ball/blue_ball.png" ,"ball/gray_ball.png","ball/green_ball.png" ,
															 "ball/red_ball.png" ,"ball/yellow_ball.png" };
double ScoreToSize(unsigned int score)
{
	return Director::getInstance()->getVisibleSize().width / 300 * std::sqrt(score);
}

BaseBall::BaseBall()
{

}


BaseBall::~BaseBall()
{

}

int BaseBall::getScore() const
{
	return score_;
}

double BaseBall::getSize() const
{
	return size_;
}

bool BaseBall::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}