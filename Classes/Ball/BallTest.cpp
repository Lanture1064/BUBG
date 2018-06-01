#include "BallTest.h"
#include "FoodBallManager.h"
#include "FoodBall.h"
#include "ControledBallManager.h"
#include "ControledBall.h"
#include "LocalControler.h"
USING_NS_CC;
#define SIMPLE_GAME_TEST
const int g_kFoodFlag = 0;
const int g_kNumberFlag = 1;
const int g_kFoodManagerFlag = 2;
const int g_kControledManagerFlag = 3;
auto food_ball_number = 0;
BallTestScene::~BallTestScene()
{
	
}

bool BallTestScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visible_size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto food_ball_manager = FoodBallManager::createManager();
	this->addChild(food_ball_manager);
	food_ball_manager->setTag(g_kFoodManagerFlag);

	auto food_layer = Layer::create();
	food_layer->setAnchorPoint(Vec2::ZERO);
	food_layer->setPosition(Vec2(origin.x, origin.y));
	this->addChild(food_layer);
	food_layer->setTag(g_kFoodFlag);
	mouse_position_ = Vec2::ZERO;
#ifdef SIMPLE_GAME_TEST


	auto temp = food_ball_manager->getNewFoodBall(100);
	food_container_.insert(food_container_.end(), temp.begin(), temp.end());
	for (auto i = food_container_.begin(); i != food_container_.end(); ++i)
	{
		(*i)->setPosition(Vec2(origin.x + getDoubleRand(visible_size.width), origin.y + getDoubleRand(visible_size.height)));
		food_layer->addChild(*i);
	}

	auto controled_ball_manager = ControledBallManager::createManager();
	auto ball_list = new std::list<ControledBall*>();
	auto temp_list = controled_ball_manager->getBallList();
	ball_list->insert(ball_list->end(), temp_list.begin(), temp_list.end());
	controled_ball_manager->addFatherScene(this);
	controled_ball_manager->setTag(g_kControledManagerFlag);
	auto layer = LocalControler::createControler(controled_ball_manager, ball_list);
	this->addChild(layer);
#else
	Sprite *bg = Sprite::create("menu/background.png");
	bg->setPosition(Vec2(origin.x + visible_size.width / 2,
		origin.y + visible_size.height / 2));
	this->addChild(bg);

	auto button_layer = Layer::create();
	button_layer->setAnchorPoint(Vec2::ZERO);
	button_layer->setPosition(Vec2(origin.x, origin.y));

	auto create_one_food_label = Label::create("Create one", "Arial", 20);
	auto pCreate_one_food_label = MenuItemLabel::create(create_one_food_label, this, menu_selector(BallTestScene::createOneFood));
	auto create_one_food_button = Menu::create(pCreate_one_food_label, NULL);

	auto create_ten_food_label = Label::create("Create ten", "Arial", 20);
	auto pCreate_ten_food_label = MenuItemLabel::create(create_ten_food_label, this, menu_selector(BallTestScene::createTenFood));
	auto create_ten_food_button = Menu::create(pCreate_ten_food_label, NULL);

	auto return_label = Label::create("Return menu", "Arial", 20);
	auto pReturn_label = MenuItemLabel::create(return_label, this, menu_selector(BallTestScene::returnMenu));
	auto return_button = Menu::create(pReturn_label, NULL);

	create_one_food_button->setPosition(Vec2(origin.x + visible_size.width / 3 * 2, origin.y + visible_size.height / 5 * 3));
	create_ten_food_button->setPosition(Vec2(origin.x + visible_size.width / 3 * 2, origin.y + visible_size.height / 5 * 2));
	return_button->setPosition(Vec2(origin.x + visible_size.width / 3 * 2, origin.y + visible_size.height / 5 * 1));
	button_layer->addChild(create_one_food_button);
	button_layer->addChild(create_ten_food_button);
	button_layer->addChild(return_button);
	this->addChild(button_layer);

	auto number_label = Label::create("food number : 0", "Arial", 30);
	number_label->setPosition(Vec2(origin.x + visible_size.width / 5, origin.y + visible_size.height / 5 * 4));
	this->addChild(number_label);
	number_label->setTag(g_kNumberFlag);
#endif // SIMPLE_GAME_TEST
	return true;
}

cocos2d::Scene * BallTestScene::createScene()
{
	return BallTestScene::create();
}

#ifndef SIMPLE_GAME_TEST
void BallTestScene::createOneFood(cocos2d::Object * pSender)
{
	auto visible_size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto food_ball_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto temp = food_ball_manager->getNewFoodBall();
	if (temp)
	{
		temp->setPosition(Vec2(origin.x + getDoubleRand(visible_size.width), origin.y + getDoubleRand(visible_size.height)));
		auto layer = this->getChildByTag(g_kFoodFlag);
		layer->addChild(temp);
		++(food_ball_number);
		auto label = static_cast<Label*>(this->getChildByTag(g_kNumberFlag));
		label->setString(StringUtils::format("food number : %d", food_ball_number));
	}
}

void BallTestScene::createTenFood(cocos2d::Object * pSender)
{
	auto visible_size = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto food_ball_manager = static_cast<FoodBallManager*>(this->getChildByTag(g_kFoodManagerFlag));
	auto temp = food_ball_manager->getNewFoodBall(10);

	food_ball_number += temp.size();
	auto label = static_cast<Label*>(this->getChildByTag(g_kNumberFlag));
	label->setString(StringUtils::format("food number : %d", food_ball_number));

	for (auto i = temp.begin(); i != temp.end(); ++i)
	{
		(*i)->setPosition(Vec2(origin.x + getDoubleRand(visible_size.width), origin.y + getDoubleRand(visible_size.height)));
		auto layer = this->getChildByTag(g_kFoodFlag);
		layer->addChild(*i);
	}
}

#endif // !SIMPLE_GAME_TEST


void BallTestScene::returnMenu(cocos2d::Object * pSender)
{
	Director::getInstance()->end();
}
inline double getDoubleRand(unsigned int range)
{
	double x = rand() / static_cast<double>(RAND_MAX);
	double y = rand() % range;
	return x + y;
}
