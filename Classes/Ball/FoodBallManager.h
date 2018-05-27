#ifndef BUBG_CLASSES_BALL_FOOD_BALL_MANAGER_H_
#define BUBG_CLASSES_BALL_FOOD_BALL_MANAGER_H_

#include"FoodBall.h"
#include<list>
#include<vector>

class FoodBallManager
{
public:
	FoodBallManager(unsigned int size=100);
	FoodBall* getNewFoodBall();
	std::list<FoodBall*> getNewFoodBall(unsigned int food_ball_number);
	~FoodBallManager();
private:
	std::vector<FoodBall*> food_ball_container;
	unsigned int size_;
	unsigned int index_;
};

#endif // !BUBG_CLASSES_BALL_FOOD_BALL_MANAGER_H_