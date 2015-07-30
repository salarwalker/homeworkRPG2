#include "Enemy.h"
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Game/DynamicObject.h"
#include "Game/StaticObject.h"
#include "Image.h"
#include "Vector2.h"
#include "Magic.h"
#include "Game/State.h"
#include "SoundManager.h"
#include "Rect.h"
#include "Character.h"
#include "math.h"

using namespace GameLib;

namespace {

	struct EnemyData{
		Enemy::Type type;
		int scoutDistance;
		int attackDistance;
		int attackRate;
		int speed;
		int CD;
		int HP;
		int EXP;
	};
static const int S_SIZE = 7500;
static const int HALF_SIZE = 8000;
static const int SCALE = 16000;
static const int RECOVER = 30;
static const int MAXMAGIC = 4;
static const int ANIMEMOVE = 30;
static const int HALFMOVE = 15;
static const int BUFFTIME = 120;
static const int CASTTIME = 90;

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

	static EnemyData ED[] = {
		//						s   a   ar  sp      cd      hp		exp
		{ Enemy::TYPE_SLIME,	5,	3,	30,	700,	120,	2,		1 },
		{ Enemy::TYPE_BOX,		3,	1,	60,	1200,	150,	5,		2 },
		{ Enemy::TYPE_SHADOW,	8,	5,	25,	900,	180,	10,		4 },
		{ Enemy::TYPE_BOSS,		10,	6,  20,	1000,	60,		20,		8 },
		{ Enemy::TYPE_NONE,		0,	0,	0,	0,		1000,	0,		0 },
	};

	const static bool EMTable[Enemy::TYPE_NONE][Magic::NONE]={
//					MINOR_ILLUSION	RAY_OF_FROST	SHOCKING_GRASP	LANTERNLIGHT	BURNING_HANDS	LONGSTRIDER	THUNDERWAVE	FLAMING_SPHERE	SCORCHING_RAY,
/*TYPE_SLIME	*/{	false,			true,			false,			true,			false,			false,		false,		false,			false },
/*TYPE_BOX		*/{	false,			false,			false,			false,			true,			false,		false,		true,			false },
/*TYPE_SHADOW	*/{	false,			true,			true,			true,			false,			true,		true,		false,			false },
/*TYPE_BOSS		*/{	false,			false,			true,			true,			true,			false,		false,		true,			true  },
	};

} //namespace{}

//尽量写入无效值。使其如果不调用set直接使用会出错。
Enemy::Enemy() : 
mType( Enemy::TYPE_NONE ),
mX( -1 ),
mY( -1 ),
mDirection( 1 ),
mSpeed( -1 ),
pState( 0 ){
	pAS = 0;
	mStatus = Enemy::IDLE;	
	mBuffCount = 0;
	mTarget=Vector2(0,0);
}

Enemy::~Enemy(){
	pAS = 0;
}

Enemy::Type Enemy::randomEnemy(){
	Framework f=Framework::instance();
	return static_cast<Enemy::Type>(f.getRandom(4));
}

void Enemy::set( int x, int y, Type type ,State* s ){
	//内部座標値変換
	mX = convertCellToInner( x );
	mY = convertCellToInner( y );
	mDirection = 1;
	mType = type;
	mSpeed = ED[ mType ].speed;//速度。単位是内部単位/帧
	mStatus = Enemy::IDLE;
	mHP = ED[ mType ].HP;
	mBuff = Enemy::NORMAL;
	pState = s;
	mCDCount = mAnimeCount=0;
}

void Enemy::think(){
	if( isDead() ){
		return;
	}
	Framework f = Framework::instance();
	int dis = getDistance();
	if( dis >= ED[ mType ].scoutDistance ){
		mDirection = getRotation();
		mStatus = IDLE;
	} else if( dis>=ED[ mType ].attackDistance ){
		if( f.getRandom( 100 ) < ED[ mType ].attackRate ){
			mStatus = ATTACK;
			mDirection = getRotation();
		} else {
			mStatus = HOLD;
		}	
	} else{
		mStatus = ATTACK;
		mDirection = getRotation();
	}
}

void Enemy::updateBuff(){
	switch(mBuff){
	case NORMAL:
		mBuffCount=0;
		break;
	case BURNING:
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
		mBuffCount++;
		changeSpeed(3.0f);
		if(mBuffCount>=BUFFTIME){
			mBuff=NORMAL;
			mBuffCount=0;
			changeSpeed(1.0f);
		}
		break;
	case SLOWDOWN:
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

void Enemy::changeSpeed( double n ){
	mSpeed = static_cast< int >( n * static_cast<double>(ED[ mType ].speed) );
}

void Enemy::update(){
		if ( isDead() ){ //死亡。结束。
			return;
		}

		think();
		updateBuff();

		Framework f = Framework::instance();
		mCDCount = ( mCDCount >= ED[ mType ].CD )?( ED[ mType ].CD ):( mCDCount + 1 );


		switch( mStatus ){
			case Enemy::IDLE:
				break;
			case Enemy::HOLD:{
				mAnimeCount++;
				int random = f.getRandom( 4 );
				while( random == mDirection ){
					random = f.getRandom( 4 );
				}
				mDirection = random;;			 
				}
				break;
			case Enemy::ATTACK:{
				mAnimeCount++;
				mCastCount = ( mCastCount >= CASTTIME )?( CASTTIME ):( mCastCount + 1 );
				if(( mCastCount == CASTTIME )&&( mCDCount == ED[ mType ].CD )){
					randomCast();//TODO
					mCastCount = mCDCount = 0;
					mStatus = Enemy::IDLE;
				}
				}
				break;
			default:
				break;
		}

		//現在的中心取得
		int x = convertInnerToCell( mX );
		int y = convertInnerToCell( mY );

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
					wallsX[ wallNumber ] = x + i - 1;
					wallsY[ wallNumber ] = y + j - 1;
					++wallNumber;				
				}
			}
		}

		//过渡墙壁列表的移動処理
		move( wallsX, wallsY, wallNumber );
}

void Enemy::randomCast(){
	Magic::TYPE t=Magic::NONE;
	Framework f=Framework::instance();
	while(!EMTable[mType][t-1]){
		t=static_cast<Magic::TYPE>(f.getRandom(9));
	}
	pState->instanceMagic( getCell() ,mDirection,static_cast<Magic::TYPE>(static_cast<int>(t)-1),DynamicObject::TYPE_ENEMY);

}

int Enemy::getDistance() const{
	double disX = static_cast< double >( convertInnerToCell( mX ) - pState->getCharacter()->getCell().x );
	disX *=disX;
	double disY = static_cast< double >( convertInnerToCell( mY ) - pState->getCharacter()->getCell().y );
	disY *=disY;
	if(mTarget.x!=0.0||mTarget.y!=0.0){
	disX = static_cast< double >( convertInnerToCell( mX ) - mTarget.x );
	disX *=disX;
	disY = static_cast< double >( convertInnerToCell( mY ) - mTarget.y );
	disY *=disY;	
	}
	return static_cast< int >( sqrt( disX + disY ));
}

int Enemy::getRotation() const{
	int x = static_cast<int>(pState->getCharacter()->getCell().x ) - convertInnerToCell( mX );
	int y = static_cast<int>(pState->getCharacter()->getCell().y ) - convertInnerToCell( mY );
	if(mTarget.x!=0.0||mTarget.y!=0.0){
		x = static_cast<int>(mTarget.x) - convertInnerToCell( mX );
		y = static_cast<int>(mTarget.y) - convertInnerToCell( mY );
	}
	int absX = abs( x );
	int absY = abs( y );
	if(( x >= 0 ) && ( y >= 0 )){
		return ( absX > absY )? 3 : 1; 
	} if(( x >= 0 ) && ( y <= 0 )){
		return ( absX > absY )? 3 : 0; 
	}if(( x <= 0 ) && ( y >= 0 )){
		return ( absX > absY )? 2 : 1; 
	}if(( x <= 0 ) && ( y <= 0 )){
		return ( absX > absY )? 2 : 0; 
	}
	return 1;
}

void Enemy::lookAt(const Vector2 &c){
	mTarget=c;
}

void Enemy::draw( const Image* image ) const {
	if ( isDead() ){
		return;//draw dead body
	}
	//内部座標变换到像素座標(+500是四捨五入)
	int dstX =  mX ;
	int dstY =  mY ;
	//确认画像裁剪位置
	int srcX, srcY;
	srcX = srcY = -1;

	bool isAntherFrame = ( mAnimeCount > HALFMOVE )?true:false;
	
	switch( mType ){
		case Enemy::TYPE_SLIME:
			srcX = isAntherFrame?32:32;
			srcY = isAntherFrame?48:48;
			break;
		case Enemy::TYPE_BOX:
			srcX = isAntherFrame?32:80;
			srcY = isAntherFrame?32:32;
			break;
		case Enemy::TYPE_SHADOW:
			srcX = isAntherFrame?112:96;
			srcY = isAntherFrame?32:32;
			break;
		case Enemy::TYPE_BOSS:
			srcX = isAntherFrame?32:32;
			srcY = isAntherFrame?16:16;
			break;
		default:
			break;
	}
		
	image->draw( dstX, dstY, srcX, srcY, 16, 16 );
}

void Enemy::move( const int* wallsX, const int* wallsY, int wallNumber ){
	
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

void Enemy::getVelocity( int* dx, int* dy ) const {

	switch( mDirection ){
		case 0:
			*dx=0;
			*dy=-1;
			break;
		case 1:
			*dx=0;
			*dy=-1;
			break;
		case 2:
			*dx=-1;
			*dy=0;
			break;
		case 3:
			*dx=1;
			*dy=0;
			break;
		default:
			*dx=*dy=0;
			break;
		
	}
	//速度計算
	*dx = *dx * mSpeed;
	*dy = *dy * mSpeed;
}

bool Enemy::isIntersectWall( int x, int y, int wallX, int wallY ){
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

bool Enemy::isIntersectWall( int wallX, int wallY ){
	return isIntersectWall( mX, mY, wallX, wallY );
}



bool Enemy::isIntersect( const DynamicObject& o ) const {
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

Vector2 Enemy::getInner(void) const{
	return Vector2( mX, mY );
}

Vector2 Enemy::getCell(void) const {
	int x = convertInnerToCell( mX );
	int y = convertInnerToCell( mY );
	return Vector2( x, y );
}

int Enemy::getType() const{
	return mType;
}

void Enemy::die(){
	mType = TYPE_NONE;
	pState->mExp+=ED[mType].EXP;
	Framework f=Framework::instance();
	pState->reborn();
}

bool Enemy::isDead() const {
	return ( mType == TYPE_NONE );
}

void Enemy::applyHeal( int heal ){

	mHP += heal;
	int Max = ED[ mType ].HP;
	if( mHP > Max ){
		mHP = Max;
	}
}

void Enemy::applyDamage(int damage){
	mHP -= damage;
	if( mHP <= 0 ){
		die();
	}
}

void Enemy::setBuff(Buff b ){

	mBuff = b;
}



void Enemy::clearBuff(  ){
	mBuff = Enemy::NORMAL;
}

bool Enemy::checkBuff( Buff b )const{
	return ( mBuff ==b );
}

