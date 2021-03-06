#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ),mCursorPosition( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
}

void Pause::update( Parent* parent ){
	//0:重开
	//1:选择画面
	//2:标题画面
	//3:返回
	if ( Pad::isTriggered( Pad::U )){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //负数则设置为最大值
 			mCursorPosition = 1;
		}
		//取消音效
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //超过1就是设置为0循环
			mCursorPosition = 0;
		}
		//取消音效
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::MAGIC ) ){
		if ( mCursorPosition == 0 ){ //回到游戏
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosition == 1 ){ //回到标题
			parent->moveTo( Parent::NEXT_TITLE );
		}
		//決定音
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}

	parent->drawState();
	mImage->drawUI();//REDRAW
	//绘制暂停
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[Pause]" );
	f.drawDebugString( 1, 2, "Game Resume" );
	f.drawDebugString( 1, 3, "GO Title" );
	//绘制光标
	f.drawDebugString( 0, mCursorPosition + 2, ">" );
}



} //namespace Game
} //namespace Sequence