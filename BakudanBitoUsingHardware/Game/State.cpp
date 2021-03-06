#include "GameLib/Framework.h"
#include "Camera.h"
#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "Character.h"
#include "SoundManager.h"
#include "Image.h"
#include "Rect.h"
#include "Enemy.h"
#include "AppStatus.h"
#include "math.h"
#include "FX.h"
using namespace GameLib;

namespace {
//一秒=60帧
//地图的广度

//適当的参数集合
static const int HALF_SIZE = 8000;
static const int SCALE = 16000;

static const int MAXFX = 50;
//適当的关卡数据
struct StageData{
	int mWidth;
	int mHeight;
	int mEnemyNumber;
	int mClearExp; 
	int mMechanismRate;
};

static StageData gSD[] = {
	{ 80, 60, 50, 100,  23 },
	{ 120, 90, 100, 200, 13 },
	{ 160, 120, 200, 300, 3 },
};

//转换到内部単位
int convertInnerToCell( int x ){
	return ( x - HALF_SIZE ) /  SCALE;
}
} //namespace{}


State::State( int StageID ) : 
mImage( 0 ),
pEnemys( 0 ),
pFXs( 0 ),
mENumber( 0 ),
mEIndex( 0 ),
mFXIndex(0),
mExp(0),
mStageID( StageID ){
	Framework f = Framework::instance();
	//AppStatus* pAS=AppStatus::instance();
	int w=gSD[mStageID].mWidth;
	int h=gSD[mStageID].mHeight;
	mSOs.setSize( w, h );
	mExp=0;
	mImage = new Image( "data/image/bakudanBitoImage.dds" );	
	
	for ( int y = 0; y < h; ++y ){
		for ( int x = 0; x < w; ++x ){
			StaticObject& o = mSOs( x, y );
				o.mCellX = x;
				o.mCellY = y;
			if(x==0||y==0||x==w-1||y==h-1){
				o.setFlag(StaticObject::FLAG_WALL);
			}else{
				if(f.getRandom(100)<=gSD[mStageID].mMechanismRate){
					o.setFlag(StaticObject::randomMechanism());
				}else{
					o.clearFlag();
				}
			}
		}
	}
	mSOs(1,1).setFlag(StaticObject::FLAG_NOTHING);
	mSOs(2,1).setFlag(StaticObject::FLAG_NOTHING);
	mSOs(1,2).setFlag(StaticObject::FLAG_NOTHING);
	pC = new Character();
	pC->set(1,1,this);
	pFXs=new FX[MAXFX]();
	pEnemys = new Enemy[gSD[mStageID].mEnemyNumber]();
	for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
		int x =f.getRandom(w-1)+1;
		int y =f.getRandom(h-1)+1;
		mSOs(x,y).clearFlag();
		pEnemys[i].set(x,y,Enemy::randomEnemy(),this);
	}
}

State::~State(){
	SAFE_DELETE( mImage );
	SAFE_DELETE_ARRAY( pEnemys );
}

void State::instanceMagic(const Vector2& c,int Dir, Magic::TYPE m, DynamicObject::Type t){
	switch(m){
		
		case Magic::MINOR_ILLUSION:
			if(t!=DynamicObject::TYPE_CHARACTER){return;}
			pFXs[mFXIndex].set(c,Dir,Magic::getLastTime(m),FX::ILLUSION);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
				if((abs(pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).y)&&
				   (abs(pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y)){
					pEnemys[i].lookAt(c);
				}
			}			
			break;
		case Magic::RAY_OF_FROST:{
			FX::TAG tag=FX::FLASH_VER;
			if(t==DynamicObject::TYPE_CHARACTER){	
				if(Dir==0||Dir==1){
					tag=FX::FLASH_VER;
					if(Dir==0){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().x==c.x&&(c.y-pEnemys[i].getCell().y)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
								pEnemys[i].setBuff(Enemy::SLOWDOWN);
							}
						}
					}else{
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().x==c.x&&(pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
								pEnemys[i].setBuff(Enemy::SLOWDOWN);
							}
						}
					}
				}else{ 
					tag=FX::FLASH_HOR;
					if(Dir==2){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().y==c.y&&(c.x-pEnemys[i].getCell().x)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
								pEnemys[i].setBuff(Enemy::SLOWDOWN);
							}
						}
					}else if(Dir==3){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().y==c.y&&(pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
								pEnemys[i].setBuff(Enemy::SLOWDOWN);
							}
						}
					}				
				}
			}else if(t==DynamicObject::TYPE_ENEMY){
				if(Dir==0||Dir==1){
					tag=FX::FLASH_VER;
					if(Dir==0){
						if(pC->getCell().x==c.x&&(c.y-pC->getCell().y)<=Magic::getScope(m).y){
							pC->applyDamage(1);
							pC->setBuff(Character::SLOWDOWN);
						}						
					}else{
						if(pC->getCell().x==c.x&&(pC->getCell().y-c.y)<=Magic::getScope(m).y){
							pC->applyDamage(1);
							pC->setBuff(Character::SLOWDOWN);
						}	
					}
				}else{ 
					tag=FX::FLASH_HOR;
					if(Dir==2){
						if(pC->getCell().y==c.y&&(c.x-pC->getCell().x)<=Magic::getScope(m).y){
							pC->applyDamage(1);
							pC->setBuff(Character::SLOWDOWN);
						}	
					}else if(Dir==3){
						if(pC->getCell().y==c.y&&(pC->getCell().x-c.x)<=Magic::getScope(m).y){
							pC->applyDamage(1);
							pC->setBuff(Character::SLOWDOWN);
						}	
					}				
				}
			}	
			pFXs[mFXIndex].set(c,Dir,Magic::getLastTime(m),tag);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			}
			break;
		case Magic::SHOCKING_GRASP:
			if(t==DynamicObject::TYPE_CHARACTER){
				for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
					if((abs(pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).y)&&
					   (abs(pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y)){
						   pEnemys[i].applyDamage(3);
						   pFXs[mFXIndex].set( pEnemys[i].getCell(),Dir,Magic::getLastTime(m),FX::LIGHTNTING);
						   mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
					}
				}
			}else if(t==DynamicObject::TYPE_ENEMY){			
				if((abs(pC->getCell().x-c.x)<=Magic::getScope(m).x)&&
					(abs(pC->getCell().y-c.y)<=Magic::getScope(m).y)){
					   pC->applyDamage(3);
					   pFXs[mFXIndex].set(pC->getCell(),Dir,Magic::getLastTime(m),FX::LIGHTNTING);
					   mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
				}
			}
			break;
		case Magic::LANTERNLIGHT:{
			FX::TAG tag=FX::LASER_VER;;
			if(t==DynamicObject::TYPE_CHARACTER){				
				if(Dir==0||Dir==1){
					tag=FX::LASER_VER;
					if(Dir==0){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().x==c.x&&(c.y-pEnemys[i].getCell().y)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
							}
						}
					}else{
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().x==c.x&&(pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
							}
						}
					}
				}else{ 
					tag=FX::LASER_HOR;
					if(Dir==2){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().y==c.y&&(c.x-pEnemys[i].getCell().x)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
							}
						}
					}else if(Dir==3){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().y==c.y&&(pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(2);
							}
						}
					}				
				}
			}else if(t==DynamicObject::TYPE_ENEMY){
				if(Dir==0||Dir==1){
					tag=FX::LASER_VER;
					if(Dir==0){
						if(pC->getCell().x==c.x&&(c.y-pC->getCell().y)<=Magic::getScope(m).y){
							pC->applyDamage(2);
						}						
					}else{
						if(pC->getCell().x==c.x&&(pC->getCell().y-c.y)<=Magic::getScope(m).y){
							pC->applyDamage(2);
						}	
					}
				}else{ 
					tag=FX::LASER_HOR;
					if(Dir==2){
						if(pC->getCell().y==c.y&&(c.x-pC->getCell().x)<=Magic::getScope(m).y){
							pC->applyDamage(2);
						}	
					}else if(Dir==3){
						if(pC->getCell().y==c.y&&(pC->getCell().x-c.x)<=Magic::getScope(m).y){
							pC->applyDamage(2);
						}	
					}				
				}
			}	
			pFXs[mFXIndex].set(c,Dir,Magic::getLastTime(m),tag);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			}
			break;
		case Magic::BURNING_HANDS:
			if(t==DynamicObject::TYPE_CHARACTER){
				pFXs[mFXIndex].set(pC->getCellForward(),Dir,Magic::getLastTime(m),FX::FIRE);
				mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
				for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
					if((pEnemys[i].getCell().x==pC->getCellForward().x)&&
					   (pEnemys[i].getCell().y==pC->getCellForward().y)){
						   pEnemys[i].applyDamage(3);
						   pEnemys[i].setBuff(Enemy::BURNING);
						   break;
					}
				}
			}else if(t==DynamicObject::TYPE_ENEMY){
				Vector2 forward;
				switch( Dir ){
				case 0:
					forward = Vector2( c.x, c.y - 1 );
					break;
				case 1:
					forward = Vector2( c.x, c.y + 1 );
					break;
				case 2:
					forward = Vector2( c.x - 1 , c.y );
					break;
				case 3:
					forward = Vector2( c.x+ 1 , c.y );
					break;
				default:	
					forward = Vector2( c.x, c.y );
					break;
				}
				pFXs[mFXIndex].set(forward,Dir,Magic::getLastTime(m),FX::FIRE);
				mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
				if((forward.x==pC->getCell().x)&&
				   (forward.y==pC->getCell().y)){
					pC->applyDamage(1);
					pC->setBuff(Character::BURNING);
					break;
				}			
			}
			break;
		case Magic::LONGSTRIDER:
			if(t==DynamicObject::TYPE_CHARACTER){
			pC->setBuff(Character::SPEEDUP);
			}
			if(t==DynamicObject::TYPE_ENEMY){
				for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
					if((pEnemys[i].getCell().x==c.x)&&
					   (pEnemys[i].getCell().y==c.y)){
						   pEnemys[i].setBuff(Enemy::SPEEDUP);
						   break;
					}
				}
			}
			break;
		case Magic::THUNDERWAVE:
			if(t==DynamicObject::TYPE_CHARACTER){
				for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
					if((abs(pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).y)&&
					   (abs(pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y)){
						   pEnemys[i].applyDamage(2);
						   pEnemys[i].setBuff(Enemy::SLOWDOWN);
						   pFXs[mFXIndex].set( pEnemys[i].getCell(),Dir,Magic::getLastTime(m),FX::LIGHTNTING);
						   mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
					}
				}
			}else if(t==DynamicObject::TYPE_ENEMY){			
				if((abs(pC->getCell().x-c.x)<=Magic::getScope(m).x)&&
				   (abs(pC->getCell().y-c.y)<=Magic::getScope(m).y)){
					   pC->applyDamage(2);
					   pC->setBuff(Character::SLOWDOWN);
					   pFXs[mFXIndex].set(pC->getCell(),Dir,Magic::getLastTime(m),FX::LIGHTNTING);
					   mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
				}
			}
			break;
		case Magic::FLAMING_SPHERE:{
			if(t==DynamicObject::TYPE_CHARACTER){				
				if(Dir==0||Dir==1){
					if(Dir==0){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().x==c.x&&(c.y-pEnemys[i].getCell().y)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(5);
								pEnemys[i].setBuff(Enemy::BURNING);

							}
						}
					}else{
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().x==c.x&&(pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(5);
								pEnemys[i].setBuff(Enemy::BURNING);
							}
						}
					}
				}else{ 
					if(Dir==2){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().y==c.y&&(c.x-pEnemys[i].getCell().x)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(5);
								pEnemys[i].setBuff(Enemy::BURNING);
							}
						}
					}else if(Dir==3){
						for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
							if(pEnemys[i].getCell().y==c.y&&(pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).y){
								pEnemys[i].applyDamage(5);
								pEnemys[i].setBuff(Enemy::BURNING);
							}
						}
					}				
				}
			}else if(t==DynamicObject::TYPE_ENEMY){
				if(Dir==0||Dir==1){
					if(Dir==0){
						if(pC->getCell().x==c.x&&(c.y-pC->getCell().y)<=Magic::getScope(m).y){
							pC->applyDamage(3);
							pC->setBuff(Character::BURNING);
						}						
					}else{
						if(pC->getCell().x==c.x&&(pC->getCell().y-c.y)<=Magic::getScope(m).y){
							pC->applyDamage(3);
							pC->setBuff(Character::BURNING);
						}	
					}
				}else{ 
					if(Dir==2){
						if(pC->getCell().y==c.y&&(c.x-pC->getCell().x)<=Magic::getScope(m).y){
							pC->applyDamage(3);
							pC->setBuff(Character::BURNING);
						}	
					}else if(Dir==3){
						if(pC->getCell().y==c.y&&(pC->getCell().x-c.x)<=Magic::getScope(m).y){
							pC->applyDamage(3);
							pC->setBuff(Character::BURNING);
						}	
					}				
				}
			}	
			pFXs[mFXIndex].set(c,Dir,Magic::getLastTime(m),FX::FIREBALL);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
		}
		break;
		case Magic::SCORCHING_RAY:
			if(t==DynamicObject::TYPE_CHARACTER){
				for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
					if(((c.x==pEnemys[i].getCell().x)&&((pEnemys[i].getCell().x-c.x)<=Magic::getScope(m).x))||
					   ((c.y==pEnemys[i].getCell().y)&&((pEnemys[i].getCell().y-c.y)<=Magic::getScope(m).y))){
						pEnemys[i].applyDamage(4);
					}
				}
			}else if(t==DynamicObject::TYPE_ENEMY){
				if(((c.x==pC->getCell().x)&&((pC->getCell().x-c.x)<=Magic::getScope(m).x))||
				   ((c.y==pC->getCell().y)&&((pC->getCell().y-c.y)<=Magic::getScope(m).y))){
						pC->applyDamage(4);
				}
			}
			pFXs[mFXIndex].set(c,0,Magic::getLastTime(m),FX::FLASH_VER);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			pFXs[mFXIndex].set(c,1,Magic::getLastTime(m),FX::FLASH_VER);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			pFXs[mFXIndex].set(c,2,Magic::getLastTime(m),FX::FLASH_HOR);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			pFXs[mFXIndex].set(c,3,Magic::getLastTime(m),FX::FLASH_HOR);
			mFXIndex=(mFXIndex>=(MAXFX-1))?0:(mFXIndex+1);
			break;
		case Magic::NONE:
			return;
		default:
			break;
	}	
}



void State::draw() const {

	int w=gSD[mStageID].mWidth;
	int h=gSD[mStageID].mHeight;
	//背景贴图操作
	for ( int y = 0; y < h; ++y ){
		for ( int x = 0; x < w; ++x ){
			mSOs( x, y ).draw( mImage );
		}
	}
	//敌方贴图操作
	for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
		pEnemys[i].draw(mImage);
	}
	
	for(int i =0;i<MAXFX;++i){
		pFXs[i].draw(mImage);
	}
	pC->draw( mImage );
}

void State::update(){
	Camera* pCam = Camera::Instance();
	pC->handleInput();
	for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
		pEnemys[i].update();
	}
	pC->update();
	for(int i =0;i<MAXFX;++i){
		pFXs[i].update();
	}
	pCam->updateCamera((*pC));
	pCam = 0;
}
void State::reborn(){
	Framework f =Framework::instance();
	int w=gSD[mStageID].mWidth;
	int h=gSD[mStageID].mHeight;
	int i=f.getRandom(100);
	if(i<75){
		for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
			if(pEnemys[i].isDead()){
				int x =f.getRandom(w-1)+1;
				int y =f.getRandom(h-1)+1;
				pEnemys[i].set(x,y,Enemy::randomEnemy(),this);
			}
			break;
		}
	}
	i=f.getRandom(100);
	if(i<25){
	for(int i = 0;i<gSD[mStageID].mEnemyNumber;++i){
		if(pEnemys[i].isDead()){
			int x =f.getRandom(w-1)+1;
			int y =f.getRandom(h-1)+1;
			pEnemys[i].set(x,y,Enemy::randomEnemy(),this);
		}
		break;
	}
}

}


bool State::hasCleared() const {
	return mExp>=gSD[mStageID].mClearExp;
}

void State::clearWall( int x, int y ){
	mSOs( x, y ).setFlag( StaticObject::FLAG_NOTHING );
}

bool State::isAlive( void ) const {
	return !pC->isDead();
}

Character* State::getCharacter(){
	return pC;
}