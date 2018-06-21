#include "GameControler.h"
#include "Net/Net.h"
#include "Tool/MathTool.h"
#include <thread>
#include"QuitScene.h"

USING_NS_CC;

using namespace CocosDenshion;

//it's a suited proportion to eliminate the blank section of the background;
#define BACKGROUND_WIDTH_PROPORTION 1

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
	background->setScale(Director::getInstance()->getVisibleSize().width * 1.5 / background->getContentSize().width);

	auto food_manager = FoodBallManager::createManager(100);
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

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event * event)
	{
		keys[keyCode] = true;
	};
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event * event)
	{
		keys[keyCode] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
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
	background_size.width *= BACKGROUND_WIDTH_PROPORTION;
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
		auto manager = ControledBallManager::createManager(&controled_ball_list_,Vec2(background_size.width, background_size.height),Vec2(x,y));
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
		auto manager = ControledBallManager::createManager(&controled_ball_list_, Vec2(background_size.width, background_size.height),Vec2(x, y));
		manager->setId(command.id);
		manager->addFatherScene(controled_ball_layer);
		manager_container_.push_back(manager);
		net_controler_->addManager(manager);
		Server::getInstance()->addNetCommand(command);
	}

	command.command = NEW_VIRUS;
	for (auto i = 0; i < 5; ++i)
	{
		auto virus = VirusBall::createBall();
		if (virus)
		{
			virus_list_.push_back(virus);
			controled_ball_layer->addChild(virus, virus->getScore());
			auto x = getDoubleRand(background_size.width - virus->getSize()) + virus->getSize() / 2;
			auto y = getDoubleRand(background_size.height - virus->getSize()) + virus->getSize() / 2;
			virus->setPosition(Vec2(x, y));
			command.x = x;
			command.y = y;
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
	Client::getInstance()->startGame();
	std::thread get_command_thread(&Client::getCommand, Client::getInstance());
	std::thread replay_command_thread(&Client::replayCommand, Client::getInstance());
	get_command_thread.detach();
	replay_command_thread.detach();

	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto controled_ball_layer = static_cast<Layer*>(this->getChildByTag(g_kControledBallLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto background = static_cast<Sprite*> (this->getChildByTag(g_kBackgroundFlag));
	auto background_size = background->getBoundingBox().size;
	net_controler_ = NetControler::createControler();
	this->addChild(net_controler_);
	for (;;)
	{
		auto command_vec = Client::getInstance()->getLocalCommand();
		for (auto command = command_vec.begin(); command != command_vec.end(); ++command)
		{
			{
				FoodBall* food = nullptr;
				ControledBallManager* manager = nullptr;
				VirusBall* virus = nullptr;

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
					manager = ControledBallManager::createManager(&controled_ball_list_, Vec2(background_size.width, background_size.height),
																  Vec2(command->x, command->y));
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
				case NEW_VIRUS:
					virus = VirusBall::createBall();
					if (virus)
					{
						controled_ball_layer->addChild(virus, virus->getScore());
						virus_list_.push_back(virus);
						virus->setPosition(Vec2(command->x, command->y));
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

	//these codes are used to move the map with the player;
	auto background = static_cast<Sprite*> (this->getChildByTag(g_kBackgroundFlag));
	auto background_size = background->getBoundingBox().size;
	background_size.width *= BACKGROUND_WIDTH_PROPORTION;
	auto visible_size = Director::getInstance()->getVisibleSize();

	auto manager_position = local_controler_->getManagerPosition();
	auto this_position = this->getPosition();
	cocos2d::Vec2 position, temp_position;
	temp_position.x = manager_position.x + this_position.x;
	temp_position.y = manager_position.y + this_position.y;
	position.x = this_position.x + (visible_size.width / 2 - temp_position.x);
	position.y = this_position.y + (visible_size.height / 2 - temp_position.y);

	//border judge;
	if (position.x > 0)
	{
		position.x = 0;
	}
	else if (position.x + background_size.width < visible_size.width)
	{
		position.x = visible_size.width - background_size.width;

	}
	if (position.y > 0)
	{
		position.y = 0;
	}
	else if (position.y + background_size.height < visible_size.height)
	{
		position.y = visible_size.height - background_size.height;
	}
	this->setPosition(position);

	Node::update(dt);
	auto q = EventKeyboard::KeyCode::KEY_Q;
	if (isKeyPressed(q)) {
		keyPressedDuration(q);
	}
}

void GameControler::updateWithServer()
{
	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto background = static_cast<Sprite*> (this->getChildByTag(g_kBackgroundFlag));
	auto controled_ball_layer = static_cast<Layer*>(this->getChildByTag(g_kControledBallLayerFlag));

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
		{
			auto temp = (*manager)->swallow(food_list_, controled_ball_list_);
			command.command = NEW_FOOD;
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
		{
			command.command = NEW_VIRUS;
			auto temp = (*manager)->swallowVirus(virus_list_);
			for (auto i = 0; i < temp; ++i)
			{
				auto virus = VirusBall::createBall();
				if (virus)
				{
					virus_list_.push_back(virus);
					controled_ball_layer->addChild(virus, virus->getScore());
					auto x = getDoubleRand(background_size.width - virus->getSize()) + virus->getSize() / 2;
					auto y = getDoubleRand(background_size.height - virus->getSize()) + virus->getSize() / 2;
					virus->setPosition(Vec2(x, y));
					command.x = x;
					command.y = y;
					Server::getInstance()->addNetCommand(command);
				}
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

	Vec2 position;
	auto user_default = UserDefault::getInstance();
	if (user_default->getBoolForKey(CONTROL_KEY))
	{
		command.command = DIRECTION_BY_KEY;
		position = local_controler_->getKeyDirection();
	}
	else
	{
		command.command = DIRECTION;
		position = local_controler_->getMousePosition();
	}
	command.id = 0x0000;
	command.x = position.x;
	command.y = position.y;
	Server::getInstance()->addNetCommand(command);
	
}

void GameControler::updateWithClient()
{
	auto food_layer = static_cast<Layer*>(this->getChildByTag(g_kFoodLayerFlag));
	auto food_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto controled_ball_layer = static_cast<Layer*>(this->getChildByTag(g_kControledBallLayerFlag));

	for (auto manager = manager_container_.begin(); manager != manager_container_.end(); ++manager)
	{
		(*manager)->swallow(food_list_, controled_ball_list_);
		(*manager)->swallowVirus(virus_list_);
	}
	auto local_command = Client::getInstance()->getLocalCommand();
	for (auto i = local_command.begin(); i != local_command.end(); ++i)
	{
		{
			FoodBall* new_food = nullptr;
			VirusBall* new_virus = nullptr;
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
			case NEW_VIRUS:
				new_virus = VirusBall::createBall();
				if (new_virus)
				{
					controled_ball_layer->addChild(new_virus, new_virus->getScore());
					new_virus->setPosition(Vec2(i->x, i->y));
					virus_list_.push_back(new_virus);
				}
			case DIRECTION: case DIVIDE:  case DIRECTION_BY_KEY: default:
				net_controler_->addCommand(*i);
				break;
			}
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
	auto user_default = UserDefault::getInstance();
	Vec2 position;
	if (user_default->getBoolForKey(CONTROL_KEY))
	{
		command.command = DIRECTION_BY_KEY;
		position = local_controler_->getKeyDirection();
	}
	else
	{
		command.command = DIRECTION;
		position = local_controler_->getMousePosition();
	}
	command.id = Client::getInstance()->getId();
	command.x = position.x;
	command.y = position.y;
	Client::getInstance()->addNetCommand(command);
}

bool GameControler::isKeyPressed(EventKeyboard::KeyCode keyCode) {
	if (keys[keyCode]) {
		return true;
	}
	else {
		return false;
	}
}

void GameControler::keyPressedDuration(EventKeyboard::KeyCode code) {
	auto sc = QuitScene::createScene();
	keys[code] = false;
	Director::getInstance()->pushScene(sc);
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
		SimpleAudioEngine::getInstance()->playEffect("sound/bubble.mp3");
	}
}
