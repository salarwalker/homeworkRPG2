#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Ending.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence{

Ending::Ending() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Ending::~Ending(){
	SAFE_DELETE( mImage );
}

void Ending::update( Parent* parent ){
	if ( mCount == 120 ){ //等待2秒
		parent->moveTo( Parent::NEXT_TITLE );
	}
	//绘制
	mImage->drawUI();//REDRAW
	//绘制文字
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "Congratulations！You finish the game!" );
	f.drawDebugString( 0, 1, "Builder Salar" );

	++mCount;
}

} //namespace Sequence