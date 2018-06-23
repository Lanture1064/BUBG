#ifndef BUBG_CLASSES_HELLOWORLD_SCENE_H_
#define BUBG_CLASSES_HELLOWORLD_SCENE_H_

#include "cocos2d.h"
#include "SettingScene1.h"
#include "GameScene.h"
#include "SettingHeader.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();
    
	//the menu button
	void getGameScene(cocos2d::Object *pSender);
	void getSettingScene(cocos2d::Object *pSender);
	void quitGame(cocos2d::Object *pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
