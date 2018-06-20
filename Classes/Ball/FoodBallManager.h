#ifndef BUBG_CLASSES_BALL_FOOD_BALL_MANAGER_H_
#define BUBG_CLASSES_BALL_FOOD_BALL_MANAGER_H_

#include"cocos2d.h"
#include "FoodBall.h"
#include<list>
#include<vector>


class FoodBallManager : public cocos2d::Node
{
public:
	FoodBallManager() = default;
	CREATE_FUNC(FoodBallManager);
	static FoodBallManager* createManager(unsigned int size=100U);
	virtual bool init();
	void initManager(unsigned int);

	FoodBall* getNewFoodBall();
	std::list<FoodBall*> getNewFoodBall(unsigned int food_ball_number);
	unsigned int getSize() const;
	~FoodBallManager();
	
private:
	std::vector<FoodBall*> food_ball_container_;
	unsigned int size_;
	unsigned int index_;
};

#endif // !BUBG_CLASSES_BALL_FOOD_BALL_MANAGER_H_