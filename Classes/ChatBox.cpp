#include "ChatBox.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

ChatBox::ChatBox()
{}


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

void ChatBox::initBox(int message_number)
{
	auto size = Director::getInstance()->getVisibleSize();
	label_size_ = size.height / 36;
	
	for (auto i = 0; i < message_number; ++i)
	{
		auto label = Label::create("", "Arial", label_size_);
		int count = 0;
		auto temp = std::make_pair(label, count);
		this->addChild(label);
		label->setPosition(0, label_size_ * 3 + label_size_ * i*1.2);
		message_.push_back(temp);
	}
	auto edit_box = EditBox::create(Size(size.width / 3, label_size_), "inputfield.png");
	this->addChild(edit_box);
	edit_box->setPosition(Vec2(0, label_size_));
	edit_box->setPlaceholderFontSize(label_size_);
	edit_box->setMaxLength(50);
	edit_box->setDelegate(this);
	this
}

void ChatBox::editBoxReturn(EditBox * editBox)
{
	
}
