#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"

#define MIN_ENEMY_COUNT 5
class GameLayer : public cocos2d::Layer
{
public:
	GameLayer();
	~GameLayer();
	virtual bool init();
	// 定义 Hero* 类型变量 hero，并只声明了其 get 方法
	CC_SYNTHESIZE_READONLY(Hero*, hero, Hero);

	// GameLayer设置了英雄的如何移动等
	void onHeroWalk(cocos2d::Point direction, float distance);
	void onHeroAttack();
	void onHeroStop();
	void update(float dt);
	void updateHero(float dt);
	void updateEnemies(float dt);

	void addEnemy();
	void onEnemyAttack(BaseSprite *pSprite);

	// 表示每个瓦片的宽高，用来控制英雄的行走区域
	float tileWidth;
	float tileHeight;

	float screenWidth;
	float screenHeight;
	cocos2d::Point origin;

	cocos2d::Array *enemies;
	CREATE_FUNC(GameLayer);  // 自动生成一个不带参数的 create 静态方法，返回一个 GameLayer* 类型指针。自动调用了init和autorelease方法
private:
	cocos2d::TMXTiledMap* tmx;
};
#endif