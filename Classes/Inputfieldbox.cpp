#include "Inputfieldbox.h"
#include "extensions\cocos-ext.h"  
USING_NS_CC_EXT;
USING_NS_CC;

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

	//Ok��ť
	auto okMenuItem = MenuItemImage::create(
		"menu/ok-down.png",
		"menu/ok-up.png",
		CC_CALLBACK_1(Inputfield::menuOkCallback, this));

	okMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(640, 560)));

	//�����
	//����򱳾�  
	auto inputBox = Sprite::create("inputfield.jpg");
	

    //���������
	auto editBox = EditBox::create(Size(inputBox->getContentSize().width, inputBox->getContentSize().height), Scale9Sprite::create("inputfield.png"));
	//������λ��
	editBox->setPosition(Director::getInstance()->convertToGL(Vec2(640, 400)));
	//������������ַ�������� 
	editBox->setMaxLength(15);  
	//��ʼ������ 
	editBox->setText("Please enter an English name");  
	//������ɫ  
	editBox->setFontColor(Color3B(255, 255, 255)); 
	//�������ֵĴ�С  
	editBox->setFontSize(27);   
	bg->addChild(editBox);



	Menu* mn = Menu::create(okMenuItem, NULL);
	mn->setPosition(Vec2::ZERO);
	this->addChild(mn);

	return true;
}

void Inputfield::menuOkCallback(Ref* pSender)
{
	auto sc = BallTestScene::createScene();
	//change Scene from inputfield scene to balltestscene
	Director::getInstance()->replaceScene(sc);
}


