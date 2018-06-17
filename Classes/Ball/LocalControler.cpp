#include "LocalControler.h"
#include "ControledBall.h"
#include "ControledBallManager.h"
#include "../Net/Net.h"
USING_NS_CC;


bool LocalControler::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

void LocalControler::initControler(ControledBallManager * manager, std::list<ControledBall*> *ball_list)
{
	manager_ = manager;
	ball_list_ = ball_list;
	divide_count_ = 0;

	auto mouse_listener = EventListenerMouse::create();
	mouse_listener->onMouseMove = [=](Event* mouse_event) {
		EventMouse* e = static_cast<EventMouse*> (mouse_event);
		mouse_position_ = e->getLocation();
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse_listener, this);

	auto key_listener = EventListenerKeyboard::create();
	key_listener->onKeyPressed = [=](EventKeyboard::KeyCode key_code, Event* key_event) {
		std::list<ControledBall*> temp;
		switch (key_code)
		{
		case EventKeyboard::KeyCode::KEY_SPACE:
			manager_->divideBall(mouse_position_);
			divide_state_lock_.lock();
			divide_count_++;
			divide_state_lock_.unlock();
			break;
		default:
			break;
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(key_listener, this);

	this->scheduleUpdate();
}

LocalControler * LocalControler::createControler(ControledBallManager * manager, std::list<ControledBall*> *ball_list)
{
	auto controler = new LocalControler();
	if (controler&&controler->init())
	{
		controler->initControler(manager, ball_list);
		controler->autorelease();
		return controler;
	}
	CC_SAFE_DELETE(controler);
	return nullptr;
}


LocalControler::~LocalControler()
{

}

void LocalControler::update(float dt)
{
	manager_->moveTo(Director::getInstance()->getDeltaTime(), Director::getInstance()->convertToGL(mouse_position_));
	manager_->updateState();
}

int LocalControler::getDivideCount()
{
	divide_state_lock_.lock();
	auto temp = divide_count_;
	divide_count_ = 0;
	divide_state_lock_.unlock();
	return temp;
}

cocos2d::Vec2 LocalControler::getMousePosition() const
{
	return mouse_position_;
}

cocos2d::Vec2 LocalControler::getManagerPosition() const
{
	return manager_->getPosition();
}
