#include "GameLib/Framework.h"
#include "AppStatus.h"
#include "Pad.h"
#include "Sequence/Parent.h"

using namespace GameLib;
//用户实现函数。实际上里面就全部工作交给了Sequence::Parent

namespace GameLib{
	void Framework::update(){
		if ( !Sequence::Parent::instance() ){
			Sequence::Parent::create();
			setFrameRate(60);
			
		}
		Sequence::Parent::instance()->update();
		//结束判定(按下esc键或按下X按钮)
		AppStatus* pAS=AppStatus::instance();

		if ( pAS->getIsRequestEnd()){
			requestEnd();
			
		}
		pAS = 0;

		if ( isEndRequested() ){
			Sequence::Parent::destroy();
		}
	}
}


