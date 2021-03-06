#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Failure::Failure() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Failure::~Failure(){
	SAFE_DELETE( mImage );
}

void Failure::update( Parent* parent ){
	if ( mCount == 60 ){ //等待1秒
		if ( parent->lifeNumber() == 0 ){
			parent->moveTo( Parent::NEXT_GAME_OVER ); 
		}else{
			parent->moveTo( Parent::NEXT_READY );
		}
	}
	//绘制
	//先绘制预习画面
	parent->drawState();
	//上面是祝贺信息（？）
	mImage->drawUI();//REDRAW
	//失败了！绘制这句话
	Framework::instance().drawDebugString( 0, 0, "You Fail." );

	++mCount;
}

} //namespace Game
} //namespace Sequence
