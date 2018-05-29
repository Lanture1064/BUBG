#ifndef BUBG_CLASSES_BALL_FOOD_BALL_H_
#define BUBG_CLASSES_BALL_FOOD_BALL_H_

#include"cocos2d.h"
#include"Ball\BaseBall.h"

//Please use class FoodBallManager to get new food;
class FoodBall : public BaseBall
{
public:
	CREATE_FUNC(FoodBall);
	FoodBall() = default;
	~FoodBall();
	static FoodBall* createFoodBall();
	virtual bool init();
	bool isUsed() const;
	void changeUsedState();
protected:
	bool is_used_;
};

#endif // !BUBG_CLASSES_BALL_FOOD_BALL_H_