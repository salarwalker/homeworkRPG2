#ifndef INCLUDED_PAD_H
#define INCLUDED_PAD_H

class Pad{
public:
	enum Button{
		MAGIC, //按键0 'j'
		MAGIC_U,
		MAGIC_D,
		ITEM, //按键1 'k'
		ITEM_U,
		ITEM_D,
		RUN,
		U, //up
		D, //down
		L, //left
		R, //right
		Q, //quit。退出键
	};
	//其实还应该加入无效按键
	static bool isOn( Button);
	static bool isTriggered( Button);
};

#endif