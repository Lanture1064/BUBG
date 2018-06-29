#ifndef BUBG_CLASSES_SETTING_SCENE_H_
#define BUBG_CLASSES_SETTING_SCENE_H_
#include "cocos2d.h"
#include "SettingHeader.h"

class Setting : public cocos2d::Layer
{
private:
	bool isEffect;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();
	//菜单项回调函数
	void menuSoundToggleCallback(cocos2d::Ref* pSender);
	void menuMusicToggleCallback(cocos2d::Ref* pSender);
	void menuControlToggleCallback(cocos2d::Ref* pSender);
	void menuOkCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(Setting);
};

#endif // __Setting_SCENE_H__

