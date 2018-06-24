#ifndef BUBG_CLASSES_WAITTING_ROOM_H_
#define BUBG_CLASSES_WAITTING_ROOM_H_

#include "cocos2d.h"
#include "Net/Net.h"
#include "SettingHeader.h"

class WaittingRoom : public cocos2d::Scene
{
public:
	CREATE_FUNC(WaittingRoom);
	static WaittingRoom* createRoom(int state);
	virtual bool init();
	void initRoom(int state);
	//this function is used for the Server to start game;
	void startGame(cocos2d::Object *pSender);
	//this function is used to return main menu;
	void returnStartScene(cocos2d::Object *pSender);
	void update(float dt);
	WaittingRoom() = default;
	~WaittingRoom();
protected:
	int state_;
	cocos2d::Label* message_label_;
};

#endif // !BUBG_CLASSES_WAITTING_ROOM_H_




