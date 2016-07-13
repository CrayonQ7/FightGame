#include "GameScene.h"
#include "GameLayer.h"
#include "OperateLayer.h"
USING_NS_CC;
cocos2d::Scene * GameScene::createScene()
{
	auto scene = Scene::create();
	auto gameLayer = GameLayer::create();
	auto operateLayer = OperateLayer::create();
	operateLayer->setHero(gameLayer->getHero());
	scene->addChild(gameLayer, 0);
	scene->addChild(operateLayer, 1);
	return scene;
}
