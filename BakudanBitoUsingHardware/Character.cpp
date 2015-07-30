#include "Character.h"
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Game/DynamicObject.h"
#include "Game/StaticObject.h"
#include "Image.h"
#include "Pad.h"
#include "Vector2.h"
#include "Magic.h"
#include "Game/State.h"
#include "SoundManager.h"
#include "Rect.h"

using namespace GameLib;

namespace {

//適当的参数集合
static const int S_SIZE = 7500;
static const int HALF_SIZE = 8000;
static const int SCALE = 16000;
static const int NORMALSPEED = 1000;
static const int MAXAP = 150;
static const int HITRECOVER = 15;
static const int ITEMCAST = 30;
static const int ANIMEMOVE = 30;
static const int HALFMOVE = 15;
static const int ANIMERUN = 20;
static const int HALFRUN = 10;
static const int BUFFTIME = 120;


//转换到内部単位
int convertCellToInner( int x ){
	return x *  SCALE + HALF_SIZE;
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
Character::Character( ) : 
mType( TYPE_NONE ),
mX( -1 ),
mY( -1 ),
mDirection( 1 ),
mSpeed( -1 ){
	mStatus = Character::RECOVER;	
	mRecoverCount = mBuffCount =mAnimeCount= 0;
	mDirection=1;
	mRotation=2;
}

Character::~Character(){
	pM=0;
	pAS = 0;
	pState=0;
}

void Character::set( int x, int y, State* s ){
	//内部座標値変換
	mX = convertCellToInner( x );
	mY = convertCellToInner( y );
	mType = TYPE_CHARACTER;
	mSpeed = NORMALSPEED;//速度。単位是内部単位/帧	
	mBuff = Character::NORMAL;
	pState = s;
	mStatus = Character::IDLE;
	mAP = MAXAP;
	pAS = AppStatus::instance();	
	mHP = pAS->getMaxHP();
	pAS = 0;
}

void Character::handleDirectionAndRotation(){
	//Direction means which orientation the character facing like UDLR by 0123
	//Rotation means the more subdivided one like N-NE-E-SE-S-SW-W-NW by 01234567
	if((Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))){
		mRotation = 0;
		mDirection= 0;
	}else if((!Pad::isOn(Pad::U))&&(Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))){
		mRotation = 4;
		mDirection = 1;
	}else if((!Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))){
		mRotation = 6;
		mDirection = 2;
	}else if((!Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(Pad::isOn(Pad::R))){
		mRotation = 2;
		mDirection = 3;
	}else if((Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(Pad::isOn(Pad::R))){
		mRotation = 1;
		mDirection = 3;
	}else if((!Pad::isOn(Pad::U))&&(Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(Pad::isOn(Pad::R))){
		mRotation = 3;
		mDirection= 3;
	}else if((!Pad::isOn(Pad::U))&&(Pad::isOn(Pad::D))&&(Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))){
		mRotation = 5;
		mDirection = 2;
	}else if((Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))){
		mRotation = 7;
		mDirection = 2;
	}else{
		mStatus=IDLE;
		changeSpeed(1.0f);
	}

}

void Character::handleInput( ){
	pAS = AppStatus::instance();
	pM=Magic::Instance();

	if((Pad::isTriggered(Pad::MAGIC_D))&&(!Pad::isTriggered(Pad::MAGIC_U))){
		pAS->nextMagic();
	}else
	if((!Pad::isTriggered(Pad::MAGIC_D))&&(Pad::isTriggered(Pad::MAGIC_U))){
		pAS->lastMagic();
	}

	if((Pad::isTriggered(Pad::ITEM_D))&&(!Pad::isTriggered(Pad::ITEM_U))){
		pAS->nextItem();
	}else
	if((!Pad::isTriggered(Pad::ITEM_D))&&(Pad::isTriggered(Pad::ITEM_U))){
		pAS->lastItem();
	}

	if(Pad::isTriggered(Pad::ITEM)){
		if(pAS->reduceItem(pAS->getCurrentItem())){
			mStatus=RECOVER;
			changeSpeed(1.0f);
			mRecoverCount=0;
			useItem();
		}
	}
	
	switch(mStatus){
		case RECOVER:{
			if( mRecoverCount < HITRECOVER ){
				mRecoverCount ++;
			}else{
				mRecoverCount = 0;
				mStatus = IDLE;
			}
			break;
			}
		case IDLE:{
			if(((Pad::isOn(Pad::U))||(Pad::isOn(Pad::D))||(Pad::isOn(Pad::L))||(Pad::isOn(Pad::R)))&&(!Pad::isOn(Pad::RUN))){
				mStatus = MOVE;
				changeSpeed(1.0f);
			}else if(((Pad::isOn(Pad::U))||(Pad::isOn(Pad::D))||(Pad::isOn(Pad::L))||(Pad::isOn(Pad::R)))&&(Pad::isOn(Pad::RUN))){
				mStatus = RUN;
				handleDirectionAndRotation();
				changeSpeed( 1.5f );
			}
			if(Pad::isOn(Pad::MAGIC)){
				mStatus=CAST;
				changeSpeed(1.0f);
			}							
			if(Pad::isTriggered(Pad::ITEM)){
				mRecoverCount = 0;
				mStatus = RECOVER;	
			}
			break;	  
			}
		case MOVE:{
				handleDirectionAndRotation();			
			if((!Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))){
				mStatus=IDLE;
				changeSpeed(1.0f);
			}
			if(((Pad::isOn(Pad::U))||(Pad::isOn(Pad::D))||(Pad::isOn(Pad::L))||(Pad::isOn(Pad::R)))&&(Pad::isOn(Pad::RUN))){
				mStatus=RUN;
				changeSpeed(1.5f);
			}
			if(Pad::isOn(Pad::MAGIC)){
				mStatus=CAST;
				changeSpeed(1.0f);
			}
			break;	  
				  }
		case RUN:{
			handleDirectionAndRotation();	
			if((!Pad::isOn(Pad::U))&&(!Pad::isOn(Pad::D))&&(!Pad::isOn(Pad::L))&&(!Pad::isOn(Pad::R))&&(!Pad::isOn(Pad::RUN))){
				mStatus=IDLE;
				changeSpeed(1.0);
			}else if(((Pad::isOn(Pad::U))||(Pad::isOn(Pad::D))||(Pad::isOn(Pad::L))||(Pad::isOn(Pad::R)))&&(!Pad::isOn(Pad::RUN))){
				mStatus=MOVE;
				changeSpeed(1.0f);
			}
			if(Pad::isOn(Pad::MAGIC)){
				mStatus=CAST;
				changeSpeed(1.0f);
			}
			break;	  
		case CAST:{
			handleDirectionAndRotation();	
			if(!Pad::isOn(Pad::MAGIC)){
				mStatus= IDLE;
				changeSpeed(1.0f);
			}else{
				mCastCount++;
				if(mCastCount>=pM->getCastTime(pAS->getCurrentMagic())){
						if(mAP>=Magic::getCastTime(pAS->getCurrentMagic())){
						mAP-=Magic::getCastTime(pAS->getCurrentMagic());
						pState->instanceMagic( getCell() ,mDirection,pAS->getCurrentMagic(),DynamicObject::TYPE_CHARACTER);
						mStatus=RECOVER;
						changeSpeed(1.0f);
						mCastCount=0;
					}
				}
			}
			break;	  
				  }
		default:
			break;
		}
	}
	pAS=0;
	pM=0;
}

void Character::update(){
		
		if ( isDead() ){ //死亡。结束。
			return;
		}
		Framework f = Framework::instance();
		pAS=AppStatus::instance();
		f.drawDebugString(3,3,Magic::getName(pAS->getCurrentMagic()).c_str());
		f.drawDebugString(1,5,AppStatus::getItemName(pAS->getCurrentItem()).c_str());
		
		char s[10],p[10],a[10]; 
		int tempAP=static_cast<int>(mAP/150.0*100);
		_itoa(mHP,s,10);_itoa(tempAP,p,10);_itoa(pAS->getItemNum(pAS->getCurrentItem()),a,10);
		f.drawDebugString(3,7,a);
		f.drawDebugString(3,1,"HP:");f.drawDebugString( 7,1,s );
		f.drawDebugString(3,2,"AP:");f.drawDebugString( 7,2,p );
	
		updateBuff();

		if((mStatus!=MOVE)&&(mStatus!=RUN)){
			if(mStatus!=RUN){
				mAP++;
				if(mAP>=MAXAP){
					mAP=MAXAP;
				}
			}
			return;
		}else{
			mAnimeCount++;
			if(mStatus==RUN){
				mAP--;
				if(mAP<=0){
					mAP=0;
					mStatus=MOVE;
					changeSpeed(1.0f);
				}
			}
			//現在的中心取得
			int x = convertInnerToCell(mX);
			int y = convertInnerToCell(mY);
			StaticObject& b =pState->mSOs(static_cast<int>(getCellForward().x),static_cast<int>(getCellForward().y));
			if(b.checkFlag(StaticObject::FLAG_CLOSEDBOX)){
				pAS->addItem(pAS->getCurrentItem());
				//UI->broadcast("You got a new item.");
				b.clearFlag();
			}
			//从墙壁列表中查找这个以此为中心的方块并存储于数列中
			int wallsX[ 9 ];
			int wallsY[ 9 ];
			int wallNumber = 0;
			
			for ( int i = 0; i < 3; ++i ){
				for ( int j = 0; j < 3; ++j ){
					int tx = x + i - 1;
					int ty = y + j - 1;
					StaticObject& w = pState->mSOs( tx, ty );
					if ( w.checkFlag( StaticObject::FLAG_WALL )||w.checkFlag(StaticObject::FLAG_CLOSEDGATE)||w.checkFlag(StaticObject::FLAG_CLOSEDBOX)){
						wallsX[ wallNumber ] = tx;
						wallsY[ wallNumber ] = ty;
						++wallNumber;				
					}
				}
			}
			
			//过渡墙壁列表的移動処理
			move( wallsX, wallsY, wallNumber );	

		}	
		pAS=0;
}


Vector2 Character::getCellForward(){
	Vector2 forward;
	int x = convertInnerToCell(mX);
	int y = convertInnerToCell(mY);
	switch( mDirection ){
	case 0:
		forward = Vector2( x, y - 1 );
		break;
	case 1:
		forward = Vector2( x, y + 1 );
		break;
	case 2:
		forward = Vector2( x - 1 , y );
		break;
	case 3:
		forward = Vector2( x+ 1 , y );
		break;
	default:	
		forward = Vector2( x, y );
		break;
	}
	return forward;
}

void Character::updateBuff(){
	switch(mBuff){
	case NORMAL:
		mBuffCount=0;
		break;
	case BURNING:
		//pUI->broadcast("You're burning and losing HP.");
		mBuffCount++;
		if(mBuffCount==(BUFFTIME/2)){
			applyDamage(1);
		}
		if(mBuffCount>=BUFFTIME){
			applyDamage(1);
			mBuff=NORMAL;
			mBuffCount=0;
		}
		break;
	case SPEEDUP:
		//pUI->broadcast("Speeeed-up!Zip.");
		mBuffCount++;
		changeSpeed(3.0f);
		if(mBuffCount>=BUFFTIME){
			mBuff=NORMAL;
			mBuffCount=0;
			changeSpeed(1.0f);
		}
		break;
	case SLOWDOWN:
		//pUI->broadcast("You're Slowing down.");
		mBuffCount++;
		changeSpeed(0.5f);
		if(mBuffCount>=BUFFTIME){
			mBuff=NORMAL;
			mBuffCount=0;
			changeSpeed(1.0f);
		}
		break;
	default:
		break;
	}
}

void Character::draw( const Image* image ) {
	if ( isDead() ){
		return;
	}
	//内部座標变换到像素座標(+500是四捨五入)
	int dstX = mX;
	int dstY = mY;
	//确认画像裁剪位置
	int srcX, srcY;
	srcX = srcY = -1;
	bool isAntherFrame = false;
	if( mStatus == MOVE ){
		isAntherFrame = ( mAnimeCount > HALFMOVE )?true:false;
		if( mAnimeCount >= ANIMEMOVE ){
			mAnimeCount = 0;
		}
	} else if( mStatus == RUN ){
		isAntherFrame = ( mAnimeCount > HALFRUN )?true:false;
		if( mAnimeCount >= ANIMERUN ){
			mAnimeCount = 0;
		}
	} 
	
	switch ( mDirection ){
	case 0:
			srcX = isAntherFrame?32:48;
			srcY = isAntherFrame?0:0;
			break;
		case 1:
			srcX = isAntherFrame?0:16;
			srcY = isAntherFrame?0:0;
			break;
		case 2:
			srcX = isAntherFrame?80:64;
			srcY = isAntherFrame?0:0;
			break;
		case 3:
			srcX = isAntherFrame?112:96;
			srcY = isAntherFrame?0:0;
			break;
				break;
		default: 
			break;
	}
	image->draw( dstX, dstY, srcX, srcY, 16, 16 );
}

void Character::move( const int* wallsX, const int* wallsY, int wallNumber ){
	
	//移動量取得
	double dx, dy;
	getVelocity( &dx, &dy );
	
	//X,Y分別移動的時侯，检查是否碰撞
	int movedX = mX + static_cast<int>(dx);
	int movedY = mY + static_cast<int>(dy);
	bool hitX = false;
	bool hitY = false;
	bool hit = false;
	for ( int i = 0; i < wallNumber; ++i ){
		if ( isIntersectWall( movedX, mY, wallsX[ i ], wallsY[ i ] ) ){
			hitX = hit = true;
		}
		if (  isIntersectWall( mX, movedY, wallsX[ i ], wallsY[ i ] ) ){
			hitY = hit = true;
		}
	}
	if ( hitX && !hitY ){
		mY = movedY; //只有Y也可以
	}else if ( !hitX && hitY ){
		mX = movedX; //只有X也可以
	}else{ //因为都没碰撞 所以普通处理
		for ( int i = 0; i < wallNumber; ++i ){
			if ( isIntersectWall( movedX, movedY, wallsX[ i ], wallsY[ i ] ) ){
				hit = true;
			}
		}
		if ( !hit ){
			mX = movedX;
			mY = movedY;
		}
	}
	
}

bool Character::isIntersectWall( int x, int y, int wallX, int wallY ){
	int wx = convertCellToInner( wallX );
	int wy = convertCellToInner( wallY );

	int al = x - S_SIZE; //left A
	int ar = x + S_SIZE; //right A
	int bl = wx - S_SIZE; //left B
	int br = wx + S_SIZE; //right B
	if ( ( al < br ) && ( ar > bl ) ){
		int at = y - S_SIZE; //top A
		int ab = y + S_SIZE; //bottom A
		int bt = wy - S_SIZE; //top B
		int bb = wy + S_SIZE; //bottom B
		if ( ( at < bb ) && ( ab > bt ) ){
			return true;
		}
	}
	return false;
}

bool Character::isIntersectWall( int wallX, int wallY ){
	return isIntersectWall( mX, mY, wallX, wallY );
}

void Character::getVelocity( double* dx, double* dy ) const {

	//取得方向
	getRotation( dx, dy );
	//速度計算
	double s=static_cast<double>(mSpeed);
	*dx = *dx * s;
	*dy = *dy * s;
}

void Character::getRotation( double* dx, double* dy ) const {
	switch( mRotation ){
		case 0:
			*dx=0.0;
			*dy=-1.0;
			break;
		case 1:
			*dx=0.71;
			*dy=-0.71;
			break;
		case 2:
			*dx=1.0;
			*dy=0.0;
			break;
		case 3:
			*dx=0.71;
			*dy=0.71;
			break;
		case 4:
			*dx=0.0;
			*dy=1.0;
			break;
		case 5:
			*dx=-0.71;
			*dy=0.71;
			break;
		case 6:
			*dx=-1.0;
			*dy=0.0;
			break;
		case 7:
			*dx=-0.71;
			*dy=-0.71;
			break;
		default:
			*dx=*dy=0.0;
			break;
		
	}
}

int Character::getAP()const{
	return mAP;
}

bool Character::isIntersect( const DynamicObject& o ) const {
	Vector2 v = o.getCell( );
	int al = mX - HALF_SIZE; //left A
	int ar = mX + HALF_SIZE; //right A
	int bl = static_cast<int>(v.x) - HALF_SIZE; //left B
	int br = static_cast<int>(v.x) + HALF_SIZE; //right B
	if ( ( al < br ) && ( ar > bl ) ){
		int at = mY - HALF_SIZE; //top A
		int ab = mY + HALF_SIZE; //bottom A
		int bt = static_cast<int>(v.y) - HALF_SIZE; //top B
		int bb = static_cast<int>(v.y) + HALF_SIZE; //bottom B
		if ( ( at < bb ) && ( ab > bt ) ){
			return true;
		}
	}
	return false;

}

Vector2 Character::getInner(void) const{
	return Vector2( mX, mY );
}

Vector2 Character::getCell(void) const {
	int x = convertInnerToCell( mX );
	int y = convertInnerToCell( mY );
	return Vector2( x, y );
}

int Character::getType() const{
	return mType;
}

void Character::die(){
	mType = TYPE_NONE;
}

bool Character::isDead() const {
	return ( mType == TYPE_NONE );
}

void Character::applyHeal( int heal ){
	mHP += heal;
	pAS = AppStatus::instance();
	int Max = pAS->getMaxHP();
	if( mHP > Max ){
		mHP = Max;
	}
	pAS = 0;
}

void Character::applyDamage(int damage){
	mHP -= damage;
	if( mHP <= 0 ){
		die();
	}
}

void Character::setBuff( Buff b ){
	mBuff = b;
}



void Character::resetBuff(){
	mBuff = NORMAL;
}

bool Character::checkBuff( Buff b )const{
	return  mBuff == b ;
}

void Character::useItem( ){
	pAS = AppStatus::instance();
	if( pAS->reduceItem(pAS->getCurrentItem())){
		switch(pAS->getCurrentItem()){
		case AppStatus::KEY:{
			Vector2 f=getCellForward();
			StaticObject& w = pState->mSOs( static_cast<int>(f.x), static_cast<int>(f.y ));	
			if(w.checkFlag(StaticObject::FLAG_CLOSEDGATE)){
				w.setFlag(StaticObject::FLAG_OPENEDGATE);
			}else{
				pAS->addItem(AppStatus::KEY);
			}
			break;
		}
		case AppStatus::BOOK:
			pAS->unlockMagic();
			break;
		case AppStatus::KUSURIBIG:
			applyHeal(6);
			break;
		case AppStatus::KUSURI:
			applyHeal(3);
		default:
			break;
		}
	}
	pAS=0;
}

void Character::changeSpeed( double n ){
	mSpeed = static_cast< int >( n * static_cast<double>(NORMALSPEED) );
}
