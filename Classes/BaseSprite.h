#ifndef __BASESPRITE_H__
#define __BASESPRITE_H__

#include "cocos2d.h"
// 精灵有5个状态，idle空闲，walk行走，attack攻击，hurt受伤，dead死亡
typedef enum
{
	ACTION_STATE_NONE = 0,
    ACTION_STATE_IDLE,
	ACTION_STATE_WALK,
	ACTION_STATE_ATTACK,
	ACTION_STATE_HURT,
	ACTION_STATE_DEAD
}ActionState;

class BaseSprite : public cocos2d::Sprite
{
public:
	BaseSprite();
	~BaseSprite();
	// 精灵的五个行为：
	void runIdleAction();  // 空闲
	void runWalkAction();  // 行走
	void runAttackAction();  // 攻击
	void runHurtAction();  // 受伤
	void runDeadAction();  // 死亡
	// CC_SYNTHESIZE_RETAIN(cocos2d::CCObject*, m_weapon, Weapon);定义了一个 Object* 类型的变量 m_weapon ，并直接定义默认的 get/set 方法（后面一个参数 Weapon 是方法名）
	// 即声明了一个 cocos2d::CCObject* getWeapon();和一个 void setWeapon(cocos2d::CCObject* value);
	// 在 setWeapon 的时候，调用原有 m_weapon 的 release，并且调用新值的 retain。（当然，已经排除了意外情况，如相等或者NULL之类的）
	// 在 CC_SYNTHESIZE 之后直接声明函数或者变量都会变成 public
	// 这里都声明为指针类型变量，所有在析构函数里要被 release 掉
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, idle, IdleAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, walk, WalkAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, attack, AttackAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, hurt, HurtAction);
	CC_SYNTHESIZE_RETAIN(cocos2d::Action*, dead, DeadAction);

	CC_SYNTHESIZE(ActionState, currActionState, CurrActionState);  // 当前状态

	CC_SYNTHESIZE(cocos2d::Point, velocity, Velocity);  // 速度属性
	CC_SYNTHESIZE(unsigned int, hp, HP);  // 生命值属性
	CC_SYNTHESIZE(unsigned int, atk, ATK);  // 攻击力

	std::function<void(void)> pAttack; // 表示 void pAttack();


	// “立即返回空闲状态”回调函数
	cocos2d::CallFunc* createIdleCallbackFunc();
protected:
	// 根据图片路径，分辨率，帧数
	static cocos2d::Animation* createAnimation(const char* formatStr, float width, float height, int frameCount, float f);
private:
	// 用来判断当前状态 currActionState 能否转换到 actionState
	bool changeState(ActionState actionState);
};
#endif
