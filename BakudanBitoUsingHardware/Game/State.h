#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H

#include "Array2D.h"
#include "Vector2.h"
#include "Enemy.h"

class FX;
class Image;
class StaticObject;
class Rect;
class Character;

class State{
public:
	State( int stageID );
	~State();
	void update();
	void draw() const;
	
	bool hasCleared() const;
	bool isAlive( void ) const;
	void clearWall( int x, int y );

	Character* getCharacter();
	//静态对象
	Array2D< StaticObject > mSOs;
	void instanceMagic(const Vector2& Cell,int Dir,Magic::TYPE CurrMagic,DynamicObject::Type t);
	int mExp;
	void reborn();
private:

	FX* pFXs;
	Enemy* pEnemys;
	Character* pC;
	int mENumber;
	int mEIndex;
	int mFXIndex;
	int mStageID;
	Image* mImage; //图
	

};

#endif
