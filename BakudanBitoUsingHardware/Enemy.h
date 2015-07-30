#pragma once
#include "Game/DynamicObject.h"
#include "Vector2.h"
#include "AppStatus.h"
#include "Magic.h"
class AppStatus;
class DynamicObject;
class Image;
class State;

class Enemy :
	public DynamicObject
{
public:
	Enemy();
	~Enemy(void);
	public:
	enum Type{
		TYPE_SLIME,
		TYPE_BOX,
		TYPE_SHADOW,
		TYPE_BOSS,
		TYPE_NONE, //�Ѿ�����
	};

	enum Status{
		IDLE,
		ATTACK,
		HOLD,
	};

	enum Buff{
		NORMAL,
		BURNING ,
		SPEEDUP,
		SLOWDOWN,
	};

	static Enemy::Type randomEnemy();

	//��ʼ��DynamicObject
	void set( int x, int y, Type, State* s );
	
	//����DO
	void draw( const Image* ) const;
	void update();
	void lookAt(const Vector2& Cell);
	
	//get inner position
	Vector2 getInner(void) const;
	
	//���������ĸ��Թ��������ġ�
	Vector2 getCell(void) const;

	//��DynamicObject�Ľ����ж�
	bool isIntersect( const DynamicObject& ) const;
	
	//����������
	void applyDamage( int damage );
	void applyHeal( int heal );
	int getType() const;
	void die(); 
	bool isDead() const; 
	void setBuff( Buff b );
	void clearBuff();
	bool checkBuff( Buff b )const;
	bool isIntersectWall( int wallX, int wallY );
	
private:
	void randomCast();
	AppStatus *pAS;
	Buff mBuff;
	Status mStatus;
	int mBuffCount;
	int mCDCount,mAnimeCount,mCastCount;
	int mDirection;
	Vector2 mTarget;
	State* pState;
	void think();
	void updateBuff();
	int getRotation() const;
	int getDistance() const;
	int mSpeed;
	int mX,mY,mHP;
	Enemy::Type mType;
	void changeSpeed(double n);
	void getVelocity( int* dx, int* dy ) const;
//�Ƅ�
	void move( const int* wallsX,const int* wallsY, int wallNumber );
//ǽר��
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );

};
