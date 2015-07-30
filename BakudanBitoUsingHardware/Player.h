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
		TYPE_NONE, //�Ѿ�����
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

	
	//��ʼ��DynamicObject
	void set( int x, int y, Type, State* s );
	
	//����DO
	void draw( const Image* ) const;

	void handleInput( );

	Vector2 seleteCell();

	void update();
	
	//�Ƅ�
	void move( const int* wallsX,const int* wallsY, int wallNumber );

	//get inner position
	Vector2 getInner(void) const;
	
	//���������ĸ��Թ��������ġ�
	Vector2 getCell(void) const;

	//��DynamicObject�Ľ����ж�
	bool isIntersect( const DynamicObject& ) const;
	
	//ǽר��
	bool isIntersectWall( int wallCellX, int wallCellY );

	//����������

	void applyDamage( int damage );
	void applyHeal( int heal );
	int getType() const;
	int getAP() const;
	Magic::TYPE getCurrentMagic() const;
	AppStatus::ITEM getCurrentItem() const;
	void die(); //ִ������(mType��������ΪNONE�ı�F)
	bool isDead() const; //�Ƿ�����
	void setBuff( unsigned b );
	void resetBuff( unsigned b );
	bool checkBuff( unsigned b )const;
	
	
private:
	//ǽר��
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	void changeSpeed( double n );
	//ȡ�ñ�֡���Ƅ���
	void getVelocity( int* dx, int* dy ) const;
	int getItemNumber( AppStatus::ITEM item ) const;

	bool useItem( AppStatus::ITEM i);
	Vector2 getForward();
	void select();

	//ȡ���Ƅӷ���
	void getDirection( int* dx, int* dy ) const;
	void nextMagic();
	void lastMagic();
	void nextItem();
	void lastItem();
	//����(�ڲ��gλ)
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
