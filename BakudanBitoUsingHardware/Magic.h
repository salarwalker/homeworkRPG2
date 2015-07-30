#pragma once

#include <memory>
#include <string>
#include "Vector2.h"

class Magic
{
public:
	enum TYPE{
		MINOR_ILLUSION,
		RAY_OF_FROST,
		SHOCKING_GRASP,
		LANTERNLIGHT,
		BURNING_HANDS,
		LONGSTRIDER,
		THUNDERWAVE,
		FLAMING_SPHERE,
		SCORCHING_RAY,
		NONE,
	};


	static Magic* Instance(void);
	~Magic(void);
	static int getCastTime( TYPE t );
	static int getLastTime( TYPE t );
	static std::string getName( TYPE t );
	static Vector2 getScope( TYPE t );
	
private:
	Magic();
	static std::auto_ptr<Magic> pMag;
};
