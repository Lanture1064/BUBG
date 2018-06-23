#ifndef BUBG_CLASSES_GAME_CONTROLER_H_
#define BUBG_CLASSES_GAME_CONTROLER_H_

#include "cocos2d.h"
#include <vector>
#include <list>
#include "Ball/Ball.h"
#include "Net/Net.h"

USING_NS_CC;

class ControledBallManager;
class ControledBall;
class FoodBall;
class LocalControler;
class NetControler;

//this class anchorpoint is Vec2::ZERO
class GameControler : public cocos2d::Layer
{
public:
	GameControler() = default;
	~GameControler();

	virtual bool init();
	bool initControler(int state);
	//when start game as server,call this function to init;
	bool initWithServer();
	//when start game as client,call this function to init;
	bool initWithClient();
	static GameControler* createControler(int state);
	CREATE_FUNC(GameControler);
	
	void update(float dt);
	//when start game as server,call this function to update;
	void updateWithServer();
	//when start game as client,call this function to update;
	void updateWithClient();
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	void keyPressedDuration(EventKeyboard::KeyCode code);
protected:
	std::vector<ControledBallManager*> manager_container_;
	std::list<FoodBall*> food_list_;
	std::list<ControledBall*> controled_ball_list_;
	std::list<VirusBall*> virus_list_;
	int state_;
	LocalControler* local_controler_;
	NetControler* net_controler_;
};
#endif // !BUBG_CLASSES_GAME_CONTROLER_H_


