#pragma once
#include "Vector2.h"
#include "GameLib/GameLib.h"
#include "Image.h"
#include "Magic.h"
class FX
{
public:
	enum TAG{
	INACTIVE,
	ILLUSION,
	FIRE,
	LIGHTNTING,
	LASER_HOR,
	LASER_VER,
	FIREBALL,
	FLASH_HOR,
	FLASH_VER,	
	};
	
	FX(void);
	~FX(void);
	void set( const Vector2& Cell,int Direction, int LastTime, TAG Tag );
	void inactive( void );
	void update();
	void draw( const Image* );
	bool isActived()const;
	Vector2 getInner()const;	

private:
	Vector2 mCell,mInner;
	TAG mTag;
	int mDirection;
	int mLastTime;
	int mCount;
	bool mIsActived;
	Magic* pM;
};
