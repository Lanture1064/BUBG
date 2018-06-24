#ifndef BUBG_CLASSES_INPUTFIELD_BOX_H_
#define BUBG_CLASSES_INPUTFIELD_BOX_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"  
#include "SettingHeader.h"

class Inputfield : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	void menuOkCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Inputfield);
protected:
	cocos2d::extension::EditBox* edit_box_;
};

#endif // __Inputfield_SCENE_H__
