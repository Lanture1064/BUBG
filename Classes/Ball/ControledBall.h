#ifndef BUBG_CLASSES_BALL_CONTROLED_BALL_H_
#define BUBG_CLASSES_BALL_CONTROLED_BALL_H_

#include"cocos2d.h"
#include"Ball\BaseBall.h"
#include"Ball\FoodBall.h"
#include<list>

class ControledBall : public BaseBall
{
public:
	CREATE_FUNC(ControledBall);
	ControledBall() = default;
	~ControledBall();
	static FoodBall* createControledBall();
	virtual bool init();
	double getSpeed() const;
	std::list<FoodBall*> checkSwallow(std::list<FoodBall*> food_ball_list);
	std::list<ControledBall*> checkoutSwallow(std::list<ControledBall*> controled_ball_list);
	void updateState();

protected:
	double speed_;
	//the list is store the ball will be swallowed;
	std::list<BaseBall*> temp_ball_storage_;
};
#endif // !BUBG_CLASSES_BALL_CONTROLED_BALL_H_

