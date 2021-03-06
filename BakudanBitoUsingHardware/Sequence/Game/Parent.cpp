#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Game/Failure.h"
#include "Game/State.h"
#include "File.h"
#include <sstream>

namespace Sequence{
namespace Game{

Parent::Parent( GrandParent::Mode mode ) : 
mState( 0 ),
mStageID( 0 ),
mLife( INITIALI_LIFE_NUMBER ),
mNextSequence( NEXT_NONE ),
mChild( 0 ){
	/*if ( mode == GrandParent::MODE_S1 ){
		mStageID = 1;
	}else{
		mStageID = 0;
	}*/

	switch(mode){
		case GrandParent::MODE_S1:
			mStageID = 0;
			break;
		case GrandParent::MODE_S2:
			mStageID = 1;
			break;
		case GrandParent::MODE_S3:
			mStageID = 2;
			break;
		case GrandParent::MODE_S4:
			mStageID = 3;
			break;
		case GrandParent::MODE_S5:
			mStageID = 4;
			break;
		case GrandParent::MODE_NONE:
			HALT("Stage wrong");
			break;
		default:
			break;
	}
	//最初Ready
	mChild = new Ready();
}

Parent::~Parent(){
	SAFE_DELETE( mState );
	SAFE_DELETE( mChild );
}

void Parent::update( GrandParent* parent ){
	mChild->update( this );
	//遷移判定
	switch ( mNextSequence ){
		case NEXT_CLEAR:
			SAFE_DELETE( mChild );
			mChild = new Clear();
			++mStageID; //下一关
			break;
		case NEXT_READY:
			SAFE_DELETE( mChild );
			mChild = new Ready();
			break;
		case NEXT_PAUSE:
			SAFE_DELETE( mChild );
			mChild = new Pause();
			break;
		case NEXT_PLAY:
			SAFE_DELETE( mChild );
			mChild = new Play();
			break;
		case NEXT_FAILURE:
			SAFE_DELETE( mChild );
			mChild = new Failure();
			--mLife;
			break;
		case NEXT_ENDING:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_ENDING );
			break;
		case NEXT_GAME_OVER:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_GAME_OVER );
			break;
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_TITLE );
			break;
	}
	mNextSequence = NEXT_NONE;
}

void Parent::moveTo( NextSequence next ){
	ASSERT( mNextSequence == NEXT_NONE );
	mNextSequence = next;
}

State* Parent::state(){
	return mState;
}

bool Parent::hasFinalStageCleared() const {
	return ( mStageID > FINAL_STAGE );
}

int Parent::lifeNumber() const {
	return mLife;
}

//Parent::Mode实例化。之后的画面无视Parent。
Parent::Mode Parent::mode() const {
	Mode r = MODE_NONE;
	switch ( GrandParent::instance()->mode() ){
		case GrandParent::MODE_S1: r = MODE_S1; break;
		case GrandParent::MODE_S2: r = MODE_S2; break;
		case GrandParent::MODE_S3: r = MODE_S3; break;
		case GrandParent::MODE_S4: r = MODE_S4; break;
		case GrandParent::MODE_S5: r = MODE_S5; break;
		default: ASSERT( false ); break;
	}
	return r;
}

void Parent::startLoading(){
	SAFE_DELETE( mState );
	mState = new State( mStageID );
}

void Parent::drawState() const {
	mState->draw();
}



} //namespace Game
} //namespace Sequence