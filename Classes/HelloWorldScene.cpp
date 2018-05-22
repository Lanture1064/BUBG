#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include<string>
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//the size of the font used on this scene
	const auto BUTTON_SIZE = visibleSize.width / 15;
	const auto TITLE_SIZE = visibleSize.width / 10;
	const std::string FONT_DIRECTORY = "font\Marker Felt.ttf";
	
	//create start button
	auto start_label = Label::create("START", FONT_DIRECTORY, BUTTON_SIZE);
	start_label->setColor(cocos2d::Color3B::BLACK);
	auto pStart_label = MenuItemLabel::create(start_label,this, menu_selector(HelloWorld::getGameScene));
	auto start_button = Menu::create(pStart_label, NULL);

	//create setting button
	auto setting_label = Label::create("SETTING", FONT_DIRECTORY, BUTTON_SIZE);
	setting_label->setColor(cocos2d::Color3B::BLACK);
	auto pSetting_label = MenuItemLabel::create(setting_label, this, menu_selector(HelloWorld::getSettingScene));
	auto setting_button = Menu::create(pSetting_label, NULL);

	//create quit button
	auto quit_label = Label::create("QUIT", FONT_DIRECTORY, BUTTON_SIZE);
	quit_label->setColor(cocos2d::Color3B::BLACK);
	auto pQuit_label = MenuItemLabel::create(quit_label, this, menu_selector(HelloWorld::quitGame));
	auto quit_button = Menu::create(pQuit_label, NULL);

    //create button layer
	auto button_layer = Layer::create();
	button_layer->setPosition(Vec2::ZERO);
	start_button->setPosition(Vec2(origin.x + visibleSize.width / 5 * 4, origin.y + visibleSize.height / 7 * 4));
	setting_button->setPosition(Vec2(origin.x + visibleSize.width / 5 * 4, origin.y + visibleSize.height / 7 * 3));
	quit_button->setPosition(Vec2(origin.x + visibleSize.width / 5 * 4 , origin.y + visibleSize.height / 7 * 2));
	button_layer->addChild(start_button);
	button_layer->addChild(setting_button);
	button_layer->addChild(quit_button);

	//create background layer
	auto background_layer = Layer::create();
	auto title_label = Label::create("BUBG", FONT_DIRECTORY, TITLE_SIZE);
	title_label->setColor(cocos2d::Color3B::BLACK);
	auto background_sprite = Sprite::create("background.jpg");
	auto background_size = background_sprite->getContentSize();
	background_sprite->setScaleX(visibleSize.width / background_size.width);
	background_sprite->setScaleY(visibleSize.height / background_size.height);
	background_layer->addChild(background_sprite);
	background_layer->addChild(title_label);
	background_layer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	title_label->setPosition(Vec2(-visibleSize.width / 4, visibleSize.width / 5));

	addChild(background_layer);
	addChild(button_layer);
    return true;
}

void HelloWorld::getGameScene(cocos2d::Object *pSender)
{

}

void HelloWorld::getSettingScene(cocos2d::Object *pSender)
{

}

void HelloWorld::quitGame(Object *pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
