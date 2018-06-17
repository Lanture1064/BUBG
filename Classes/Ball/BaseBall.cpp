#include "BaseBall.h"
USING_NS_CC;

const std::vector<std::string>BaseBall::kColorDirectoryVec = { "ball/blue_ball.png" ,"ball/gray_ball.png","ball/green_ball.png" ,
															 "ball/red_ball.png" ,"ball/yellow_ball.png" };

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