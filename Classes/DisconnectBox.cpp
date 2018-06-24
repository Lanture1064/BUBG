#include "DisconnectBox.h"
#include "Net/Net.h"
#include "HelloWorldScene.h"
USING_NS_CC;

DisconnectBox::~DisconnectBox()
{

}

DisconnectBox * DisconnectBox::createBox()
{
	auto box = new DisconnectBox();
	if (box && box->init())
	{
		box->autorelease();
		return box;
	}
	CC_SAFE_DELETE(box);
	return nullptr;
}

bool DisconnectBox::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visible_size = Director::getInstance()->getVisibleSize();
	auto label = Label::create("Disconnect from server", "Arial", visible_size.height / 18);
	label->setColor(Color3B::BLACK);
	this->addChild(label);
	label->setPosition(0, visible_size.height / 5);
	auto button = MenuItemImage::create("menu/ok-up.png", "menu/ok-down.png", CC_CALLBACK_1(DisconnectBox::quitGame, this));
	auto menu = Menu::create(button, NULL);
	this->addChild(menu);
	menu->setPosition(0, 0);
	return true;
}

void DisconnectBox::quitGame(cocos2d::Object * pSender)
{
	Client::getInstance()->clear();
	auto scene = HelloWorld::create();
	Director::getInstance()->replaceScene(scene);
}
