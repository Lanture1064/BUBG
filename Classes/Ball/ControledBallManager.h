#ifndef BUBG_CLASSES_BALL_CONTROLED_BALL_MANAGER_H_
#define BUBG_CLASSES_BALL_CONTROLED_BALL_MANAGER_H_

#include "cocos2d.h"
#include <string>
#include <list>
#include <vector>

class ControledBall;

class ControledBallManager : public cocos2d::Node
{
protected:
	static std::vector<bool> kUsedColor;
	std::list<ControledBall*> controled_ball_list_;
	double speed_;
	cocos2d::Node* father_;
	bool is_dead_;
	std::string color_directory_;
public:
	//please use it to add child to the scene;
	//Notice: the function only can be used once;
	void addFatherScene(cocos2d::Node* father);
	//the function is used to remove and delete the ball which is uselee;
	void removeBall();
	void updateState();
	//the function is used to make the controled balls run toward target;
	void moveTo(double time, cocos2d::Vec2 target);
	void divideBall(cocos2d::Vec2 target);
	bool isDead();
	static ControledBallManager* createManager();
	static ControledBallManager* createManager(std::string color_directory);
	CREATE_FUNC(ControledBallManager);
	virtual bool init();
	void initManager(std::string color_directory);
	~ControledBallManager();
	ControledBallManager() = default;
};
#endif // !BUBG_CLASSES_BALL_CONTROLED_BALL_MANAGER_H_
