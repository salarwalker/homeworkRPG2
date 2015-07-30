#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

namespace Sequence{
class Child;

class Parent{
public:
	enum NextSequence{
		NEXT_TITLE,
		NEXT_GAME,
		NEXT_GAME_OVER,
		NEXT_ENDING,

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
	void update();
	void moveTo( NextSequence );
	Mode mode() const;
	void setMode( Mode );

	static void create();
	static void destroy();
	static Parent* instance();
private:
	Parent();
	~Parent();

	NextSequence mNextSequence;
	Mode mMode;

	Child* mChild;

	static Parent* mInstance;
};

} //namespace Sequence

#endif
