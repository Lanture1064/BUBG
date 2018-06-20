//this head file inclue some function to help calculate;
#ifndef BUBG_CLASSES_TOOL_MATH_TOOL_H_
#define BUBG_CLASSES_TOOL_MATH_TOOL_H_
#include "cocos2d.h"

//this function is transformed the score of the ball to the size of the ball;
double ScoreToSize(unsigned int score);

//the funtion return the square of the two Vec2;
double calDistence(const cocos2d::Vec2 &i, const cocos2d::Vec2 &j);
double ScoreToSpeed(int score);

double getDoubleRand(int range);


#endif // !BUBG_CLASSES_TOOL_MATH_TOOL_H_

