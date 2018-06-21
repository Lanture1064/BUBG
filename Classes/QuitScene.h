#ifndef __QUIT_SCENE_H__
#define __QUIT_SCENE_H__

#include "cocos2d.h"
#include"SettingScene1.h"
#include "SettingHeader.h"

class QuitScene : public cocos2d::Layer
{
private:
	bool isEffect;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	//菜单项回调函数
	void menuSettingCallback(cocos2d::Ref* pSender);
	void menuQuitCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(QuitScene);
};

#endif // __QUIT_SCENE_H__
