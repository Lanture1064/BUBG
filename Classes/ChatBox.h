#ifndef BUBG_CLASSES_CHAT_BOX_H_
#define BUBG_CLASSES_CHAT_BOX_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include "SettingHeader.h"

USING_NS_CC;

class ChatBox : public  cocos2d::Layer,cocos2d::extension::EditBoxDelegate
{
public:
	ChatBox() = default;
	~ChatBox();
	virtual bool init();
	void initBox(int id,int state,int message_number);
	static ChatBox* createBox(int id,int state,int message_number = 5);
	CREATE_FUNC(ChatBox);
	virtual void editBoxReturn(cocos2d::extension::EditBox* edit_box);
	void update(float dt);
	void displayMessage(const std::string& text);
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;

	void menuQuitCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

protected:
	int label_size_;
	std::vector<std::pair<cocos2d::Label*,int>> message_;
	int id_;
	int state_;
};

#endif // !BUBG_CLASSES_CHAT_BOX_H_



