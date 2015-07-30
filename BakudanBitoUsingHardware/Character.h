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
		TYPE_NONE, //�Ѿ�����
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

	
	//��ʼ��DynamicObject
	void set( int x, int y, State* s );
	
	//����DO
	void draw( const Image* );

	void handleInput( );

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
	void die(); //ִ������(mType��������ΪNONE�ı�F)
	bool isDead() const; //�Ƿ�����
	void setBuff( Buff b );
	void resetBuff( );
	bool checkBuff( Buff b )const;
	Vector2 getCellForward();
	
private:
	//ǽר��
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	void changeSpeed( double n );
	//ȡ�ñ�֡���Ƅ���
	void getVelocity( double* dx, double* dy ) const;
	void handleDirectionAndRotation();
	void useItem();
	


	//ȡ���Ƅӷ���
	void getRotation( double* dx, double* dy ) const;
	void updateBuff();
	//����(�ڲ��gλ)
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
