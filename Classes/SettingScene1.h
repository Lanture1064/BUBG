#ifndef BUBG_CLASSES_SETTING_SCENE_H_
#define BUBG_CLASSES_SETTING_SCENE_H_
#include "cocos2d.h"
#include "SettingHeader.h"

class Setting : public cocos2d::Layer
{
private:
	bool isEffect;
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();
	//�˵���ص�����
	void menuSoundToggleCallback(cocos2d::Ref* pSender);
	void menuMusicToggleCallback(cocos2d::Ref* pSender);
	void menuControlToggleCallback(cocos2d::Ref* pSender);
	void menuOkCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Setting);
};

#endif // __Setting_SCENE_H__

