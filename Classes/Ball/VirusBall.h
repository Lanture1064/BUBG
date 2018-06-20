#ifndef BUBG_CLASSES_BALL_VIRUS_BALL_H_
#define BUBG_CLASSES_BALL_VIRUS_BALL_H_

#include "cocos2d.h"
#include "BaseBall.h"
#include "../Tool/MathTool.h"

class VirusBall : public BaseBall
{
public:
	VirusBall() = default;
	~VirusBall();
	virtual bool init();
	static VirusBall* createBall();
	bool isDelete() const;
	void deleteVirus();
	CREATE_FUNC(VirusBall);
protected:
	bool is_delete_;
};

#endif // !BUBG_CLASSES_BALL_VIRUS_BALL_H_