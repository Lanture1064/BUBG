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

bool GameControler::initControler(int state)
{
	state_ = state;
	this->setAnchorPoint(Vec2::ZERO);

	auto background = Sprite::create("menu/background_game.png");
	background->setTag(g_kBackgroundFlag);
	background->setAnchorPoint(Vec2::ZERO);
	this->addChild(background);
	//background->setScale(Director::getInstance()->getVisibleSize().width * 3 / background->getContentSize().width);

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
		command.x = x;
		command.y = y;
		auto manager = ControledBallManager::createManager(&controled_ball_list_,Vec2(x,y));
		manager->setId(command.id);
		manager->addFatherScene(controled_ball_layer);
		manager_container_.push_back(manager);
		local_controler_ = LocalControler::createControler(manager, &controled_ball_list_);
		this->addChild(local_controler_);
		Server::getInstance()->addNetCommand(command);
	}
	net_controler_ = NetControler::createControler();
	this->addChild(net_controler_);
	for (auto player = Server::getInstance()->getPlayer().begin(); player != Server::getInstance()->getPlayer().end(); ++player)
	{
		command.id = player->id;
		auto x = getDoubleRand(background_size.width);
		auto y = getDoubleRand(background_size.height);
		command.x = x;
		command.y = y;
		auto manager = ControledBallManager::createManager(&controled_ball_list_, Vec2(x, y));
		manager->setId(command.id);
		manager->addFatherScene(controled_ball_layer);
		manager_container_.push_back(manager);
		net_controler_->addManager(manager);
		Server::getInstance()->addNetCommand(command);
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
	Client::getInstance()->startGame();
	std::thread get_command_thread(&Client::getCommand, Client::getInstance());
	std::thread replay_command_thread(&Client::replayCommand, Client::getInstance());
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
			FoodBall* food = nullptr;
			ControledBallManager* manager = nullptr;

			switch (command->command)
			{
			case NEW_FOOD:
				food = food_manager->getNewFoodBall();
				if (food)
				{
					food_list_.push_back(food);
					food_layer->addChild(food);
					food->setPosition(command->x, command->y);
				}
				break;
			case NEW_MANAGER:
				manager = ControledBallManager::createManager(&controled_ball_list_, Vec2(command->x, command->y));
				if (manager)
				{
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

GameControler * GameControler::createControler(int state)
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
	/*auto manager_position = local_controler_->getManagerPosition();
	auto this_position = this->getPosition();
	cocos2d::Vec2 position, temp_position;
	temp_position.x = manager_position.x + this_position.x;
	temp_position.y = manager_position.y + this_position.y;
	auto visible_size = Director::getInstance()->getVisibleSize();
	position.x = this_position.x + (visible_size.width / 2 - temp_position.x);
	position.y = this_position.y + (visible_size.height / 2 - temp_position.y);
	this->setPosition(position);*/
}

void GameControler::updateWithServer()
{
	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto background = static_cast<Sprite*> (this->getChildByTag(g_kBackgroundFlag));

	CommandImformation command;
	auto background_size = background->getBoundingBox().size;
	command.command = NEW_FOOD;

	auto local_command = Server::getInstance()->getLocalCommand();
	for (auto i = local_command.begin(); i != local_command.end(); ++i)
	{
		net_controler_->addCommand(*i);
	}

	for (auto manager = manager_container_.begin(); manager != manager_container_.end(); ++manager)
	{
		auto temp = (*manager)->swallow(food_list_, controled_ball_list_);
		for (auto i = 0; i < temp.first; ++i)
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
	}
	int time = 0;
	if (time=local_controler_->getDivideCount())
	{
		command.command = DIVIDE;
		command.id = 0x0000;
		Server::getInstance()->addNetCommand(command);
	}
	command.command = DIRECTION;
	command.id = 0x0000;
	auto position = local_controler_->getMousePosition();
	command.x = position.x;
	command.y = position.y;
	Server::getInstance()->addNetCommand(command);

}

void GameControler::updateWithClient()
{
	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));

	for (auto manager = manager_container_.begin(); manager != manager_container_.end(); ++manager)
	{
		(*manager)->swallow(food_list_, controled_ball_list_);
	}
	auto local_command = Client::getInstance()->getLocalCommand();
	FoodBall* new_food = nullptr;
	for (auto i = local_command.begin(); i != local_command.end(); ++i)
	{
		switch (i->command)
		{
		case NEW_FOOD:
			new_food = food_manager->getNewFoodBall();
			if (new_food)
			{
				food_layer->addChild(new_food);
				new_food->setPosition(Vec2(i->x, i->y));
				food_list_.push_back(new_food);
			}
			break;
		case DIRECTION: case DIVIDE:  default:
			net_controler_->addCommand(*i);
			break;
		}
	}
	CommandImformation command;
	int time = 0;
	if (time = local_controler_->getDivideCount())
	{
		command.command = DIVIDE;
		command.id = Client::getInstance()->getId();
		Client::getInstance()->addNetCommand(command);
	}

	command.command = DIRECTION;
	command.id = Client::getInstance()->getId();
	auto position = local_controler_->getMousePosition();
	command.x = position.x;
	command.y = position.y;
	Client::getInstance()->addNetCommand(command);
}
