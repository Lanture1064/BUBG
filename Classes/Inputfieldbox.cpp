#include "Inputfieldbox.h"
#include "extensions\cocos-ext.h"  
#include "Net/Net.h"
#include "WaittingRoom.h"
#include <exception>
USING_NS_CC_EXT;
USING_NS_CC;

using namespace CocosDenshion;

const int g_kErrorLabelFlag = 1;

Scene* Inputfield::createScene()
{
	auto scene = Scene::create();
	auto layer = Inputfield::create();
	scene->addChild(layer);

	return scene;
}

bool Inputfield::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visible_size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("menu/background_box.jpg");

	bg->setPosition(Vec2(origin.x + visible_size.width / 2,
		origin.y + visible_size.height / 2));
	this->addChild(bg);

	auto okMenuItem = MenuItemImage::create(
		"menu/ok-down.png",
		"menu/ok-up.png",
		CC_CALLBACK_1(Inputfield::menuOkCallback, this));

	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(640, 560)));

	auto inputBox = Sprite::create("inputfield.jpg");
	
	auto visibble_size = Director::getInstance()->getVisibleSize();
	auto tip_label = Label::create("Please enter the ip address of the server", "Arial", 35);
	tip_label->setPosition(visible_size.width / 2, visibble_size.height / 7 * 4);
	tip_label->setColor(Color3B(255, 140, 0));
	this->addChild(tip_label);

	auto error_label = Label::create("", "Arial", 30);
	error_label->setPosition(visibble_size.width / 2, visibble_size.height / 3);
	error_label->setColor(Color3B(255, 140, 0));
	this->addChild(error_label);
	error_label->setTag(g_kErrorLabelFlag);

	edit_box_ = EditBox::create(Size(inputBox->getContentSize().width, inputBox->getContentSize().height), Scale9Sprite::create("inputfield.png"));
	edit_box_->setPosition(Director::getInstance()->convertToGL(Vec2(640, 400)));
	edit_box_->setMaxLength(15);
	edit_box_->setText("127.0.0.1"); 
	edit_box_->setFontColor(Color3B(255, 255, 255)); 
	edit_box_->setFontSize(27);   
	bg->addChild(edit_box_);

	Menu* mn = Menu::create(okMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	return true;
}

void Inputfield::menuOkCallback(Ref* pSender)
{
	auto ip = edit_box_->getText();
	try
	{
		boost::asio::ip::tcp::resolver::query query(ip);
		Client::getInstance()->setServerIp(ip);
		auto sc = WaittingRoom::createRoom(USE_CLIENT);
		//change Scene from inputfield scene to waittingroom
		Director::getInstance()->replaceScene(sc);
	} 
	catch (std::exception& e)
	{
		auto error = static_cast<Label*>(this->getChildByTag(g_kErrorLabelFlag));
		error->setString("The ip address is illegal");
	}
	
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}
}