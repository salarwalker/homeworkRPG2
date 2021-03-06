#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Game/DynamicObject.h"
#include "Game/StaticObject.h"
#include "Image.h"
#include "Pad.h"

using namespace GameLib;

namespace {

//適当的参数集合

static const int HALF_SIZE = 8000;
static const int SCALE = 16000;

//转换到内部単位
int convertCellToInner( int x ){
	return x * SCALE + HALF_SIZE;
}
//从内部単位到像素単位
int convertInnerToPixel( int x ){
	return  ( x - HALF_SIZE + 500  ) / 1000;
}

int convertInnerToCell( int x ){
	return ( x - HALF_SIZE ) /  SCALE ;
}


} //namespace{}

//尽量写入无效值。使其如果不调用set直接使用会出错。
DynamicObject::DynamicObject() : 
mType( TYPE_NONE ),
mX( -1 ),
mY( -1 ),
mSpeed( -1 ){
}

void DynamicObject::set( int x, int y, Type type ){
	//内部座標値変換
	mX = convertCellToInner( x );
	mY = convertCellToInner( y );
	mType = type;
	mSpeed = 1000;//速度。単位是内部単位/帧
}

void DynamicObject::draw( const Image* image ) const {
	image->drawUI();
}

/*void DynamicObject::move( const int* wallsX, int* wallsY, int wallNumber ){
return;
}*/

bool DynamicObject::isIntersectWall( int x, int y, int wallX, int wallY ){
	int wx = convertCellToInner( wallX );
	int wy = convertCellToInner( wallY );

	int al = x - HALF_SIZE; //left A
	int ar = x + HALF_SIZE; //right A
	int bl = wx - HALF_SIZE; //left B
	int br = wx + HALF_SIZE; //right B
	if ( ( al < br ) && ( ar > bl ) ){
		int at = y - HALF_SIZE; //top A
		int ab = y + HALF_SIZE; //bottom A
		int bt = wy - HALF_SIZE; //top B
		int bb = wy + HALF_SIZE; //bottom B
		if ( ( at < bb ) && ( ab > bt ) ){
			return true;
		}
	}
	return false;
}

bool DynamicObject::isIntersectWall( int wallX, int wallY ){
	return isIntersectWall( mX, mY, wallX, wallY );
}

void DynamicObject::getVelocity( int* dx, int* dy ) const {

	//取得方向
	getDirection( dx, dy );
	//速度計算
	*dx = *dx * mSpeed;
	*dy = *dy * mSpeed;
}

void DynamicObject::getDirection( int* dx, int* dy ) const {
	*dx = *dy = 0;
}


bool DynamicObject::isIntersect( const DynamicObject& o ) const {
	int al = mX - HALF_SIZE; //left A
	int ar = mX + HALF_SIZE; //right A
	int bl = o.mX - HALF_SIZE; //left B
	int br = o.mX + HALF_SIZE; //right B
	if ( ( al < br ) && ( ar > bl ) ){
		int at = mY - HALF_SIZE; //top A
		int ab = mY + HALF_SIZE; //bottom A
		int bt = o.mY - HALF_SIZE; //top B
		int bb = o.mY + HALF_SIZE; //bottom B
		if ( ( at < bb ) && ( ab > bt ) ){
			return true;
		}
	}
	return false;

}

void DynamicObject::applyHeal( int heal ){
	mHP += heal;
//	pAS = AppStatus::instance();
//	int Max = pAS->getMaxHP();
//	if( mHP > Max ){
//		mHP = Max;
//	}
//	pAS = 0;
}

void DynamicObject::applyDamage(int damage){
	mHP -= damage;
	if( mHP <= 0 ){
		die();
	}
}

Vector2 DynamicObject::getCell() const {
	int x = convertInnerToCell( mX );
	int y = convertInnerToCell( mY );
	return Vector2( x, y );
}

Vector2 DynamicObject::getInner() const{
	return Vector2( mX, mY );
}

int DynamicObject::getType() const{
	return mType;
}

void DynamicObject::die(){
	mType = TYPE_NONE;
}

bool DynamicObject::isDead() const {
	return ( mType == TYPE_NONE );
}

