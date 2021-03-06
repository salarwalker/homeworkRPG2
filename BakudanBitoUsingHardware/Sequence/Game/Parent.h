#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence/Child.h"
class State;

namespace Sequence{
class Parent;
namespace Game{
class Child;

class Parent : public Sequence::Child{
public:
	typedef Sequence::Parent GrandParent;
	enum NextSequence{
		NEXT_CLEAR,
		NEXT_READY,
		NEXT_PAUSE,
		NEXT_PLAY,
		NEXT_FAILURE,
		NEXT_ENDING,
		NEXT_GAME_OVER,
		NEXT_TITLE,

		NEXT_NONE,
	};
	enum Mode{
		MODE_S1,
		MODE_S2,
		MODE_S3,
		MODE_S4,
		MODE_S5,
		MODE_NONE,
	};
	enum CharacterID{
		CHAR_CHARACTER,
		CHAR_ENEMY,
		CHAR_NPC,
		CHAR_DYNAMICOBJECT,
		CHAR_NONE,
	};
	Parent( GrandParent::Mode );
	~Parent();
	void update( GrandParent* );
	void moveTo( NextSequence );


	State* state();
	void drawState() const; //游戏绘制
	bool hasFinalStageCleared() const; //是否通关最终关？
	int lifeNumber() const;
	Mode mode() const;
	void startLoading();
private:
	State* mState;
	int mStageID;
	int mLife;
	static const int FINAL_STAGE = 4; //现在第二关是最终关
	static const int INITIALI_LIFE_NUMBER = 2;

	NextSequence mNextSequence;

	Game::Child* mChild;
};

} //namespace Game
} //namespace Sequence

#endif