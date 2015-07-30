#include "Player.h"
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Game/DynamicObject.h"
#include "Game/StaticObject.h"
#include "Mechanism.h"
#include "Image.h"
#include "Pad.h"
#include "Vector2.h"
#include "Magic.h"
#include "Game/State.h"
#include "SoundManager.h"
#include "Wall.h"
#include "Rect.h"

using namespace GameLib;

namespace {

//適当的参数集合

static const int HALF_SIZE = 8000;
static const int SCALE = 16000;
static const int NORMALSPEED = 1000;
static const int MAXAP = 720;
static const int RECOVER = 30;
static const int ITEMCAST = 30;
static const int APEXPENDPS = 12;
static const int APRECOVERPS = 4;
static const int ANIMEMOVE = 30;
static const int HALFMOVE = 15;
static const int ANIMERUN = 20;
static const int HALFRUN = 10;
static const int BUFFTIME = 120;
static const int LAP = 360;
static const int MAP = 240;
static const int SAP = 120;

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
Player::Player( ) : 
mType( TYPE_NONE ),
mX( -1 ),
mY( -1 ),
mDirectionX( 0 ),
mDirectionY( 0 ),
mDirection( 1 ),
mSpeed( -1 ){
	mStatus = Player::UNCONTROLLABLE;	
	mCurrentMagic = Magic::BURNING_HANDS;
	mCurrentItem = AppStatus::MATCH;
	mBurningCount = mSpeedUpCount = mSlowdownCount = mFettersCount = mFlammableCount =mAnimeCount= 0;
}

Player::~Player(){
	SAFE_DELETE_ARRAY( mMagic );
	pAS = 0;
}

void Player::set( int x, int y, Type type ,State* s ){
	//内部座標値変換
	mX = convertCellToInner( x );
	mY = convertCellToInner( y );
	mType = type;
	mSpeed = NORMALSPEED;//速度。単位是内部単位/帧
	mAP = MAXAP;
	pAS = AppStatus::Instance();
	mStatus = Player::IDLE;
	mHP = pAS->getMaxHP();
	mBuff = Player::NORMAL;
	isNewMagic = false;
	mMagic = new Magic[ MAXMAGIC ];
	pState = s;
	pAS = 0;
}

void Player::handleInput( ){
	  /*UNCONTROLLABLE,
		IDLE,
		MOVE,
		RUN,
		CAST,*/

}

void Player::update(){
		if ( isDead() ){ //死亡。结束。
			return;
		}
		//Framework f = Framework::instance();
		if ( mStatus == UNCONTROLLABLE ){
			if( checkBuff( BEATBACK )){
				changeSpeed( 0.5 );
			} else if( checkBuff( FETTERS )){
				++mFettersCount;
				if( mFettersCount > BUFFTIME ){
					mFettersCount = 0;
					resetBuff( FETTERS );
				}
			} else {
				mStatus = IDLE;
				changeSpeed( 1.0 );
			}		
		}
		if( checkBuff( SLOWDOWN )){
			changeSpeed( 0.5 );
			mSlowdownCount++;
			if( mSlowdownCount >= BUFFTIME ){
				mSlowdownCount = 0;
				resetBuff( SLOWDOWN );
				changeSpeed( 1.0 );
			}
		}
		if( checkBuff( SPEEDUP )){
			changeSpeed( 2.0 );
			mSpeedUpCount++;
			if( mSpeedUpCount >= BUFFTIME ){
				mSpeedUpCount = 0;
				resetBuff( SPEEDUP );
				changeSpeed( 1.0 );
			}
		}
		if( checkBuff( FLAMMABLE )){
			mFlammableCount++;
			if( mFlammableCount >= BUFFTIME ){
				mFlammableCount = 0;
				resetBuff( FLAMMABLE );
			}
		}
		if( checkBuff( BURNING )){
			mBurningCount++;
			if( mBurningCount == 60 ){
				applyDamage( 1 );
			}
			if( mBurningCount >= BUFFTIME ){
				applyDamage( 1 );
				mBurningCount = 0;
				resetBuff( BURNING );
			}
		}

		//現在的中心取得
		int x = convertInnerToCell(mX);
		int y = convertInnerToCell(mY);

		//从墙壁列表中查找这个以此为中心的方块并存储于数列中
		int wallsX[ 9 ];
		int wallsY[ 9 ];
		int wallNumber = 0;
		
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				int tx = x + i - 1;
				int ty = y + j - 1;
				const Wall& w = pState->mWalls( tx, ty );
				if ( w.checkFlag( Wall::PLU )){ //若是墙壁
					wallsX[ wallNumber ] = x + i - 1;
					wallsY[ wallNumber ] = y + j - 1;
					++wallNumber;				
				}
			}
		}

		//过渡墙壁列表的移動処理
		move( wallsX, wallsY, wallNumber );
	
		for( int i = 0; i < MAXMAGIC; ++ i){	
			mMagic[mMagicIndex].work();
			mMagicIndex = (( mMagicIndex + 1 )>= MAXMAGIC )?( 0 ):( mMagicIndex + 1);
		}
		Framework f = Framework::instance();
		f.drawDebugString(3,3,Magic::getName(mCurrentMagic).c_str());
		char s[10],p[10]; 
		_itoa(mHP,s,10);_itoa(mAP,p,10);
		f.drawDebugString(3,1,"HP:");f.drawDebugString( 7,1,s );
		f.drawDebugString(3,2,"AP:");f.drawDebugString( 7,2,p );
		
}

Vector2 Player::getForward(){
	Vector2 forward;
	switch( mDirection ){
	case 0:
		forward = Vector2( mX, mX - SCALE );
		break;
	case 1:
		forward = Vector2( mX, mX + SCALE );
		break;
	case 2:
		forward = Vector2( mX - SCALE , mX );
		break;
	case 3:
		forward = Vector2( mX+ SCALE , mX );
		break;
	default:	
		forward = Vector2( mX, mY );
		break;
	}
	return forward;
}

void Player::select(){
	if(isNewMagic){
		mSelect = getForward();
		isNewMagic = false;
	}
	bool isHorizon = mDirection > 2 ? true : false;
	Rect r = Magic::getScope( mCurrentMagic, isHorizon );
	Vector2 m = Vector2( mX, mY );
	switch( mDirection ){
		case 0:
			r = Rect( m + Vector2( - 0.5 * r.getWidth(), - r.getHeight()), r.getWidth(), r.getHeight());
			if( Pad::isOn( Pad::U )){
				mSelect.y = (mSelect.y < r.getOriginVector().y) ?( r.getOriginVector().y ):( mSelect.y - NORMALSPEED);
			}
			if( Pad::isOn( Pad::D )){
				mSelect.y = ( mSelect.y >( r.getOriginVector().y + r.getHeight()))? ( r.getOriginVector().y + r.getHeight()) :( mSelect.y + NORMALSPEED);
			}
			if( Pad::isOn( Pad::L )){
				mSelect.x = (mSelect.x < r.getOriginVector().x )?( r.getOriginVector().x ):( mSelect.x - NORMALSPEED);
			}
			if( Pad::isOn( Pad::R )){
				mSelect.x = ( mSelect.x >( r.getOriginVector().x + r.getHeight()))? ( r.getOriginVector().x + r.getWidth()) :( mSelect.y + NORMALSPEED);
			}
			break;
		case 1:
			r = Rect( m + Vector2( - 0.5 * r.getWidth(),  r.getHeight()), r.getWidth(), r.getHeight());
			if( Pad::isOn( Pad::U )){
				mSelect.y = (mSelect.y < r.getOriginVector().y) ?( r.getOriginVector().y ):( mSelect.y - NORMALSPEED);
			}
			if( Pad::isOn( Pad::D )){
				mSelect.y = ( mSelect.y >( r.getOriginVector().y + r.getHeight()))? ( r.getOriginVector().y + r.getHeight()) : (mSelect.y + NORMALSPEED);
			}
			if( Pad::isOn( Pad::L )){
				mSelect.x =( mSelect.x < r.getOriginVector().x )? (r.getOriginVector().x ):( mSelect.x - NORMALSPEED);
			}
			if( Pad::isOn( Pad::R )){
				mSelect.x = ( mSelect.x >( r.getOriginVector().x + r.getHeight()))? ( r.getOriginVector().x + r.getWidth()) :( mSelect.y + NORMALSPEED);
			}
			break;
		case 2:
			r = Rect( m + Vector2( - r.getWidth(), - 0.5 * r.getHeight()), r.getWidth(), r.getHeight());
			if( Pad::isOn( Pad::U )){
				mSelect.y = (mSelect.y < r.getOriginVector().y )?( r.getOriginVector().y ): (mSelect.y - NORMALSPEED);
			}
			if( Pad::isOn( Pad::D )){
				mSelect.y = ( mSelect.y >( r.getOriginVector().y + r.getHeight()))? ( r.getOriginVector().y + r.getHeight()) :( mSelect.y + NORMALSPEED);
			}
			if( Pad::isOn( Pad::L )){
				mSelect.x = (mSelect.x < r.getOriginVector().x )? (r.getOriginVector().x ):( mSelect.x - NORMALSPEED);
			}
			if( Pad::isOn( Pad::R )){
				mSelect.x = ( mSelect.x >( r.getOriginVector().x + r.getHeight()))? ( r.getOriginVector().x + r.getWidth()) : (mSelect.y + NORMALSPEED);
			}
			break;
		case 3:
			r = Rect( m + Vector2(  r.getWidth(), - 0.5 *  r.getHeight()), r.getWidth(), r.getHeight());
			if( Pad::isOn( Pad::U )){
				mSelect.y =( mSelect.y < r.getOriginVector().y )? (r.getOriginVector().y ): (mSelect.y - NORMALSPEED);
			}
			if( Pad::isOn( Pad::D )){
				mSelect.y = ( mSelect.y >( r.getOriginVector().y + r.getHeight()))? ( r.getOriginVector().y + r.getHeight()) : (mSelect.y + NORMALSPEED);
			}
			if( Pad::isOn( Pad::L )){
				mSelect.x = (mSelect.x < r.getOriginVector().x )?( r.getOriginVector().x ): (mSelect.x - NORMALSPEED);
			}
			if( Pad::isOn( Pad::R )){
				mSelect.x = ( mSelect.x >( r.getOriginVector().x + r.getHeight()))? ( r.getOriginVector().x + r.getWidth()) : (mSelect.y + NORMALSPEED);
			}
		break;
		default:
			break;
	}
}	

void Player::draw( const Image* image ) const {
	if ( isDead() ){
		return;//draw dead body
	}
	//内部座標变换到像素座標(+500是四捨五入)
	int dstX = mX;
	int dstY = mY;
	//确认画像裁剪位置
	int srcX, srcY;
	srcX = srcY = -1;
	bool isMove=( mStatus == MOVE )?true:false;
	bool isRun=( mStatus == RUN )?true:false;
	bool isAntherFrame = false;
	if(isMove){
		isAntherFrame = ( mAnimeCount > HALFMOVE )?true:false;
	} else if(isRun){
		isAntherFrame = ( mAnimeCount > HALFRUN )?true:false;
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
			srcX = isAntherFrame?80:96;
			srcY = isAntherFrame?0:0;
			break;
		case 3:
			srcX = isAntherFrame?112:128;
			srcY = isAntherFrame?0:0;
			break;
				break;
		default: 
			break;
	}
	image->draw( dstX, dstY, srcX, srcY, 16, 16 );
}

void Player::move( const int* wallsX, const int* wallsY, int wallNumber ){
	
	//移動量取得
	int dx, dy;
	getVelocity( &dx, &dy );
	
	//X,Y分別移動的時侯，检查是否碰撞
	int movedX = mX + dx;
	int movedY = mY + dy;
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

bool Player::isIntersectWall( int x, int y, int wallX, int wallY ){
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

bool Player::isIntersectWall( int wallX, int wallY ){
	return isIntersectWall( mX, mY, wallX, wallY );
}

void Player::getVelocity( int* dx, int* dy ) const {

	//取得方向
	getDirection( dx, dy );
	//速度計算
	*dx = *dx * mSpeed;
	*dy = *dy * mSpeed;
}

void Player::getDirection( int* dx, int* dy ) const {
	Framework f = Framework::instance();
	*dx = *dy = 0;	
	if( checkBuff( FETTERS )){
		return;
	}
	if( !checkBuff( BEATBACK )){
		if ( Pad::isOn( Pad::L) ){
			*dx = -1;
		}else if ( Pad::isOn( Pad::R) ){
			*dx = 1;
		}
		if ( Pad::isOn( Pad::U) ){
			*dy = -1;
		}else if ( Pad::isOn( Pad::D) ){
			*dy = 1;
		}
	} else {
		switch( mDirection ){
			case 0:
				*dy = 1;
				break;
			case 1:
				*dy = -1;
				break;
			case 2:
				*dx = 1;
				break;
			case 3:
				*dx = -1;
				break;
			default:
				break;
		}
	}
}


bool Player::isIntersect( const DynamicObject& o ) const {
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

Vector2 Player::getInner(void) const{
	return Vector2( mX, mY );
}

Vector2 Player::getCell(void) const {
	int x = convertInnerToCell( mX );
	int y = convertInnerToCell( mY );
	return Vector2( x, y );
}

int Player::getType() const{
	return mType;
}

void Player::die(){
	mType = TYPE_NONE;
}

bool Player::isDead() const {
	return ( mType == TYPE_NONE );
}

void Player::applyHeal( int heal ){
	mHP += heal;
	pAS = AppStatus::Instance();
	int Max = pAS->getMaxHP();
	if( mHP > Max ){
		mHP = Max;
	}
	pAS = 0;
}

void Player::applyDamage(int damage){
	mHP -= damage;
	if( mHP <= 0 ){
		die();
	}
}

void Player::setBuff( unsigned b ){
	if(( b == Player::BURNING )&&( checkBuff( Player::FLAMMABLE ))){
		resetBuff( FLAMMABLE );
		resetBuff( BURNING );
		applyDamage( 3 );
	} 
	if( b == BURNING ){
		applyDamage( 1 );
	}
	mBuff |= b;
}



void Player::resetBuff( unsigned b ){
	mBuff &= ~b;
}

bool Player::checkBuff( unsigned b )const{
	return ( mBuff & b ) ? true : false;
}

bool Player::useItem( AppStatus::ITEM i ){
	pAS = AppStatus::Instance();
	bool result = false;
	switch( i ){
		case AppStatus::KUSURIBIG:
			if( pAS->reduceItem( i )){
				applyHeal(6);
			}else{
				pAS = 0;
				result = false;
			}
			pAS = 0;
			result = true;
			break;
		case AppStatus::KUSURI:
			if( pAS->reduceItem( i )){
				applyHeal(3);
			}else{
				pAS = 0;
				result = false;
			}
			pAS = 0;
			result = true;
			break;
		default:
			pAS = 0;
			result = false;
			break;
	}
	return result;
}

Magic::TYPE Player::getCurrentMagic() const{
	return Player::mCurrentMagic;
}

AppStatus::ITEM Player::getCurrentItem() const{
	return Player::mCurrentItem;
}

void Player::nextMagic(){
	switch(mCurrentMagic){
		case Magic::RAY_OF_FROST:
				mCurrentMagic = Magic::BURNING_HANDS;
				break;
		case Magic::BURNING_HANDS:
				mCurrentMagic = Magic::LONGSTRIDER;
				break;
		case Magic::LONGSTRIDER:
				mCurrentMagic = Magic::FLAMING_SPHERE;
				break;
		case Magic::FLAMING_SPHERE:
				mCurrentMagic = Magic::BLINK;
				break;
		case Magic::BLINK:
				mCurrentMagic = Magic::RAY_OF_FROST;
				break;
		default:
			break;
	}
}

void Player::lastMagic(){
	switch(mCurrentMagic){
		case Magic::RAY_OF_FROST:
				mCurrentMagic = Magic::BLINK;
				break;
		case Magic::BURNING_HANDS:
				mCurrentMagic = Magic::RAY_OF_FROST;
				break;
		case Magic::LONGSTRIDER:
				mCurrentMagic = Magic::BURNING_HANDS;
				break;
		case Magic::FLAMING_SPHERE:
				mCurrentMagic = Magic::LONGSTRIDER;
				break;
		case Magic::BLINK:
				mCurrentMagic = Magic::FLAMING_SPHERE;
				break;
		default:
			break;
	}
}

void Player::nextItem(){
if(mCurrentItem== AppStatus::KUSURI ){
		mCurrentItem = AppStatus::KUSURIBIG;}else{mCurrentItem=AppStatus::KUSURI;}
}

void Player::lastItem(){
if(mCurrentItem== AppStatus::KUSURI ){
		mCurrentItem = AppStatus::KUSURIBIG;}else{mCurrentItem=AppStatus::KUSURI;}
}

void Player::changeSpeed( double n ){
	mSpeed = static_cast< int >( n * NORMALSPEED );
}
