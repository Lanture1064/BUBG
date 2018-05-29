#ifndef BUBG_CLASSES_BALL_CONTROLED_BALL_H_
#define BUBG_CLASSES_BALL_CONTROLED_BALL_H_

#include"BaseBall.h"
#include<list>

class FoodBall;
class ControledBallManager;

//the funtion return the square of the two Vec2;
double calDistence(const cocos2d::Vec2 &i,const cocos2d::Vec2 &j);

class ControledBall : public BaseBall
{
public:
	CREATE_FUNC(ControledBall);
	ControledBall() = default;
	~ControledBall();
	static ControledBall* createControledBall();
	virtual bool init();
	double getSpeed() const;
	//the function return the list of the food which would be swallowed
	//but it does not operate on the food;
	std::list<FoodBall*> checkSwallowBall(const std::list<FoodBall*> &food_ball_list);
	//the function return the list of the controled ball which would be swallowed
	//but it does not operate on these ball;
	std::list<ControledBall*> checkoutSwallowBall(const std::list<ControledBall*> &controled_ball_list);
	ControledBallManager* getManager() const;
	void setManager(ControledBallManager* manager);
	void updateState();
protected:
	ControledBallManager * manager_;
	double speed_;
	//the list is store the ball will be swallowed;
	std::list<BaseBall*> temp_ball_storage_;
};
#endif // !BUBG_CLASSES_BALL_CONTROLED_BALL_H_

