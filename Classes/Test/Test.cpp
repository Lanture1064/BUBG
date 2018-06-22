#include "Test.h"
#include "../GameControler.h"
#include "../Net/Net.h"
#include "ChatBox.h"
#include <thread>
USING_NS_CC;

bool TestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	return true;
}

TestScene * TestScene::createScene()
{
	auto scene = new TestScene();
	if (scene && scene->init())
	{
		scene->initScene();
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

void TestScene::initScene()
{

	auto server_label = Label::create("Server", "Arial", 20);
	auto pServer_label = MenuItemLabel::create(server_label, this, menu_selector(TestScene::serverButton));
	auto server_button = Menu::create(pServer_label, NULL);

	auto client_label = Label::create("Client", "Arial", 20);
	auto pClient_label = MenuItemLabel::create(client_label, this, menu_selector(TestScene::clientButton));
	auto client_button = Menu::create(pClient_label, NULL);
	//this address is the ip of ZYD'laptop;
	Client::getInstance()->setServerIp("192.168.3.5");
	server_button->setPosition(Vec2(100, 100));
	client_button->setPosition(Vec2(200, 200));
	this->addChild(server_button);
	this->addChild(client_button);
}

void TestScene::serverButton(cocos2d::Object * pSender)
{
	Server::getInstance()->beginWait();
	std::thread t(&Server::connect, Server::getInstance());
	t.detach();
	for (;;)
	{
		if (Server::getInstance()->getPlayer().size()<1)
		{
			Server::getInstance()->endWait();
			Server::getInstance()->startGame();
			break;
		}
	}
	auto controler = GameControler::createControler(USE_SERVER);
	this->addChild(controler);
	auto chat_box = ChatBox::createBox(0x0000);
	chat_box->setPosition(500, 100);
	this->addChild(chat_box);
}

void TestScene::clientButton(cocos2d::Object * pSender)
{
	Client::getInstance()->connect();
	Client::getInstance()->startGame();
	auto controler = GameControler::createControler(USE_CLIENT);
	this->addChild(controler);
}

