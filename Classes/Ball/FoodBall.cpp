#include "FoodBall.h"
#include "BaseBall.h"
#include <string>
USING_NS_CC;


FoodBall* FoodBall::createFoodBall()
{
	FoodBall* food_ball = new FoodBall();
	if (food_ball&&food_ball->init())
	{
		return food_ball;
	}
	CC_SAFE_DELETE(food_ball);
	return nullptr;
}

bool FoodBall::init()
{
	if (!BaseBall::init())
	{
		return false;
	}
	const unsigned int kInitScore = 20;
	auto color_index = rand() % kColorNumber;
	this->initWithFile(BaseBall::kColorDirectoryVec[color_index]);
	score_ = kInitScore;
	size_ = ScoreToSize(score_);
	is_used_ = false;
	auto visible_size = Director::getInstance()->getVisibleSize();
	this->setScale(size_ / visible_size.width);
	return true;
}


bool FoodBall::isUsed() const
{
	return is_used_;
}

void FoodBall::changeUsedState()
{
	is_used_ = 1 - is_used_;
}

FoodBall::~FoodBall()
{
	
}