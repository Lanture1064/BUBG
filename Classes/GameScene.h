#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static GameScene* createScene(int state);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	bool initScene(int state);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
protected:
	int state_;
	
};

#endif // __Game_SCENE_H__
