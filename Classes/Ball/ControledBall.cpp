#include "ControledBall.h"



ControledBall::ControledBall()
{}


ControledBall::~ControledBall()
{}

FoodBall * ControledBall::createControledBall()
{
	return nullptr;
}

bool ControledBall::init()
{
	return false;
}


double ControledBall::getSpeed() const
{
	return 0.0;
}

std::list<FoodBall*> ControledBall::checkSwallow(std::list<FoodBall*> food_ball_list)
{
	return std::list<FoodBall*>();
}

std::list<ControledBall*> ControledBall::checkoutSwallow(std::list<ControledBall*> controled_ball_list)
{
	return std::list<ControledBall*>();
}
