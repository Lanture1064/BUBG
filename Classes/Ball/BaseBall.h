#ifndef BUBG_CLASSES_BALL_BASE_BALL_H_
#define BUBG_CLASSES_BALL_BASE_BALL_H_
#include "cocos2d.h"
#include <vector>
#include <string>

enum Color { BLUE, GRAY, GREEN, RED, YELLOW };
const int kColorNumber = 5;

//this function is transformed the score of the ball to the size of the ball;
double ScoreToSize(unsigned int score);

class BaseBall : public cocos2d::Sprite
{
public:
	static const std::vector<std::string> kColorDirectoryVec;
	BaseBall();
	~BaseBall();
	int getScore() const;
	double getSize() const;
	virtual bool init();
	CREATE_FUNC(BaseBall);
protected:
	int score_;
	double size_;
};
#endif // !BUBG_CLASSES_BALLBASE_BALL_H_
