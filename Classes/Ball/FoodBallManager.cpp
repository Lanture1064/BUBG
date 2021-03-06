#include "FoodBallManager.h"
USING_NS_CC;

FoodBallManager * FoodBallManager::createManager(unsigned int size)
{
	auto manager = new FoodBallManager();
	if (manager&&manager->init())
	{
		manager->initManager(size);
		manager->autorelease();
		return manager;
	}
	CC_SAFE_DELETE(manager);
	return nullptr;
}

bool FoodBallManager::init()
{
	return Node::init();
}

void FoodBallManager::initManager(unsigned int size)
{
	size_ = size;
	srand(time(NULL));
	for (auto i = 0; i < size; ++i)
	{
		auto temp = FoodBall::createFoodBall();
		food_ball_container_.push_back(temp);
	}
	index_ = 0;
}
FoodBall * FoodBallManager::getNewFoodBall()
{
	//this operator is used to save time when get food for mapInit;
	for (auto i = index_; i < size_; ++i)
	{
		if (!food_ball_container_[i]->isUsed())
		{
			food_ball_container_[i]->changeUsedState();
			index_ = i + 1;
			return food_ball_container_[i];
		}
	}
	for (auto i = 0; i < index_; ++i)
	{
		if (!food_ball_container_[i]->isUsed())
		{
			food_ball_container_[i]->changeUsedState();
			index_ = i + 1;
			return food_ball_container_[i];
		}
	}
	index_ = 0;
	auto new_food = FoodBall::createFoodBall();
	if (new_food)
	{
		++size_;
		food_ball_container_.push_back(new_food);
		new_food->changeUsedState();
	}
	return new_food;
}

//the parameter is the number of the food ball need;
//Notice: the size of the return value may be not as same as the food_ball_number if there are not enouth food ball could be used;
std::list<FoodBall*> FoodBallManager::getNewFoodBall(unsigned int food_ball_number)
{
	std::list<FoodBall*> temp_container;
	int count = 0;
	for (auto i = index_; i < size_; ++i)
	{
		if (!food_ball_container_[i]->isUsed())
		{
			++count;
			temp_container.push_back(food_ball_container_[i]);
			food_ball_container_[i]->changeUsedState();
			if (count == food_ball_number)
			{
				index_ = i + 1;
				return temp_container;
			}
		}
	}
	for (auto i = 0; i < index_; ++i)
	{
		if (!food_ball_container_[i]->isUsed())
		{
			++count;
			temp_container.push_back(food_ball_container_[i]);
			food_ball_container_[i]->changeUsedState();
			if (count == food_ball_number)
			{
				index_ = i + 1;
				return temp_container;
			}
		}
	}
	for (auto i = count; i != food_ball_number; ++i)
	{
		auto new_food = FoodBall::createFoodBall();
		if (new_food)
		{
			++size_;
			food_ball_container_.push_back(new_food);
			temp_container.push_back(new_food);
			new_food->changeUsedState();
		}
	}
	return temp_container;
}

unsigned int FoodBallManager::getSize() const
{
	return size_;
}

FoodBallManager::~FoodBallManager()
{
	for (auto i = food_ball_container_.begin(); i != food_ball_container_.end(); ++i)
	{
		if ((*i)->getParent())
		{
			(*i)->removeFromParent();
			delete *i;
			*i = nullptr;
		}
		else
		{
			delete *i;
			*i = nullptr;
		}
	}
	food_ball_container_.clear();
}
