#include "WaittingRoom.h"
#include "GameScene.h"
#include "HelloWorldScene.h"
#include <thread>
#include <string>
USING_NS_CC;
using namespace CocosDenshion;

WaittingRoom * WaittingRoom::createRoom(int state)
{
	auto room = new WaittingRoom();
	if (room && room->init())
	{
		room->initRoom(state);
		room->autorelease();
		return room;
	}
	CC_SAFE_DELETE(room);
	return nullptr;
}

bool WaittingRoom::init()
{
	if (!Scene::init())
	{
		return false;
	}
	return true;
}

void WaittingRoom::initRoom(int state)
{
	state_ = state;

	auto background = Sprite::create("menu/background_game.PNG");
	auto position = Director::getInstance()->getVisibleSize();
	background->setPosition(position.width / 2, position.height / 2);
	this->addChild(background);
	message_label_ = Label::create("", "Arial", position.height / 15);
	message_label_->setPosition(position.width / 2, position.height / 3 * 2);
	message_label_->setColor(Color3B(255, 140, 0));
	this->addChild(message_label_);
	if (state == USE_SERVER)
	{
		Server::getInstance()->beginWait();
		std::thread t(&Server::connect, Server::getInstance());
		t.detach();
		auto start_button = MenuItemImage::create("menu/start01.png", "menu/start02.png", CC_CALLBACK_1(WaittingRoom::startGame, this));
		start_button->setPosition(position.width / 2, position.height / 2);
		auto quit_button = MenuItemImage::create("menu/back01.png", "menu/back02.png", CC_CALLBACK_1(WaittingRoom::returnStartScene, this));
		quit_button->setPosition(position.width / 2, position.height / 4);

		auto menu = Menu::create(start_button, quit_button, NULL);
		this->addChild(menu);
		menu->setPosition(Vec2::ZERO);
	}
	else if (state == USE_CLIENT)
	{
		if (Client::getInstance()->connect())
		{
			message_label_->setString("Connect successfully and wait starting");
		}
		else
		{
			message_label_->setString("Connect fail");
			auto quit_button = MenuItemImage::create("menu/back01.png", "menu/back02.png", CC_CALLBACK_1(WaittingRoom::returnStartScene, this));
			auto menu = Menu::create(quit_button, NULL);
			this->addChild(menu);
			menu->setPosition(position.width / 2, position.height / 2);
		}
	}
	this->scheduleUpdate();

}

void WaittingRoom::startGame(Object * pSender)
{
	Server::getInstance()->endWait();
	Server::getInstance()->startGame();
	auto scene = GameScene::createScene(USE_SERVER);
	if (scene)
	{
		Director::getInstance()->replaceScene(scene);
	}
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}
}

void WaittingRoom::returnStartScene(Object * pSender)
{
	Server::getInstance()->clear();
	Client::getInstance()->clear();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}
	auto scene = HelloWorld::createScene();
	if (scene)
	{
		Director::getInstance()->replaceScene(scene);
	}
}

void WaittingRoom::update(float dt)
{
	static const std::string text = std::string("Player Number: ");
	if (state_ == USE_SERVER)
	{
		int count = 0;
		std::string number = "01";
		count = Server::getInstance()->getPlayer().size();
		number[0] += count / 10;
		number[1] += count % 10;
		message_label_->setString(text + number);
	}
	else if (state_ == USE_CLIENT)
	{
		if (Client::getInstance()->isStart())
		{
			auto scene = GameScene::createScene(state_);
			if (scene)
			{
				Director::getInstance()->replaceScene(scene);
			}
			else
			{
				Client::getInstance()->clear();
				auto position = Director::getInstance()->getVisibleSize();
				message_label_->setString("Server close the socket");
				auto quit_button = MenuItemImage::create("menu/back01.png", "menu/back02.png", CC_CALLBACK_1(WaittingRoom::returnStartScene, this));
				auto menu = Menu::create(quit_button, NULL);
				this->addChild(menu);
				menu->setPosition(position.width / 2, position.height / 2);
			}
		}
	}
}


WaittingRoom::~WaittingRoom()
{}
