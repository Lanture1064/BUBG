#ifndef BUBG_CLASSES_BALL_CONTROLED_BALL_H_
#define BUBG_CLASSES_BALL_CONTROLED_BALL_H_

#include "BaseBall.h"
#include "FoodBall.h"
#include "VirusBall.h"
#include <string>
#include <list>

class ControledBallManager;

class ControledBall : public BaseBall
{
public:
	CREATE_FUNC(ControledBall);
	ControledBall() = default;
	~ControledBall();
	static ControledBall* createControledBall();
	static ControledBall* createControledBall(int score,std::string color_directory);
	static ControledBall* createControledBall(std::string color_directory);

	virtual bool init();
	void initControledBall(int score,std::string color_directory);
	double getSpeed() const;
	
	void checkSwallowBall(const std::list<FoodBall*> &food_ball_list);
	void checkSwallowBall(const std::list<ControledBall*> &controled_ball_list);
	//this function is including dividing and removing ball operator after swallowing
	int checkSwallowVirus(const std::list<VirusBall*> &virus_ball_list);
	ControledBallManager* getManager() const;
	void setManager(ControledBallManager* manager);
	void divide();
	void updateState();
	bool isDelete() const;
	std::string getColorDirectory() const;
	void changeDividedState();
	bool isDivided() const;

	void countTime();
	int getTimeCount() const;
	void resetTimeCount();
	//the function is used to get the direction of the ball which is divided;
	cocos2d::Vec2 getDivideDirection() const;
	void setDivideDirection(cocos2d::Vec2 direction);

	bool isSwallowVirus() const;
	void changeSwallowVirusState();
	void setDirectionWhenSwallowVirus(cocos2d::Vec2 direction);
	cocos2d::Vec2 getDirectionWhenSwallow() const;
	//this function is used to judge if ball will move beyond window;
	//if true,the speed in this direction will be set by zero;
	template<class T>
	void judgeIfMove(T &x_speed, T &y_speed, cocos2d::Vec2 visible_size);
protected:
	ControledBallManager * manager_;
	double speed_;
	//the list is store the ball will be swallowed;
	std::list<BaseBall*> temp_ball_storage_;
	bool is_delete_;
	std::string color_directory_;
	bool is_divided_;
	bool is_swallow_virus_;
	int time_count_;
	//this is used when move by key;
	cocos2d::Vec2 divide_direction_;
	cocos2d::Vec2 direction_when_swallow_virus_;
};

template<class T>
inline void ControledBall::judgeIfMove(T & x_speed, T & y_speed, cocos2d::Vec2 visible_size)
{
	auto space_position = this->getPosition();
	auto size = this->getBoundingBox().size;
	if (space_position.x <= size.width/2)
	{
		if (x_speed < 0)
		{
			x_speed = 0;
		}
	}
	else if (space_position.x + size.width/2 >= visible_size.x)
	{
		if (x_speed > 0)
		{
			x_speed = 0;
		}
	}
	if (space_position.y <= size.height/2)
	{
		if (y_speed < 0)
		{
			y_speed = 0;
		}
	}
	else if (space_position.y + size.height/2 >= visible_size.y)
	{
		if (y_speed > 0)
		{
			y_speed = 0;
		}
	}
}


#endif // !BUBG_CLASSES_BALL_CONTROLED_BALL_H_