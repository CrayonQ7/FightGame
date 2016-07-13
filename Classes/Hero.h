#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "BaseSprite.h"

class Hero : public BaseSprite
{
public:
	Hero();
	~Hero();
	bool init();
	CREATE_FUNC(Hero);
	std::function<void(cocos2d::Point, float)> pWalk;
	std::function<void(void)> pStop;
};

#endif
