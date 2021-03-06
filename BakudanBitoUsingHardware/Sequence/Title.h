#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

#include "Sequence/Child.h"
#include "AppStatus.h"
class Image;

namespace Sequence{
class Parent;

class Title : public Child{
public:
	Title();
	~Title();
	void update( Parent* );
private:
	Image* mImage; //标题封绘
	int mCursorPosition;
	bool isRequestEnd;
	AppStatus* pAS;
};

} //namespace Sequence

#endif