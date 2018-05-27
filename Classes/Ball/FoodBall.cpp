#include "FoodBall.h"
#include<cmath>
USING_NS_CC;

inline double ScoreToSize(unsigned int score)
{
	return sqrt(score);
}

FoodBall* FoodBall::createFoodBall()
{
	FoodBall* food_ball = new FoodBall();
	if (food_ball&&food_ball->init())
	{
		food_ball->autorelease();
		return food_ball;
	}
	CC_SAFE_DELETE(food_ball);
	return nullptr;
}

bool FoodBall::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	const unsigned int kInitScore = 20;
	this->initWithFile("ball/gray_ball.png");
	score_ = kInitScore;
	size_ = ScoreToSize(score_);
	is_used_ = false;
	auto visible_size = Director::getInstance()->getVisibleSize();
	this->setScale(size_ / visible_size.width);
	return true;
}

int FoodBall::getScore() const
{
	return score_;
}

double FoodBall::getSize() const
{
	return size_;
}

bool FoodBall::isUsed() const
{
	return is_used_;
}

FoodBall::~FoodBall()
{

}