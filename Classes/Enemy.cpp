#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy() :
	nextDecisionTime(0)
{
}

Enemy::~Enemy()
{
}
bool Enemy::init()
{
	bool ret = false;
	do
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage("Enemy_walk.png");
		auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 114, 110)));
		CC_BREAK_IF(!this->initWithSpriteFrame(frame0));
		Animation* idleAnim = this->createAnimation("Enemy_idle.png", 206, 100, 2, 0.3);
		this->setIdleAction(RepeatForever::create(Animate::create(idleAnim)));
		Animation* walkAnim = this->createAnimation("Enemy_walk.png", 412, 100, 4, 0.1);
		this->setWalkAction(RepeatForever::create(Animate::create(walkAnim)));
		Animation* attackAnim = this->createAnimation("Enemy_attack.png", 515, 100, 5, 0.08);
		this->setAttackAction(Sequence::create(Animate::create(attackAnim), BaseSprite::createIdleCallbackFunc(), NULL));
		Animation* hurtAnim = this->createAnimation("Enemy_idle.png", 228, 110, 2, 0.3);  // 因为没有图，所以还没实现
		this->setHurtAction(Sequence::create(Animate::create(hurtAnim), BaseSprite::createIdleCallbackFunc(), NULL));
		Animation* deadAnim = this->createAnimation("Enemy_idle.png", 228, 110, 2, 0.3);  // 因为没有图，所以还没实现
		this->setDeadAction(Sequence::create(Animate::create(deadAnim), Blink::create(3, 9), NULL));
		ret = true;
	} while (0);
	return ret;
}

void Enemy::execute(const cocos2d::Point& target, float targetBodyWidth)
{
	if (nextDecisionTime == 0)
	{
		this->decide(target, targetBodyWidth);
	}
	else {
		--nextDecisionTime;
	}
}

void Enemy::decide(const cocos2d::Point& target, float targetBodyWidth)
{
	auto location = this->getPosition();
	auto distance = location.getDistance(target);
	distance = distance - (targetBodyWidth / 2 + this->getDisplayFrame()->getRect().size.width / 2 + 30);
	auto isFlippedX = this->isFlippedX();
	auto isOnTargetLeft = (location.x < target.x ? true : false);
	// 如果机器人向左移动并且在英雄左边，或者 机器人向右移动并且在英雄右边，那么机器人只会继续巡逻或者休息
	if ((isFlippedX && isOnTargetLeft) || (!isFlippedX && !isOnTargetLeft))
	{
		this->aiState = CCRANDOM_0_1() > 0.5f ? AI_PATROL : AI_IDLE;
	}
	else {
		if (distance < eyeArea)  // 如果被机器人发现了
		{
			this->aiState = distance < attackArea ? AI_ATTACK : AI_PURSUIT;  // 在机器人攻击范围内，那机器人直接攻击；否则就跟随
		}
		else {  // 没有被发现，机器人就会巡逻或者休息
			this->aiState = CCRANDOM_0_1() > 0.5f ? AI_PATROL : AI_IDLE;
		}
	}

	switch (aiState)
	{
	case AI_ATTACK:  // 攻击
	{
		this->runAttackAction();
		this->pAttack();
		this->nextDecisionTime = 50;
	}
	break;
	case AI_IDLE:  // x休息
	{
		this->runIdleAction();
		this->nextDecisionTime = CCRANDOM_0_1() * 100;
	}
	break;
	case AI_PATROL:  // 巡逻
	{
		this->runWalkAction();
		this->moveDirection.x = CCRANDOM_MINUS1_1();  // 产生一个 [-1, 1] 的随机数
		this->moveDirection.y = CCRANDOM_MINUS1_1();
		moveDirection.x = moveDirection.x > 0 ? (moveDirection.x + velocity.x) : (moveDirection.x - velocity.x);
		moveDirection.y = moveDirection.y > 0 ? (moveDirection.y + velocity.y) : (moveDirection.y - velocity.y);
		this->nextDecisionTime = CCRANDOM_0_1() * 100;
	}
	break;
	case AI_PURSUIT:
	{
		this->runWalkAction();
		this->moveDirection = ccpNormalize(target - location);
		this->setFlippedX(moveDirection.x < 0 ? true : false);
		moveDirection.x = moveDirection.x > 0 ? (moveDirection.x + velocity.x) : (moveDirection.x - velocity.x);
		moveDirection.y = moveDirection.y > 0 ? (moveDirection.y + velocity.y) : (moveDirection.y - velocity.y);
		this->nextDecisionTime = 10;
	}
	break;
	}
}
