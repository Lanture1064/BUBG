#include "QuitScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* QuitScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = QuitScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool QuitScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	log("Setting init");


	Size visible_size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("menu/background_box.jpg");

	// position the label on the center of the screen
	bg->setPosition(Vec2(origin.x + visible_size.width / 2,
		origin.y + visible_size.height / 2));
	this->addChild(bg);


	//setting°´Å¥
	auto settingMenuItem = MenuItemImage::create(
		"menu/setting01.png",
		"menu/setting02.png",
		CC_CALLBACK_1(QuitScene::menuSettingCallback, this));

	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(600, 350)));
	
	//quit°´Å¥
	auto quitMenuItem = MenuItemImage::create(
		"menu/quit01.png",
		"menu/quie02.png",
		CC_CALLBACK_1(QuitScene::menuQuitCallback, this));

	quitMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(600, 475)));

	//back°´Å¥
	auto backMenuItem = MenuItemImage::create(
		"menu/back01.png",
		"menu/back02.png",
		CC_CALLBACK_1(QuitScene::menuBackCallback, this));

	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(600, 600)));

	Menu* mn = Menu::create(settingMenuItem, quitMenuItem,backMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	return true;
}

void QuitScene::menuSettingCallback(Ref* pSender)
{
	
}

void QuitScene::menuQuitCallback(Ref* pSender)
{


}


void QuitScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}
}