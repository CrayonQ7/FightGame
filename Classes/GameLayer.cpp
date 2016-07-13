#include "GameLayer.h"

USING_NS_CC;

GameLayer::GameLayer() :tmx(NULL)
{
}

GameLayer::~GameLayer()
{
}

bool GameLayer::init()
{
	auto ret = false;
	do {
		CC_BREAK_IF(!Layer::init());
		auto visibleSize = Director::getInstance()->getVisibleSize();
		this->origin = Director::getInstance()->getVisibleOrigin();
		this->screenWidth = visibleSize.width;
		this->screenHeight = visibleSize.height;
		hero = Hero::create();
		hero->setPosition(origin + Point(100, 100));
		hero->runIdleAction();
		hero->setZOrder(screenHeight - hero->getPositionY());
		addChild(hero);
		tmx = TMXTiledMap::create("background.tmx");
		//tmx->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		//tmx->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		//tmx->setAnchorPoint(Vec2(0.5, 0.5));
		//tmx->setScale(Director::getInstance()->getContentScaleFactor());
		this->addChild(tmx, -1);

		tileWidth = tmx->getTileSize().width;
		tileHeight = tmx->getTileSize().height;

		hero->pAttack = CC_CALLBACK_0(GameLayer::onHeroAttack, this);
		hero->pStop = CC_CALLBACK_0(GameLayer::onHeroStop, this);
		hero->pWalk = CC_CALLBACK_2(GameLayer::onHeroWalk, this);

		enemies = Array::createWithCapacity(MIN_ENEMY_COUNT);
		enemies->retain();
		for (int i = 0; i < MIN_ENEMY_COUNT; ++i)
		{
			this->addEnemy();
		}

		this->scheduleUpdate();
		ret = true;
	} while (false);
	return true;
}

void GameLayer::onHeroWalk(cocos2d::Point direction, float distance)
{
	hero->setFlippedX(direction.x < 0 ? true : false);  // 如果英雄要向左，就翻转一下
	hero->runWalkAction();
	auto v = direction * (distance < 78 ? 1 : 3);
	hero->setVelocity(v);
}

void GameLayer::onHeroAttack()
{
	hero->runAttackAction();
}

void GameLayer::onHeroStop()
{
	hero->runIdleAction();
}

void GameLayer::update(float dt)
{
	this->updateHero(dt);
	this->updateEnemies(dt);
}

void GameLayer::updateHero(float dt)
{
	if (hero->getCurrActionState() == ACTION_STATE_WALK)
	{
		auto halfHeroFrameHeight = (hero->getDisplayFrame()->getRect().size.height) / 2;
		auto expectP = hero->getPosition() + hero->getVelocity();
		auto actualP = expectP;
		// 确保不会移出地图或在墙上
		if (expectP.y < halfHeroFrameHeight || expectP.y >(tileHeight * 2.5 + halfHeroFrameHeight))
		{
			actualP.y = hero->getPositionY();
		}
		float mapWidth = tmx->getContentSize().width;
		float halfWinWidth = screenWidth / 2;
		float halfHeroFrameWidth = (hero->getDisplayFrame()->getRect().size.width) / 2;
		if (expectP.x > halfWinWidth && expectP.x <= (mapWidth - halfWinWidth))  // 这一句是让地图移动的，但是没搞懂啊还是
		{
			this->setPositionX(this->getPositionX() - hero->getVelocity().x);
		}
		else if (expectP.x < halfHeroFrameWidth || expectP.x >= mapWidth - halfHeroFrameWidth)
		{
			actualP.x = hero->getPositionX();
		}
		hero->setPosition(actualP);
		hero->setZOrder(screenHeight - hero->getPositionY());
	}
}

void GameLayer::updateEnemies(float dt)
{
	Object* obj = NULL;
	auto distance = Point::ZERO;
	auto heroPox = hero->getPosition();
	auto removedEnemies = Array::create();
	CCARRAY_FOREACH(enemies, obj)
	{
		auto pEnemy = (Enemy*)obj;
		pEnemy->execute(heroPox, hero->getDisplayFrame()->getRect().size.width);
		if (pEnemy->getCurrActionState() == ACTION_STATE_WALK)
		{
			auto location = pEnemy->getPosition();
			auto direction = pEnemy->getMoveDirection();
			auto expect = location + direction;
			auto halfEnemyFrameHeight = (pEnemy->getDisplayFrame()->getRect().size.height) / 2;
			if (expect.y < halfEnemyFrameHeight || expect.y >(tileHeight * 2.5 + halfEnemyFrameHeight))
			{
				direction.y = 0;
			}
			pEnemy->setFlippedX(direction.x < 0 ? true : false);
			pEnemy->setPosition(location + direction);
			pEnemy->setZOrder(pEnemy->getPositionY());
		}
	}
	CCARRAY_FOREACH(removedEnemies, obj)
	{
		Enemy *pEnemy = (Enemy*)obj;
		enemies->removeObject(pEnemy);
		this->removeChild(pEnemy, true);
	}
	removedEnemies->removeAllObjects();
}

void GameLayer::addEnemy()
{
	auto winSize = Director::getInstance()->getWinSize();
	auto location = Point::ZERO;
	auto enemy = Enemy::create();
	auto halfEnemyFrameHeight = (enemy->getDisplayFrame()->getRect().size.height) / 2;
	auto heroPosX = hero->getPositionX();
	auto halfWinWidth = winSize.width / 2;
	while (fabsf(heroPosX - location.x) < 150)
	{
		if (heroPosX < halfWinWidth)
		{
			location.x = hero->getPositionX() + CCRANDOM_0_1() * halfWinWidth;
		}
		else {
			location.x = hero->getPositionX() + CCRANDOM_MINUS1_1() * halfWinWidth;
		}
	}
	auto maxY = tileHeight * 2.5 + halfEnemyFrameHeight;
	location.y = CCRANDOM_0_1() * maxY;
	if (location.y < halfEnemyFrameHeight)
	{
		location.y = halfEnemyFrameHeight;
	}
	enemy->pAttack = CC_CALLBACK_0(GameLayer::onEnemyAttack, this, enemy);
	enemy->setPosition(origin + location);
	enemy->setZOrder(screenHeight - enemy->getPositionY());
	enemy->runIdleAction();
	enemy->setATK(5); // 攻击力为5
	enemy->setHP(30);  // 血量为30
	enemy->setVelocity(Point(0.5f, 0.5f));  // 速度
	enemy->setEyeArea(200); // 视野范围
	enemy->setAttackArea(25); // 攻击距离
	enemies->addObject(enemy);
	addChild(enemy);
}

void GameLayer::onEnemyAttack(BaseSprite * pSprite)
{
}
