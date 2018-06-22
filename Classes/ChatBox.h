#ifndef BUBG_CLASSES_CHAT_BOX_H_
#define BUBG_CLASSES_CHAT_BOX_H_

#include "cocos2d.h"
#include "Net/net.h";
#include "cocos-ext.h"
#include <vector>

class ChatBox : public  cocos2d::Layer,cocos2d::extension::EditBoxDelegate
{
public:
	ChatBox() = default;
	~ChatBox();
	virtual bool init();
	void initBox(int message_number);
	static ChatBox* createBox(int message_number = 5);
	CREATE_FUNC(ChatBox);
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);
	void update(float dt);
protected:
	int label_size_;
	std::vector<std::pair<cocos2d::Label*,int>> message_;
};

#endif // !BUBG_CLASSES_CHAT_BOX_H_



