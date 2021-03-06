#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "Game/State.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{
namespace Game{

Play::Play(){
	//BGM開始
	SoundManager::instance()->playBgm( SoundManager::BGM_GAME );
}

Play::~Play(){ 
	//BGM停止
	SoundManager::instance()->stopBgm();
}

//游戏本体
void Play::update( Parent* parent ){
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->state();

	bool cleared = state->hasCleared();
	bool die1P = !state->isAlive( );
	//先用debug命令来测试。作弊码
if ( kb.isTriggered( 'c' ) ){
		cleared = true;
	}
	//按下SPACE就暂停
	//通关就向上报告

		if ( cleared && !die1P ){
			parent->moveTo( Parent::NEXT_CLEAR );
		}else if ( die1P ){
			parent->moveTo( Parent::NEXT_FAILURE );
		}
	
	//按下B按钮就暂停
	if ( Pad::isTriggered( Pad::Q ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	} 
	//state=>acceptInput();
	//更新
	state->update();
	//绘制
	state->draw();

}
} //namespace Game
} //namespace Sequence
