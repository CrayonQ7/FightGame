#include "Hero.h"
USING_NS_CC;
Hero::Hero()
{
}

Hero::~Hero()
{
}

bool Hero::init()
{
	bool ret = false;
	do {
		auto texture = Director::getInstance()->getTextureCache()->addImage("$shana_1.png");
		auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 55, 62)));
		CC_BREAK_IF(!this->initWithSpriteFrame(frame0));
		Animation* idleAnim = this->createAnimation("$shana_1.png", 220, 62, 4, 0.3);
		this->setIdleAction(RepeatForever::create(Animate::create(idleAnim)));
		Animation* walkAnim = this->createAnimation("$shana_forward.png", 592, 63, 8, 0.1);
		this->setWalkAction(RepeatForever::create(Animate::create(walkAnim)));

		Animation* attackAnim = this->createAnimation("$shana_2.png", 868, 90, 7, 0.08);
		this->setAttackAction(Sequence::create(Animate::create(attackAnim), BaseSprite::createIdleCallbackFunc(), NULL));
		Animation* hurtAnim = this->createAnimation("$shana_4.png", 130, 58, 2, 0.1);
		this->setHurtAction(Sequence::create(Animate::create(hurtAnim), BaseSprite::createIdleCallbackFunc(), NULL));
		Animation* deadAnim = this->createAnimation("$shana_dead.png", 237, 68, 3, 0.1);
		this->setDeadAction(Sequence::create(Animate::create(deadAnim), Blink::create(3, 9), NULL));
		ret = true;
	} while (0);
	return ret;
}
