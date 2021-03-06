#include "GameLib/GameLib.h"
#include "Pad.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Joystick.h"
#include "GameLib/Input/Mouse.h"

using namespace GameLib;
//本类负责键盘输入的设置
bool Pad::isOn( Button b){
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	//Input::Mouse ms = im.mouse();

	bool ret = false;
	int table[] = { 'j', 'y','h','k','u','i','l', 'w', 's', 'a', 'd',kb.KEY_ESC };
	ret = kb.isOn( table[ b ] );
	return ret;
}

bool Pad::isTriggered( Button b ){
	Input::Manager im = Input::Manager::instance();
	Input::Keyboard kb = im.keyboard();
	bool ret = false;
	int table[] = { 'j', 'y','h','k','u','i','l', 'w', 's', 'a', 'd',kb.KEY_ESC };
	ret = kb.isTriggered( table[ b ] );
	return ret;
}
