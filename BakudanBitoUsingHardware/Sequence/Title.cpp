#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;
#include "AppStatus.h"
#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Title::~Title(){
	SAFE_DELETE( mImage );

}

void Title::update( Parent* parent ){
	//取得输入
	if(isRequestEnd){
			Framework f = Framework::instance();
		f.requestEnd();			
		if (f.isEndRequested()){
			Sequence::Parent::destroy();
		
		}
	}
	

	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ 
 			mCursorPosition = 1;
		}
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){  
			mCursorPosition = 0;
		}
 
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::MAGIC ) ){
		parent->moveTo( Parent::NEXT_GAME );
		if ( mCursorPosition == 0 ){
			parent->setMode( Parent::MODE_S1 );
		}else if ( mCursorPosition == 1 ){
			
			pAS=AppStatus::instance();
			Title::pAS->setIsRequestEnd(true);
			pAS=0;
			parent->setMode(Parent::MODE_S1);
			
	}else{
			HALT( "arienai" );
		}
 
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	mImage->drawUI();//REDRAW
	Framework f = Framework::instance();

	f.drawDebugString( 0, 0, "Boomer" );
	f.drawDebugString( 1, 2, "Play Game" );
	f.drawDebugString( 1, 3, "End Game" );
	f.drawDebugString( 0, mCursorPosition + 2, ">" );
}

} //namespace Sequence