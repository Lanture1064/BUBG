#include "FoodBallManager.h"
USING_NS_CC;


FoodBallManager::FoodBallManager(unsigned int size) :size_(size), index_(0), food_ball_container(size, FoodBall::createFoodBall())
{

}


FoodBall * FoodBallManager::getNewFoodBall()
{
	//this operator is used to save time when get food for mapInit;
	for (auto i = index_; i < size_; ++i)
	{
		if (!food_ball_container[i]->isUsed())
		{
			index_ = i + 1;
			return food_ball_container[i];
		}
	}
	for (auto i = 0; i < index_; ++i)
	{
		if (!food_ball_container[i]->isUsed())
		{
			index_ = i + 1;
			return food_ball_container[i];
		}
	}
	index_ = 0;
	return nullptr;
}

//the parameter is the number of the food ball need;
//Notice: the size of the return value may be not as same as the food_ball_number if there are not enouth food ball could be used;
std::list<FoodBall*> FoodBallManager::getNewFoodBall(unsigned int food_ball_number)
{
	std::list<FoodBall*> temp_container;
	int count = 0;
	for (auto i = index_; i < size_; ++i)
	{
		if (!food_ball_container[i]->isUsed())
		{
			++count;
			temp_container.push_back(food_ball_container[i]);
			if (count == food_ball_number)
			{
				index_ = i + 1;
				return temp_container;
			}
			
		}
	}
	for (auto i = 0; i < index_; ++i)
	{
		if (!food_ball_container[i]->isUsed())
		{
			++count;
			temp_container.push_back(food_ball_container[i]);
			if (count == food_ball_number)
			{
				index_ = i + 1;
				return temp_container;
			}
		}
	}
	index_ = 0;
	return temp_container;
}

FoodBallManager::~FoodBallManager()
{

}
