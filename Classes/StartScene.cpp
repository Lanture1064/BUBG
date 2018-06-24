#include "StartScene.h"
#include "WaittingRoom.h"
#include "Inputfieldbox.h"

USING_NS_CC;

StartScene::~StartScene()
{

}

bool StartScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
}

StartScene * StartScene::createScene()
{
	auto scene = new StartScene();
	if (scene && scene->init())
	{
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

void StartScene::joinGame(cocos2d::Object * pSender)
{
	auto scene = Inputfield::createScene();
	Director::getInstance()->replaceScene(scene);
}

void StartScene::createGame(cocos2d::Object * pSender)
{
	auto scene = WaittingRoom::createRoom(USE_SERVER);
	Director::getInstance()->replaceScene(scene);
}
