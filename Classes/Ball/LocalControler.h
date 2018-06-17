#ifndef BUBG_CLASSES_BALL_LOCAL_CONTROLER_H_
#define BUBG_CLASSES_BALL_LOCAL_CONTROLER_H_

#include "cocos2d.h"
#include <list>
#include <thread>

class ControledBallManager;
class ControledBall;

class LocalControler : public cocos2d::Layer
{
public:
	bool isDivide() const;
	void changeDivideState();
	virtual bool init();
	void initControler(ControledBallManager* manager, std::list<ControledBall*> *ball_list);
	static LocalControler* createControler(ControledBallManager* manager, std::list<ControledBall*> *ball_list);
	CREATE_FUNC(LocalControler);
	LocalControler() = default;
	~LocalControler();
	void update(float dt);
protected:
	cocos2d::Vec2 mouse_position_;
	ControledBallManager* manager_;
	std::list<ControledBall*>* ball_list_;
	bool is_divide_;
	std::mutex lock_;
};

#endif // !BUBG_CLASSES_BALL_LOCAL_CONTROLER_H_
