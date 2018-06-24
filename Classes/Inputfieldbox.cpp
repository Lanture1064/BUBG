#include "Inputfieldbox.h"
#include "Test/Test.h"
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
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Inputfield::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Inputfield::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visible_size = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *bg = Sprite::create("menu/background_box.jpg");

	// position the label on the center of the screen
	bg->setPosition(Vec2(origin.x + visible_size.width / 2,
		origin.y + visible_size.height / 2));
	this->addChild(bg);

	//Ok按钮
	auto okMenuItem = MenuItemImage::create(
		"menu/ok-down.png",
		"menu/ok-up.png",
		CC_CALLBACK_1(Inputfield::menuOkCallback, this));

	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(640, 560)));

	//输入框
	//输入框背景  
	auto inputBox = Sprite::create("inputfield.jpg");
	
	auto visibble_size = Director::getInstance()->getVisibleSize();
	auto tip_label = Label::create("Please enter the ip address of the server", "Arial", 30);
	tip_label->setPosition(visible_size.width / 2, visibble_size.height / 7 * 4);
	tip_label->setColor(Color3B::BLACK);
	this->addChild(tip_label);

	auto error_label = Label::create("", "Arial", 30);
	error_label->setPosition(visibble_size.width / 2, visibble_size.height / 3);
	error_label->setColor(Color3B::BLACK);
	this->addChild(error_label);
	error_label->setTag(g_kErrorLabelFlag);

    //输入框设置
	edit_box_ = EditBox::create(Size(inputBox->getContentSize().width, inputBox->getContentSize().height), Scale9Sprite::create("inputfield.png"));
	//输入框的位置
	edit_box_->setPosition(Director::getInstance()->convertToGL(Vec2(640, 400)));
	//输入框所接受字符的最大数 
	edit_box_->setMaxLength(15);
	//初始化文字 
	edit_box_->setText("127.0.0.1");
	//文字颜色  
	edit_box_->setFontColor(Color3B(255, 255, 255)); 
	//设置文字的大小  
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