#include "Mechanism.h"
#include "Image.h"
#include "FX.h"
#include "GameLib/GameLib.h"
#include "Wall.h"
#include "Game/State.h"

namespace {
static const int SCALE = 16000;
static const int HALF_SIZE = 8000;
};

int Mechanism::convertCellToInner( int x ){
	return x * SCALE + HALF_SIZE;
}

Mechanism::Mechanism(void):
mFlags( 0 ),
mCellX( 0 ),
mCellY( 0 ),
mDirection( -1 )
{	
}

Mechanism::~Mechanism(void)
{
	Mechanism::mFlags = 0;
	mCellX = 0;
	mCellY = 0;
	mDirection = -1;
	SAFE_DELETE( wall );
	SAFE_DELETE( mFX0 );
	SAFE_DELETE( mFX1 );
	SAFE_DELETE( mFX2 );

}

void Mechanism::draw( int x , int y ,const Image* image ) const{
	int srcX = -1;	
	int srcY = -1;				
	 if( mFlags & FLAG_WEB ){
		 mFX0->draw( image );
		 return;
		}	
	/*	FLAG_DIECANDLE = ( 1 << 0 ),
		FLAG_LIGHTCANDLE = ( 1 << 1 ),
		FLAG_CLOSEDGATE = ( 1 << 2 ), 
		FLAG_OPENEDGATE = ( 1 << 3 ),
		FLAG_CLOSEDBOX = ( 1 << 4 ),
		FLAG_OPENEDBOX = ( 1 << 5 ),
		FLAG_PORTAL = ( 1 << 6 ),
		FLAG_WEB = ( 1 << 7 );*/
	if ( mFlags & FLAG_LIGHTCANDLE ){
		mFX0->draw(image);
		mFX1->draw(image);
		mFX2->draw(image);
			
	}else{
		if ( mFlags & FLAG_DIECANDLE ){
				switch( mDirection ){
				case 0://NONE
					srcX = 32;
					srcY = 48;
					break;
				case 1://UP
					srcX = 48; 
					srcY = 16;
					break;
				case 2://DOWN
					srcX = 48; 
					srcY = 16;
					break;
				case 3://LEFT
					srcX = 48; 
					srcY = 16;
					break;
				case 4://RIGHT
					srcX = 48; 
					srcY = 16;
					break;
				default:
					break;
				}
		}else if ( mFlags & FLAG_CLOSEDGATE ){
			srcX = 48; 
			srcY = 16;
		}else if ( mFlags & FLAG_OPENEDGATE ){
			srcX = 48; 
			srcY = 16;
		}else if ( mFlags & FLAG_CLOSEDBOX ){
			srcX = 48; 
			srcY = 16;
		}else  if( mFlags & FLAG_OPENEDBOX ){
			srcX = 48; 
			srcY = 16;
		}else  if( mFlags & FLAG_PORTAL ){
			srcX = 48; 
			srcY = 16;
		}else {
				srcX = 0; 
				srcY = 32;
		}
		image->draw( x*16, y*16, srcX, srcY, 16, 16 );
	}//else ( mFlags & FLAG_LIGHTCANDLE ){


}

void Mechanism::setFlag( unsigned f ){
	mFlags |= f;	
}

void Mechanism::setFlag( unsigned f ,int Direction ){
	if(f == FLAG_DIECANDLE || f == FLAG_LIGHTCANDLE ){
		mFlags |= f;
		if(Direction > 4 || Direction < 0){
		Direction = 0;
		}
		mDirection = Direction;
	}else{
		Mechanism::setFlag(f);
	}
}


bool Mechanism::isInstanced(void){
	if( mFlags == FLAG_DIECANDLE ){
		return false;
	} else if( mFlags == FLAG_LIGHTCANDLE ){
		return true;
	} else if( mFlags == FLAG_CLOSEDGATE ){
		return false;
	} else if( mFlags == FLAG_OPENEDGATE ){
		return true;
	} else if( mFlags == FLAG_CLOSEDBOX ){
		return false;
	} else if( mFlags == FLAG_OPENEDBOX ){
		return true;
	} else if( mFlags == FLAG_PORTAL ){
		return true;
	} else if( mFlags == FLAG_WEB ){
		return true;
	} else{
		return false;
	}
}

void Mechanism::setInstanced(bool bToSet){
	if(bToSet){
		switch (mFlags){
		case FLAG_DIECANDLE:{
			int iInnerX = convertCellToInner( mCellX );
			int iInnerY = convertCellToInner( mCellY );
			mFX0 = new FX();
			mFX1 = new FX();
			mFX2 = new FX();
			mFX0->add(Vector2( iInnerX, iInnerY ),Vector2(0.0, 1.0), 9999999, FX::LIGHT, FX::UP);//REDO
			mFX1->add(Vector2( iInnerX, iInnerY ),Vector2(0.0, 2.0), 9999999, FX::LIGHT, FX::UP);
			mFX2->add(Vector2( iInnerX, iInnerY ),Vector2(0.0, 3.0), 9999999, FX::LIGHT, FX::UP);
			}
			mFlags = FLAG_LIGHTCANDLE;
			break;
		case FLAG_CLOSEDGATE:
			mFlags = FLAG_OPENEDGATE;
			wall = new Wall( mCellX, mCellY, (unsigned)Wall::PLU );
			state->addWall( *wall );	
			break;
		case FLAG_CLOSEDBOX:
			mFlags = FLAG_OPENEDBOX;
			break;
		default:
			break;
		}
	} else{
		switch (mFlags){
		case FLAG_LIGHTCANDLE:
			mFX0->remove();
			mFX1->remove();
			mFX2->remove();
			break;
		case FLAG_OPENEDGATE:
			mFlags = FLAG_CLOSEDGATE;
			state->clearWall( wall->mCellX, wall->mCellY );
			break;
		case FLAG_OPENEDBOX:
			mFlags = FLAG_CLOSEDBOX;
			break;
		case FLAG_WEB:
			mFlags = 0;//clear all flag
		default:
			break;
		}
	}
	return;
}

void Mechanism::work(){
	switch (mFlags){
	case FLAG_DIECANDLE:		
		break;
	case FLAG_LIGHTCANDLE:	
		break;
	case FLAG_CLOSEDGATE:
		break;
	case FLAG_OPENEDGATE:
		break;
	case FLAG_CLOSEDBOX:
		//to open box,add an item to player by stage date //REDO
		mFlags = 0;
		mFlags = FLAG_OPENEDBOX;
		break;
	case FLAG_OPENEDBOX:
		break;
	case FLAG_PORTAL:
		break;//REDO
	case FLAG_WEB:
		//REDO slow down player
		break;
	default:
		return;
	}
}
void Mechanism::setFX(FX *pFX){
	mFX0 = pFX;
}

