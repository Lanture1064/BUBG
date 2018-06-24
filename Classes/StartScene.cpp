#include "StartScene.h"
#include "WaittingRoom.h"
#include "Inputfieldbox.h"

USING_NS_CC;
using namespace CocosDenshion;

StartScene::~StartScene()
{

}

bool StartScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visible_size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("menu/background_startgame.jpg");
	bg->setPosition(Vec2(origin.x + visible_size.width / 2,
		origin.y + visible_size.height / 2));
	this->addChild(bg);

	MenuItemImage *creategameMenuItem = MenuItemImage::create(
		"menu/creategame01.png",
		"menu/creategame02.png",
		CC_CALLBACK_1(StartScene::createGame, this));

	creategameMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(640, 400)));

	MenuItemImage *joingameMenuItem = MenuItemImage::create(
		"menu/joingame01.png",
		"menu/joingame02.png",
		CC_CALLBACK_1(StartScene::joinGame, this));

	joingameMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(640, 550)));

	Menu* mu = Menu::create(creategameMenuItem, joingameMenuItem, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu);

	return true;
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
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}
}

void StartScene::createGame(cocos2d::Object * pSender)
{
	auto scene = WaittingRoom::createRoom(USE_SERVER);
	Director::getInstance()->replaceScene(scene);
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}
}
