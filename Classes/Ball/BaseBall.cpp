#include "BaseBall.h"
#include <cmath>
USING_NS_CC;


double ScoreToSize(unsigned int score)
{
	return Director::getInstance()->getVisibleSize().width / 50 * std::sqrt(score);
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