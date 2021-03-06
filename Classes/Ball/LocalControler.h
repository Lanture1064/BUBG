#ifndef BUBG_CLASSES_BALL_LOCAL_CONTROLER_H_
#define BUBG_CLASSES_BALL_LOCAL_CONTROLER_H_

#include "cocos2d.h"
#include "ControledBall.h"
#include "ControledBallManager.h"
#include <list>
#include <thread>
#include <vector>


class LocalControler : public cocos2d::Layer
{
public:
	virtual bool init();
	void initControler(ControledBallManager* manager, std::list<ControledBall*> *ball_list);
	static LocalControler* createControler(ControledBallManager* manager, std::list<ControledBall*> *ball_list);
	CREATE_FUNC(LocalControler);
	LocalControler() = default;
	~LocalControler();
	void update(float dt);
	//this function is return the time of the divide and reset the count;
	int getDivideCount();
	cocos2d::Vec2 getMousePosition() const;
	cocos2d::Vec2 getManagerPosition() const;
	cocos2d::Vec2 getKeyDirection() const;
	bool isDead() const;
protected:
	cocos2d::Vec2 mouse_position_;
	ControledBallManager* manager_;
	std::list<ControledBall*>* ball_list_;
	int divide_count_;
	std::mutex divide_state_lock_;
	int key_x_direction_;
	int key_y_direction_;
};

#endif // !BUBG_CLASSES_BALL_LOCAL_CONTROLER_H_
