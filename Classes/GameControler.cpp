#include "GameControler.h"
#include "Ball/Ball.h"
#include "Net/Net.h"

USING_NS_CC;

const int g_kFoodManagerFlag = 1;
const int g_kFoodLayerFlag = 2;

bool GameControler::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

void GameControler::initControler(bool state)
{
	auto background = Sprite::create("menu/background_game.png");
	this->addChild(background);
	background->setScale(Director::getInstance()->getVisibleSize().width * 3 / background->getContentSize().width);

	auto food_manager = FoodBallManager::createManager();
	this->addChild(food_manager);
	food_manager->setTag(g_kFoodManagerFlag);

	auto food_layer = Layer::create();
	food_layer->setTag(g_kFoodLayerFlag);
	this->addChild(food_layer);

	auto controled_ball_layer = Layer::create();
	this->addChild(controled_ball_layer);

}

GameControler * GameControler::createControler(bool state)
{
	auto controler = new GameControler();
	if (controler&&controler->init())
	{
		controler->initControler(state);
		controler->autorelease();
		return controler;
	}
	CC_SAFE_DELETE(controler);
	return nullptr;
}

GameControler::~GameControler()
{

}

void GameControler::update(float dt)
{}
