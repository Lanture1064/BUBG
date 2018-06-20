#include "VirusBall.h"


VirusBall::~VirusBall()
{
	
}

bool VirusBall::init()
{
	if (!BaseBall::init())
	{
		return false;
	}
	if (!this->initWithFile("ball/gear.png"))
	{
		return false;
	}
	const int kInitScore = 1000;
	score_ = kInitScore;
	size_ = ScoreToSize(score_);
	is_delete_ = false;
	this->setScale(size_ / this->getContentSize().width);
	return true;
}

VirusBall * VirusBall::createBall()
{
	auto ball = new VirusBall();
	if (ball && ball->init())
	{
		ball->autorelease();
		return ball;
	}
	CC_SAFE_DELETE(ball);
	return nullptr;
}

bool VirusBall::isDelete() const
{
	return is_delete_;
}

void VirusBall::deleteVirus()
{
	is_delete_ = true;
}
