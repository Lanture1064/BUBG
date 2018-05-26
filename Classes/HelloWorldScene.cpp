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

	//create background layer
	Sprite *bg = Sprite::create("menu/background.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);
	
	//create start button
	Sprite *startSpriteNormal = Sprite::create("menu/start01.png");
	Sprite *startSpriteSelected = Sprite::create("menu/start02.png");

	MenuItemSprite *startMenuItem = MenuItemSprite::create(startSpriteNormal,
		startSpriteSelected,
		CC_CALLBACK_1(HelloWorld::getGameScene, this));

	startMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(700, 300)));

	//create setting button
	MenuItemImage *settingMenuItem = MenuItemImage::create(
		"menu/help01.png",
		"menu/help02.png",
		CC_CALLBACK_1(HelloWorld::getSettingScene, this));

	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(700, 450)));

	//create quit button
	MenuItemImage *helpMenuItem = MenuItemImage::create(
		"menu/quit01.png",
		"menu/quit02.png",
		CC_CALLBACK_1(HelloWorld::quitGame, this));

	helpMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(700, 600)));

    //create button layer
	Menu* mu = Menu::create(startMenuItem, settingMenuItem, helpMenuItem, NULL);
	mu->setPosition(Vec2::ZERO);
	this->addChild(mu);

	
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
