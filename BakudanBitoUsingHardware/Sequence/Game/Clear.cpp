#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //等1秒 
		if ( parent->hasFinalStageCleared() ){ //最终关通关就End画面
			parent->moveTo( Parent::NEXT_ENDING ); 
		}else{
			parent->moveTo( Parent::NEXT_READY );
		}
	}
	//绘制
	//首先是游戏画面绘制
	parent->drawState();
	//上面有祝贺信息，绘制
	mImage->drawUI();//REDRAW
	//通关！绘制这字
	Framework::instance().drawDebugString( 0, 0, "Clear!" );

	++mCount;
}

} //namespace Game
} //namespace Sequence
