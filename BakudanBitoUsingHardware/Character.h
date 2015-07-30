#pragma once
#include "Game/DynamicObject.h"
#include "Vector2.h"
#include "AppStatus.h"
#include "Magic.h"

class Image;
class State;

class Character :
	public DynamicObject
{
public:
	Character(void);
	~Character(void);
	public:
	enum Type{
		TYPE_CHARACTER,
		TYPE_NONE, //已经死亡
	};

	enum Status{
		RECOVER,//talk / react 
		IDLE,
		MOVE,
		RUN,
		CAST,
	};

	enum Buff{
		NORMAL = 0,
		BURNING = 1,
		SPEEDUP = 2,
		SLOWDOWN = 3,
	};

	
	//初始化DynamicObject
	void set( int x, int y, State* s );
	
	//绘制DO
	void draw( const Image* );

	void handleInput( );

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
	void die(); //执行死亡(mType变量设置为NONE的表現)
	bool isDead() const; //是否死亡
	void setBuff( Buff b );
	void resetBuff( );
	bool checkBuff( Buff b )const;
	Vector2 getCellForward();
	
private:
	//墙专用
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	void changeSpeed( double n );
	//取得本帧的移動量
	void getVelocity( double* dx, double* dy ) const;
	void handleDirectionAndRotation();
	void useItem();
	


	//取得移動方向
	void getRotation( double* dx, double* dy ) const;
	void updateBuff();
	//座標(内部単位)
	int mX;
	int mY;

	AppStatus* pAS;
	Magic* pM;
	Status mStatus;
	Buff mBuff;
	int mBuffCount,mCastCount,mAnimeCount,mRecoverCount;
	int mHP;
	int mAP;
	int mSpeed;
	Type mType;
	int mDirection;//0=U,1=D,2=L,3=R;
	int mRotation;
	State* pState;

};
