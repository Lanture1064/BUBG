#include "GameScene.h"
#include "Net/Net.h"
#include "GameControler.h"
#include "ChatBox.h"

USING_NS_CC;

GameScene* GameScene::createScene(int state)
{
	auto scene = new GameScene();
	if (scene && scene->init() && scene->initScene(state))
	{
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	return true;
}

bool GameScene::initScene(int state)
{
	state_ = state;
	if (state != USE_SERVER && state != USE_CLIENT)
	{
		return false;
	}
	auto controler = GameControler::createControler(state);
	if (!controler)
	{
		return false;
	}
	this->addChild(controler);
	int id = -1;
	if (state == USE_SERVER)
	{
		id = 0x0000;
	}
	else if (state == USE_CLIENT)
	{
		id = Client::getInstance()->getId();
	}
	auto chat_box = ChatBox::createBox(id,state);
	auto position = Director::getInstance()->getVisibleSize();
	chat_box->setPosition(position.width / 40, position.height / 10);
	this->addChild(chat_box);
	return true;
}





