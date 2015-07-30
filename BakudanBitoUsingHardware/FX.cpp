#include "FX.h"
#include "Vector2.h"
#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include "Image.h"
#include "Magic.h"

namespace {
static const int SCALE = 16000;
static const int HALF_SIZE = 8000;
static const int FLYINGSPEED = 3000;
static const int DELTA = 5000;

//网格单位转换到内部単位
int convertCellToInner( int x ){
	return x * SCALE + HALF_SIZE;
}
//从内部単位到像素単位
int convertInnerToPixel( int x ){
	return  ( x - HALF_SIZE + 500  ) / 1000;
}

//从内部单位到网格单位
int convertInnerlToCell( int x ){
	return x / SCALE;
}

};

FX::FX(void):
mTag( INACTIVE ),
mDirection( -1 ),
mLastTime( 0 ){
	mCell =  Vector2( 0 , 0 );
	mIsActived=false;
	pM=0;
	mCount=-1;

}

void FX::set(const Vector2 &Cell, int Direction, int LastTime, FX::TAG Tag){
	if(LastTime<=0||Direction<0){
		mIsActived=false;
		return;
	}
	mLastTime=LastTime;
	pM=Magic::Instance();
	mIsActived=true;
	mTag=Tag;
	mDirection=Direction;
	GameLib::Framework f=GameLib::Framework::instance();
	if(Tag==ILLUSION){
		switch(mDirection){
		case 0:
			mCell.y=Cell.y-SCALE*(pM->getScope(Magic::MINOR_ILLUSION).y);
			break;
		case 1:
			mCell.y=Cell.y+SCALE*(pM->getScope(Magic::MINOR_ILLUSION).y);
			break;
		case 2:
			mCell.x=Cell.x-SCALE*(pM->getScope(Magic::MINOR_ILLUSION).x);
			break;
		case 3:
			mCell.x=Cell.x+SCALE*(pM->getScope(Magic::MINOR_ILLUSION).x);
			break;
		default:break;
		}
	}else{
		mCell = Cell;
	}
		
			
	mInner=Vector2(convertCellToInner(static_cast<int>(Cell.x)),convertCellToInner(static_cast<int>(Cell.y)));
	mCount=0;
	pM=0;
}

void FX::inactive(){
	mCell=mInner=Vector2(0,0) ;
	mTag=INACTIVE;
	mDirection=-1;
	mLastTime=-1;
	mCount=0;
	mIsActived=false;
	pM=0;		
}

void FX::update(){
	GameLib::Framework f = GameLib::Framework::instance();
	mCount++;
	if(mCount>= mLastTime||!mIsActived){
		inactive();
		return;
	}
	switch(mTag){	
	case INACTIVE:
		return;	
	case ILLUSION:break;
	case FIRE:break;
	case LIGHTNTING:break;
	case LASER_HOR:break;
	case LASER_VER:break;
	case FIREBALL:
		switch(mDirection){
			case 0:
				mInner.y-=DELTA;
				break;
			case 1:
				mInner.y+=DELTA;
				break;
			case 2:
				mInner.x-=DELTA;
				break;
			case 3:
				mInner.x+=DELTA;
				break;
			default:break;
		}
		break;
	case FLASH_HOR:break;
	case FLASH_VER:break;
	default:break;
	}
	
	return;
}

void FX::draw( const Image* image ){
		GameLib::Framework f = GameLib::Framework::instance();
	if(!mIsActived){
		inactive();
		return;
	}
	int dstX=static_cast<int>(mInner.x);
	int dstY=static_cast<int>(mInner.y);
	pM=Magic::Instance();
	int srcX,srcY;
	srcX=srcY=0;
	switch(mTag){	
	case INACTIVE:
		return;	
	case ILLUSION:
		srcX=0;
		srcY=0;
		image->draw(dstX,dstY,srcX,srcY,16,16);
		break;
	case FIRE:
		srcX=48;
		srcY=16;
		image->draw(dstX,dstY,srcX,srcY,16,16);
		break;
	case LIGHTNTING:
		srcX=64;
		srcY=32;
		image->draw(dstX,dstY,srcX,srcY,16,16);
		break;
	case LASER_HOR:
		srcX=112;
		srcY=16;
		if(mDirection==2){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX-(SCALE*j),dstY,srcX,srcY,16,16);
			}
		}else if(mDirection==3){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX+(SCALE*j),dstY,srcX,srcY,16,16);
			}
		}
		break;
	case LASER_VER:
		srcX=96;
		srcY=16;
		if(mDirection==0){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX,dstY-(SCALE*j),srcX,srcY,16,16);
			}
		}else if(mDirection==1){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX,dstY+(SCALE*j),srcX,srcY,16,16);
			}
		}
		break;
	case FIREBALL:
		srcX=48;
		srcY=16;
		image->draw(dstX,dstY,srcX,srcY,16,16);
		break;
	case FLASH_HOR:
		srcX=80;
		srcY=16;
		if(mDirection==2){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX-(SCALE*j),dstY,srcX,srcY,16,16);
			}
		}else if(mDirection==3){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX+(SCALE*j),dstY,srcX,srcY,16,16);
			}
		}
		break;
	case FLASH_VER:
		srcX=64;
		srcY=16;
		if(mDirection==0){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX,dstY-(SCALE*j),srcX,srcY,16,16);
			}
		}else if(mDirection==1){
			for(int j=0;j<pM->getScope(Magic::LANTERNLIGHT).y;++j){
					image->draw(dstX,dstY+(SCALE*j),srcX,srcY,16,16);
			}
		}
		break;
	default:break;
	}
	pM=0;
	return;
}

Vector2 FX::getInner()const{
	return mInner;
}

bool FX::isActived()const{
	return mIsActived;
}

FX::~FX(void)
{	
}

