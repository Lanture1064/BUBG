#ifndef BUBG_CLASSES_DISCONNECT_BOX_H_
#define BUBG_CLASSES_DISCONNECT_BOX_H_

#include "cocos2d.h"
#include <string>

//this class is used when client disconnect from server;
class DisconnectBox : public cocos2d::Layer
{
public:
	DisconnectBox() = default;
	~DisconnectBox();
	CREATE_FUNC(DisconnectBox);
	static DisconnectBox* createBox(std::string text = "Disconnect from server");
	void initBox(std::string text);
	virtual bool init();
	void quitGame(cocos2d::Object* pSender);
};



#endif // !BUBG_CLASSES_DISCONNECT_BOX_H_

