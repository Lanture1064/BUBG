#ifndef BUBG_CLASSES_BALLBASE_BALL_H_
#define BUBG_CLASSES_BALLBASE_BALL_H_

#include"cocos2d.h"

class BaseBall : public cocos2d::Sprite
{
public:
	int getScore() const;
	double getSize() const;
	virtual bool init();
	CREATE_FUNC(BaseBall);
protected:
	int score_;
	double size_;
};
#endif // !BUBG_CLASSES_BALLBASE_BALL_H_

