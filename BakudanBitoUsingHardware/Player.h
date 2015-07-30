#pragma once
#include "Game/DynamicObject.h"
#include "Vector2.h"
#include "AppStatus.h"

class DynamicObject;
class Mechanism;
class Image;
class Magic;
class State;

class Player :
	public DynamicObject
{
public:
	Player(void);
	~Player(void);
	public:
	enum Type{
		TYPE_PLAYER,
		TYPE_NONE, //已经死亡
	};

	enum Status{
		UNCONTROLLABLE,//talk react use item
		IDLE,
		MOVE,
		RUN,
		CAST,
	};

	enum Buff{
		NORMAL = 0,
		BURNING = ( 0 << 1 ),
		SPEEDUP = ( 0 << 2 ),
		SLOWDOWN = ( 0 << 3 ),
		BEATBACK = ( 0 << 4 ),
		FETTERS = ( 0 << 5 ),
		FLAMMABLE = ( 0 << 6 ),
	};

	
	//初始化DynamicObject
	void set( int x, int y, Type, State* s );
	
	//绘制DO
	void draw( const Image* ) const;

	void handleInput( );

	Vector2 seleteCell();

	void update();
	
	//移動
	void move( const int* wallsX,const int* wallsY, int wallNumber );

	//get inner position
	Vector2 getInner(void) const;
	
	//查找是在哪个迷宫格子中心。
	Vector2 getCell(void) const;

	//和DynamicObject的交差判定
	bool isIntersect( const DynamicObject& ) const;
	
	//墙专用
	bool isIntersectWall( int wallCellX, int wallCellY );

	//便利函数集

	void applyDamage( int damage );
	void applyHeal( int heal );
	int getType() const;
	int getAP() const;
	Magic::TYPE getCurrentMagic() const;
	AppStatus::ITEM getCurrentItem() const;
	void die(); //执行死亡(mType变量设置为NONE的表現)
	bool isDead() const; //是否死亡
	void setBuff( unsigned b );
	void resetBuff( unsigned b );
	bool checkBuff( unsigned b )const;
	
	
private:
	//墙专用
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	void changeSpeed( double n );
	//取得本帧的移動量
	void getVelocity( int* dx, int* dy ) const;
	int getItemNumber( AppStatus::ITEM item ) const;

	bool useItem( AppStatus::ITEM i);
	Vector2 getForward();
	void select();

	//取得移動方向
	void getDirection( int* dx, int* dy ) const;
	void nextMagic();
	void lastMagic();
	void nextItem();
	void lastItem();
	//座標(内部単位)
	int mX;
	int mY;
	int mMagicIndex;

	Magic::TYPE mCurrentMagic;
	AppStatus::ITEM mCurrentItem;
	AppStatus* pAS;
	Status mStatus;
	unsigned mBuff;
	int mBurningCount,mSpeedUpCount,mSlowdownCount,mFettersCount,mFlammableCount,mUncontrollableCount,mCastCount,mAnimeCount;
	int mDirectionX;
	int mDirectionY;
	int mHP;
	int mAP;
	int mSpeed;
	Type mType;
	int mDirection;//0=U,1=D,2=L,3=R;
	Vector2 mSelect;
	bool isNewMagic;
	State* pState;

};
