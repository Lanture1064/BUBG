#ifndef BUBG_CLASSES_INPUTFIELD_BOX_H_
#define BUBG_CLASSES_INPUTFIELD_BOX_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"  
#include "SettingHeader.h"

class Inputfield : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuOkCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(Inputfield);
protected:
	cocos2d::extension::EditBox* edit_box_;
};

#endif // __Inputfield_SCENE_H__