#ifndef BUBG_CLASSES_TEST_TEST_H_
#define BUBG_CLASSES_TEST_TEST_H_

#include "cocos2d.h"
#include"SettingScene1.h"

class TestScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(TestScene);
	virtual bool init();
	static TestScene* createScene();
	void initScene();
	void serverButton(cocos2d::Object* pSender);
	void clientButton(cocos2d::Object* pSender);
};
#endif // !BUBG_CLASSES_TEST_TEST_H_
