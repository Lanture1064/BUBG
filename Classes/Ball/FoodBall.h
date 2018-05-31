#ifndef BUBG_CLASSES_BALL_FOOD_BALL_H_
#define BUBG_CLASSES_BALL_FOOD_BALL_H_

#include"cocos2d.h"

//it's used to transform the score to the size of the ball;
double ScoreToSize(unsigned int score);

//Please use class FoodBallManager to get new food;
class FoodBall : public cocos2d::Sprite
{
public:
	CREATE_FUNC(FoodBall);
	FoodBall() = default;
	~FoodBall();
	static FoodBall* createFoodBall();
	virtual bool init();
	int getScore() const;
	double getSize() const;
	bool isUsed() const;
	void changeUsedState();
protected:
	int score_;
	double size_;
	bool is_used_;
};

#endif // !BUBG_CLASSES_BALL_FOOD_BALL_H_