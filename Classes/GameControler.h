#ifndef BUBG_CLASSES_GAME_CONTROLER_H_
#define BUBG_CLASSES_GAME_CONTROLER_H_

#include "cocos2d.h"
#include <vector>
#include <list>

#define USE_SERVER 0
#define UES_CLIENT 1

class ControledBallManager;
class ControledBall;
class FoodBall;
struct Player;

class GameControler : public cocos2d::Layer
{
public:
	virtual bool init();
	void initControler(bool state);
	void initWithServer();
	void initWithClient();
	GameControler* createControler(bool state);
	CREATE_FUNC(GameControler);
	GameControler() = default;
	~GameControler();
	void update(float dt);
	void updateWithServer();
	void updateWithClient();
protected:
	std::vector<ControledBallManager*> manager_container_;
	std::list<FoodBall*> food_list_;
	std::list<ControledBall*> controled_ball_list_;
	int state;
};

#endif // !BUBG_CLASSES_GAME_CONTROLER_H_


