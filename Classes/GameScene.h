#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
	static GameScene* createScene(int state);
	virtual bool init();
	bool initScene(int state);

	CREATE_FUNC(GameScene);
protected:
	int state_;
	
};

#endif // __Game_SCENE_H__
