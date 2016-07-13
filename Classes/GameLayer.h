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
	// ���� Hero* ���ͱ��� hero����ֻ�������� get ����
	CC_SYNTHESIZE_READONLY(Hero*, hero, Hero);

	// GameLayer������Ӣ�۵�����ƶ���
	void onHeroWalk(cocos2d::Point direction, float distance);
	void onHeroAttack();
	void onHeroStop();
	void update(float dt);
	void updateHero(float dt);
	void updateEnemies(float dt);

	void addEnemy();
	void onEnemyAttack(BaseSprite *pSprite);

	// ��ʾÿ����Ƭ�Ŀ�ߣ���������Ӣ�۵���������
	float tileWidth;
	float tileHeight;

	float screenWidth;
	float screenHeight;
	cocos2d::Point origin;

	cocos2d::Array *enemies;
	CREATE_FUNC(GameLayer);  // �Զ�����һ������������ create ��̬����������һ�� GameLayer* ����ָ�롣�Զ�������init��autorelease����
private:
	cocos2d::TMXTiledMap* tmx;
};
#endif