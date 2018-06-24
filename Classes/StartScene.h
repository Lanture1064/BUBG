#ifndef BUBG_CLASSES_START_SCENE_H_
#define BUBG_CLASSES_START_SCENE_H_

#include "cocos2d.h"
#include "SettingHeader.h"

class StartScene : public cocos2d::Scene
{
public:
	StartScene() = default;
	~StartScene();
	virtual bool init();
	CREATE_FUNC(StartScene);
	static StartScene* createScene();
	void joinGame(cocos2d::Object* pSender);
	void createGame(cocos2d::Object* pSender);
};


#endif // !BUBG_CLASSES_START_SCENE_H_


