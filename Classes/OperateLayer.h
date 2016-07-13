#ifndef __OPERATE_LAYER_H__
#define __OPERATE_LAYER_H__

#include "cocos2d.h"
#include "Hero.h"

class OperateLayer : public cocos2d::Layer
{
public:
	OperateLayer();
	~OperateLayer();
	virtual bool init();
	CC_SYNTHESIZE(Hero*, hero, Hero);
	CREATE_FUNC(OperateLayer);
	
	// OperateLayer��������Ρ���ʲô����Ӣ��
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	//void onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event);
	//void onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event);

	//void update(float f);
private:
	void showJoystick(cocos2d::Point pos);
	void hideJoystick();
	void updateJoystick(cocos2d::Point direction, float distance);
	cocos2d::Sprite *joystick; // ҡ��
	cocos2d::Sprite *joystickBg;
	//Point move;  // ��ɫ�ƶ�����
};

#endif
