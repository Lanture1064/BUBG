#include "BaseBall.h"
#include<cmath>
USING_NS_CC;


inline double ScoreToSize(unsigned int score)
{
	return sqrt(score) + 100;
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