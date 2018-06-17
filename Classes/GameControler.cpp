#include "GameControler.h"
#include "Ball/Ball.h"
#include "Net/Net.h"
#include "Tool/MathTool.h"
#include <thread>

USING_NS_CC;

const int g_kFoodManagerFlag = 1;
const int g_kFoodLayerFlag = 2;
const int g_kControledBallLayerFlag = 3;
const int g_kBackgroundFlag = 4;

GameControler::~GameControler()
{

}

bool GameControler::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

bool GameControler::initControler(bool state)
{
	state_ = state;
	this->setAnchorPoint(Vec2::ZERO);

	auto background = Sprite::create("menu/background_game.png");
	background->setTag(g_kBackgroundFlag);
	background->setAnchorPoint(Vec2::ZERO);
	this->addChild(background);
	background->setScale(Director::getInstance()->getVisibleSize().width * 3 / background->getContentSize().width);

	auto food_manager = FoodBallManager::createManager();
	this->addChild(food_manager);
	food_manager->setTag(g_kFoodManagerFlag);

	auto food_layer = Layer::create();
	food_layer->setAnchorPoint(Vec2::ZERO);
	food_layer->setTag(g_kFoodLayerFlag);
	this->addChild(food_layer);

	auto controled_ball_layer = Layer::create();
	controled_ball_layer->setAnchorPoint(Vec2::ZERO);
	controled_ball_layer->setTag(g_kControledBallLayerFlag);
	this->addChild(controled_ball_layer);

	if (state == USE_SERVER)
	{
		this->initWithServer();
	}
	else if (state == USE_CLIENT)
	{
		this->initWithClient();
	}
	else
	{
		return false;
	}
	this->scheduleUpdate();
	return true;
}

void GameControler::initWithServer()
{
	Server::getInstance()->startGame();
	std::thread get_command_thread(&Server::getCommand, Server::getInstance());
	std::thread replay_command_thread(&Server::replayCommand, Server::getInstance());
	get_command_thread.detach();
	replay_command_thread.detach();

	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto controled_ball_layer = static_cast<Layer*>(this->getChildByTag(g_kControledBallLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto background = static_cast<Sprite*> (this->getChildByTag(g_kBackgroundFlag));

	//init the food layer;
	auto food_number = food_manager->getSize();
	CommandImformation command;
	auto background_size = background->getBoundingBox().size;
	command.command = NEW_FOOD;
	for (auto i = 0; i < food_number; ++i)
	{
		auto food = food_manager->getNewFoodBall();
		if (food)
		{
			food_list_.push_back(food);
			food_layer->addChild(food);
			auto x = getDoubleRand(background_size.width);
			auto y = getDoubleRand(background_size.height);
			food->setPosition(Vec2(x, y));
			command.x = x;
			command.y = y;
			Server::getInstance()->addNetCommand(command);
		}
	}

	//init the controled ball layer;
	command.command = NEW_MANAGER;
	{
		command.id = 0x0000;
		auto x = getDoubleRand(background_size.width);
		auto y = getDoubleRand(background_size.height);
		auto manager = ControledBallManager::createManager(&controled_ball_list_,Vec2(x,y));
		manager->setId(command.id);
		manager->addFatherScene(controled_ball_layer);
		manager_container_.push_back(manager);
		local_controler_ = LocalControler::createControler(manager, &controled_ball_list_);
		this->addChild(local_controler_);
		for (auto player = Server::getInstance()->getPlayer().begin(); player != Server::getInstance()->getPlayer().end(); ++player)
		{
			Server::getInstance()->addNetCommand(command);
		}
	}
	net_controler_ = NetControler::createControler();
	this->addChild(net_controler_);
	for (auto player = Server::getInstance()->getPlayer().begin(); player != Server::getInstance()->getPlayer().end(); ++player)
	{
		command.id = player->id;
		auto x = getDoubleRand(background_size.width);
		auto y = getDoubleRand(background_size.height);
		auto manager = ControledBallManager::createManager(&controled_ball_list_, Vec2(x, y));
		manager->setId(command.id);
		manager->addFatherScene(controled_ball_layer);
		manager_container_.push_back(manager);
		net_controler_->addManager(manager);
		for (auto player = Server::getInstance()->getPlayer().begin(); player != Server::getInstance()->getPlayer().end(); ++player)
		{
			Server::getInstance()->addNetCommand(command);
		}
	}

	//finish init;
	command.command = INIT_END;
	command.id = -1;
	command.x = 0;
	command.y = 0;
	for (auto player = Server::getInstance()->getPlayer().begin(); player != Server::getInstance()->getPlayer().end(); ++player)
	{
		Server::getInstance()->addNetCommand(command);
	}

	//provide time for client to init;
	Sleep(50);
}

void GameControler::initWithClient()
{
	Server::getInstance()->startGame();
	std::thread get_command_thread(&Server::getCommand, Server::getInstance());
	std::thread replay_command_thread(&Server::replayCommand, Server::getInstance());
	get_command_thread.detach();
	replay_command_thread.detach();

	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto controled_ball_layer = static_cast<Layer*>(this->getChildByTag(g_kControledBallLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto background = static_cast<Sprite*> (this->getChildByTag(g_kBackgroundFlag));

	net_controler_ = NetControler::createControler();
	this->addChild(net_controler_);
	for (;;)
	{
		auto command_vec = Client::getInstance()->getLocalCommand();
		for (auto command = command_vec.begin(); command != command_vec.end(); ++command)
		{
			switch (command->command)
			{
			case NEW_FOOD:
				auto food = food_manager->getNewFoodBall();
				food_list_.push_back(food);
				food_layer->addChild(food);
				food->setPosition(command->x, command->y);
				break;
			case NEW_MANAGER:
				auto manager = ControledBallManager::createManager(&controled_ball_list_, Vec2(command->x, command->y));
				manager->setId(command->id);
				manager->addFatherScene(controled_ball_layer);
				manager_container_.push_back(manager);
				if (manager->getId() == Client::getInstance()->getId())
				{
					local_controler_ = LocalControler::createControler(manager, &controled_ball_list_);
					this->addChild(local_controler_);
				}
				else
				{
					net_controler_->addManager(manager);
				}
				break;
			case INIT_END:
				return;
				break;
			default:
				break;
			}
		}
	}
}

GameControler * GameControler::createControler(bool state)
{
	auto controler = new GameControler();
	if (controler && controler->init() && controler->initControler(state))
	{
		controler->autorelease();
		return controler;
	}
	CC_SAFE_DELETE(controler);
	return nullptr;
}


void GameControler::update(float dt)
{
	if (state_ == USE_SERVER)
	{
		this->updateWithServer();
	}
	else if (state_ == USE_CLIENT)
	{
		this->updateWithClient();
	}
}
