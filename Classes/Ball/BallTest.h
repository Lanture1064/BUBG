#ifndef BUBG_CLASSES_BALL_BALL_TEXT_H_
#define BUBG_CLASSES_BALL_BALL_TEXT_H_

#include "cocos2d.h"
#include "FoodBallManager.h"

class BallTestScene : public cocos2d::Scene
{
public:
	BallTestScene() = default;
	~BallTestScene();
	virtual bool init();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(BallTestScene);
	void createOneFood(cocos2d::Object* pSender);
	void createTenFood(cocos2d::Object* pSender);
	void returnMenu(cocos2d::Object* pSender);
};
double getDoubleRand(unsigned int range);
#endif // !BUBG_CLASSES_BALL_BALL_TEXT_H_
