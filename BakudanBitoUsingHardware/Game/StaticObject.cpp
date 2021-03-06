#include "GameLib/GameLib.h"
#include "Game/StaticObject.h"
#include "Image.h"
#include "GameLib/Framework.h"

using namespace GameLib;

namespace {

static const int HALF_SIZE = 8000;
static const int SCALE = 16000;

//转换到内部単位
int convertCellToInner( int x ){
	return x *  SCALE + HALF_SIZE;
}

}

StaticObject::StaticObject() :
mFlag( FLAG_NOTHING ){
}

StaticObject::Flag StaticObject::randomMechanism(){
	Framework f = Framework::instance();
	int n = f.getRandom(3);
	switch(n){
	case 0:
		return FLAG_WALL;
	case 1:
		return FLAG_CLOSEDGATE;
	case 2:
		return FLAG_CLOSEDBOX;
	default:
		return FLAG_NOTHING;
	}
}

void StaticObject::set(int cellX, int cellY, StaticObject::Flag f){
	mCellX=cellX;
	mCellY=cellY;
	mFlag=f;
}

void StaticObject::setFlag( Flag f ){
	mFlag = f;
}

void StaticObject::clearFlag(){
	mFlag = FLAG_NOTHING;
}

bool StaticObject::checkFlag( Flag f ){
	return  ( mFlag == f );
}

void StaticObject::draw( const Image* image ) const {

	int InnerX = convertCellToInner(mCellX);
	int InnerY = convertCellToInner(mCellY);

	switch(mFlag){
	case FLAG_NOTHING:
		image->draw(InnerX,InnerY,48,32,16,16);
		break;
	case FLAG_WALL:
		image->draw(InnerX,InnerY,16,32,16,16);
		break;
	case FLAG_CLOSEDGATE:
		image->draw(InnerX,InnerY,64,48,16,16);
		break;
	case FLAG_OPENEDGATE:
		image->draw(InnerX,InnerY,48,48,16,16);
		break;
	case FLAG_CLOSEDBOX:
		image->draw(InnerX,InnerY,80,48,16,16);
		break;
	case FLAG_OPENEDBOX:
		image->draw(InnerX,InnerY,96,48,16,16);
		break;
	case FLAG_PORTAL:
		image->draw(InnerX,InnerY,112,48,16,16);
		break;
	default:
		break;

	}
}
