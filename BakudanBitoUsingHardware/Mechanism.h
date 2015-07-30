#pragma once
#include "Game\StaticObject.h"
#include "FX.h"
#include "Image.h"
#include "Wall.h"

class State;

class Mechanism :
	public StaticObject
{
public:
	enum Flag{
		FLAG_NOTHING = ( 0 << 0 ),
		FLAG_DIECANDLE = ( 1 << 0 ),
		FLAG_LIGHTCANDLE = ( 1 << 1 ),
		FLAG_CLOSEDGATE = ( 1 << 2 ), 
		FLAG_OPENEDGATE = ( 1 << 3 ),
		FLAG_CLOSEDBOX = ( 1 << 4 ),
		FLAG_OPENEDBOX = ( 1 << 5 ),
		FLAG_PORTAL = ( 1 << 6 ),
		FLAG_WEB  = ( 1 << 7 ),
	};

public:
	State* state;
	Mechanism(void);
	~Mechanism(void);
	void draw( int , int ,const Image* ) const;
	bool isInstanced(void);
	void setInstanced(bool);
	void work(void);
	void setFlag( unsigned flag, int direction );
	void setFlag( unsigned flag );
	void setFX( FX* pFX );
	int mCellX;
	int mCellY;
private:
	int mDirection;
	unsigned mFlags; //±ê¼Ç
	FX *mFX0, *mFX1, *mFX2;
	int convertCellToInner( int );
	Wall* wall;

};
