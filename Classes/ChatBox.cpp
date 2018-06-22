#include "ChatBox.h"
#include "cocos-ext.h"
#include "Net/Net.h"

USING_NS_CC;
using namespace cocos2d::extension;


ChatBox::~ChatBox()
{}

bool ChatBox::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

void ChatBox::initBox(int id,int state, int message_number)
{
	id_ = id;
	state_ = state;
	auto size = Director::getInstance()->getVisibleSize();
	label_size_ = size.height / 36;
	this->setAnchorPoint(Vec2(0, 0));
	for (auto i = 0; i < message_number; ++i)
	{
		auto label = Label::create("", "Arial", label_size_);
		label->setColor(Color3B::BLACK);
		label->setAnchorPoint(Vec2(0, 0.5));
		int count = 0;
		auto temp = std::make_pair(label, count);
		this->addChild(label);
		label->setPosition(0, label_size_ * 3 + label_size_ * i*1.2);
		message_.push_back(temp);
	}
	auto edit_box = EditBox::create(Size(size.width / 3, label_size_*2), "inputfield.png");
	edit_box->setAnchorPoint(Vec2(0,0.5));
	this->addChild(edit_box);
	edit_box->setPosition(Vec2(0, label_size_));
	edit_box->setPlaceholderFontSize(label_size_);
	edit_box->setInputMode(EditBox::InputMode::SINGLE_LINE);
	edit_box->setMaxLength(50);
	edit_box->setDelegate(this);
	this->scheduleUpdate();
}

ChatBox * ChatBox::createBox(int id, int state, int message_number)
{
	auto chat_box = new ChatBox();
	if (chat_box&&chat_box->init())
	{
		chat_box->initBox(id,state,message_number);
		chat_box->autorelease();
		return chat_box;
	}
	CC_SAFE_DELETE(chat_box);
	return nullptr;
}

void ChatBox::editBoxReturn(EditBox * edit_box)
{
	auto text =  edit_box->getText();
	if (text != "")
	{
		std::string name = std::string("Player");
		std::string number = "00: ";
		number[0] += id_ / 10;
		number[1] += id_ % 10;
		name += number;
		name += text;
		this->displayMessage(name);
		if (state_ == USE_SERVER)
		{
			Server::getInstance()->sendMessage(name);
		}
		else if (state_ == USE_CLIENT)
		{
			Client::getInstance()->sendMessage(name);
		}
		edit_box->setText("");
	}
}

void ChatBox::update(float dt)
{
	for (auto i = message_.begin(); i != message_.end(); ++i)
	{
		if (i->first->getString() != "")
		{
			if (i->second >= 600)
			{
				i->second = 0;
				i->first->setString("");
			}
			else
			{
				++(i->second);
			}
		}
	}
	if (state_ == USE_SERVER)
	{
		auto temp = Server::getInstance()->getMessage();
		for (auto text = temp.begin(); text != temp.end(); ++text)
		{
			this->displayMessage(*text);
		}
	}
	else  if (state_ == USE_CLIENT)
	{
		auto text = Client::getInstance()->getMessage();
		this->displayMessage(text);
	}
}

void ChatBox::displayMessage(const std::string& text)
{
	for (auto i = message_.begin(); i != message_.end(); ++i)
	{
		if (i->first->getString() == "")
		{
			i->first->setString(text);
			i->second = 0;
			return;
		}
	}
	int index = 0;
	int max_count = 0;
	for (auto i = 0; i != message_.size(); ++i)
	{
		if (message_[i].second > max_count)
		{
			index = i;
			max_count = message_[i].second;
		}
	}
	message_[index].first->setString(text);
	message_[index].second = 0;
}
