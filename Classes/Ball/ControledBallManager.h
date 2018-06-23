#ifndef BUBG_CLASSES_BALL_CONTROLED_BALL_MANAGER_H_
#define BUBG_CLASSES_BALL_CONTROLED_BALL_MANAGER_H_

#include "cocos2d.h"
#include "ControledBall.h"
#include "FoodBall.h"
#include <string>
#include <list>
#include <vector>
#include <utility>

class ControledBallManager : public cocos2d::Node
{
protected:
	int id_;
	std::list<ControledBall*>* all_controled_ball_list_;
	static std::vector<bool> kUsedColor;
	std::list<ControledBall*> controled_ball_list_;
	double speed_;
	cocos2d::Node* father_;
	std::string color_directory_;
	std::string name_;
	cocos2d::Vec2 background_size_;
public:
	int getId() const;
	void setId(int id);

	//please use it to add child to the scene;
	//Notice: the function only can be used once;
	void addFatherScene(cocos2d::Node* father);

	//the function is used to remove and delete the ball which is uselee;
	void removeBall();
	void removeBall(ControledBall* ball);

	void updateState();
	//the function is used to make the controled balls run toward target;
	void moveTo(double time,cocos2d::Vec2 target);
	//the function is used to make the controled moved by key board;
	void moveByKey(double time, cocos2d::Vec2 direction_count);

	void divideBall(cocos2d::Vec2 target);

	const std::list<ControledBall*> &getBallList() const;

	//the first value return is the number of the food swallowed,
	//the second value is the number of the controled ball swallowed;
	std::pair<unsigned int,unsigned int> swallow(std::list<FoodBall*> &food_ball_list, std::list<ControledBall*> &controled_ball_list);
	void checkSwallowBall(const std::list<ControledBall*> &ball_list);
	int checkSwallowBall(std::list<FoodBall*> &ball_list);
	
	int swallowVirus(std::list<VirusBall*> &virus_list);

	bool isDead();

	static ControledBallManager* createManager(std::list<ControledBall*>* all_controled_ball_list, const cocos2d::Vec2 &visible_size, int id);
	static ControledBallManager* createManager(std::list<ControledBall*>* all_controled_ball_list, const cocos2d::Vec2 &visible_size, int id,
											   cocos2d::Vec2 position);
	static ControledBallManager* createManager(std::list<ControledBall*>* all_controled_ball_list, const cocos2d::Vec2 &visible_size, int id,
											   cocos2d::Vec2 position, std::string name);
	static ControledBallManager* createManager(std::list<ControledBall*>* all_controled_ball_list, const cocos2d::Vec2 &visible_size, int id,
											   cocos2d::Vec2 position, std::string name, std::string color_directory);
	CREATE_FUNC(ControledBallManager);
	virtual bool init();
	void initManager(std::list<ControledBall*>* all_controled_ball_list, cocos2d::Vec2 position, int id, std::string name,std::string color_directory,
				     const cocos2d::Vec2 &visible_size);
	~ControledBallManager();
	ControledBallManager() = default;
};
#endif // !BUBG_CLASSES_BALL_CONTROLED_BALL_MANAGER_H_