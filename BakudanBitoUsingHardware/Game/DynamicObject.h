#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H
#include "Vector2.h"

class Image;
class StaticObject;


class DynamicObject{
public:
	enum Type{
		TYPE_CHARACTER,
		TYPE_ENEMY,
		TYPE_NPC,
		TYPE_NONE, //已经死亡
	};


	
	DynamicObject();

	//初始化DynamicObject
	virtual void set( int x, int y, Type );
	
	//绘制DO
	virtual void draw( const Image* ) const;
	
	//移動（
	//virtual void move( const int* wallsX, int* wallsY, int wallNumber );
	
	//查找是在哪个迷宫格子中心。
	virtual Vector2 getCell(void) const;

	virtual Vector2 getInner(void) const;

	//和DynamicObject的交差判定
	virtual bool isIntersect( const DynamicObject& ) const;
	
	//墙专用
	virtual bool isIntersectWall( int wallCellX, int wallCellY );

	//便利函数集
	virtual int getType() const;
	virtual void die(); //执行死亡(mType变量设置为NONE的表現)
	virtual bool isDead() const; //是否死亡
	virtual void applyDamage( int damage );
	virtual void applyHeal( int heal );

	

private:
	//墙专用
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	//取得本帧的移動量
	void getVelocity( int* dx, int* dy ) const;
	//取得移動方向
	void getDirection( int* dx, int* dy ) const;
	//座標(内部単位)

	int mSpeed;
	Type mType;
	int mX;
	int mY;
	int mHP;
};

#endif
