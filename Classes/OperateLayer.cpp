#include "OperateLayer.h"
USING_NS_CC;

OperateLayer::OperateLayer() :
	joystick(NULL),
	joystickBg(NULL)
{
}

OperateLayer::~OperateLayer()
{
}

bool OperateLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF(!Layer::init());
		joystick = Sprite::create("joystick.png");
		joystickBg = Sprite::create("joystick_bg.png");
		this->addChild(joystick);
		this->addChild(joystickBg);
		this->hideJoystick();
		auto touchListener = EventListenerTouchAllAtOnce::create();  // 创建多点触摸监听�?
		touchListener->onTouchesBegan = CC_CALLBACK_2(OperateLayer::onTouchesBegan, this);
		touchListener->onTouchesMoved = CC_CALLBACK_2(OperateLayer::onTouchesMoved, this);
		touchListener->onTouchesEnded = CC_CALLBACK_2(OperateLayer::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		// 创建键盘监听�?
		//auto keyboardListener = EventListenerKeyboard::create();
		//keyboardListener->onKeyPressed = CC_CALLBACK_2(OperateLayer::onKeyPressed, this);
		//keyboardListener->onKeyReleased = CC_CALLBACK_2(OperateLayer::onKeyReleased, this);
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

		/*scheduleUpdate();*/
		ret = true;
	} while (false);
	return ret;
}

void OperateLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * unused_event)
{
	auto winSize = Director::getInstance()->getWinSize();
	auto touchIter = touches.begin();
	while (touchIter != touches.end())
	{
		auto touch = (Touch*)(*touchIter);
		auto p = touch->getLocation();
		if (p.x <= winSize.width / 2)
		{
			this->showJoystick(p);
		}
		else
		{
			hero->pAttack();
		}
		++touchIter;
	}
	//auto touch = (Touch*)(*touchIter);
	//auto p = touch->getLocation();
	//this->showJoystick(p);
}

void OperateLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * unused_event)
{
	auto winSize = Director::getInstance()->getWinSize();
	auto touchIter = touches.begin();
	auto touch = (Touch*)(*touchIter);
	Point start = touch->getStartLocation();
	if (start.x > winSize.width / 2)
	{
		return;
	}
	Point dest = touch->getLocation();
	float distance = start.getDistance(dest);
	auto direction = ccpNormalize(dest - start);
	this->updateJoystick(direction, distance);

	hero->pWalk(direction, distance);
}

void OperateLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event * unused_event)
{
	this->hideJoystick();
	hero->pStop();
}

//void OperateLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode code,cocos2d::Event * event)
//{
//	switch (code)
//	{
//	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_A:
//	//	move = Point(-5, 0);
//		break;
//	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_D:
//	//	move = Point(5, 0);
//		break;
//	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_W:
//	//	move = Point(0, 5);
//		break;
//	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_S:
//	//	move = Point(0, -5);
//		break;
//	default:
//		break;
//	}
//}
//
//void OperateLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
//{
//	switch (code)
//	{
//	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_A:
//	//	move = Point(0, 0);
//		break;
//	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_D:
//	//	move = Point(0, 0);
//		break;
//	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_W:
//	//	move = Point(0, 0);
//		break;
//	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
//	case cocos2d::EventKeyboard::KeyCode::KEY_S:
//	//	move = Point(0, 0);
//		break;
//	default:
//		break;
//	}
//}

//void OperateLayer::update(float f)
//{
//
//}

void OperateLayer::showJoystick(cocos2d::Point pos)
{
	joystick->setPosition(pos);
	joystickBg->setPosition(pos);
	joystick->setVisible(true);
	joystickBg->setVisible(true);
}

void OperateLayer::hideJoystick()
{
	joystick->setPosition(joystickBg->getPosition());
	joystick->setVisible(false);
	joystickBg->setVisible(false);
}

void OperateLayer::updateJoystick(cocos2d::Point direction, float distance)
{
	auto start = joystickBg->getPosition();
	// 33是中间摇杆的半径
	if (distance < 33)
	{
		joystick->setPosition(start + (direction * distance));
	} // 45是摇杆底盘的半径
	else if (distance > 78) {
		joystick->setPosition(start + (direction * 45));
	}
	else {
		joystick->setPosition(start + (direction * 33));
	}
}
