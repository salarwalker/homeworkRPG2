#include "Magic.h"
#include <memory>
#include "Vector2.h"

namespace{
	struct MagicData{
		Magic::TYPE type;
		int iLastTime;
		int iCastTime;
		int iScopeX;
		int dScopeY;
		bool bDirectivity;
		std::string dName;
	};

	const static int FXTIME = 30;
	const static int SCALE = 16000;
	const static int HALF_SIZE = 8000;

	const static MagicData gData[] = {
		//TYPE					Last  Cast	sX sY isDire	NNAME  Direction UP as default 		
		{ Magic::MINOR_ILLUSION, 600,  60,	5, 5, true,		"Minor Illusion"},
		{ Magic::RAY_OF_FROST,    60,  45,  1, 5, true ,	"Ray of forst"},
		{ Magic::SHOCKING_GRASP,  60,  60,  5, 5, false,	"Shocking grasp"},
		{ Magic::LANTERNLIGHT,	  60,  30,  1,10, true,		"Lantern Light"},
		{ Magic::BURNING_HANDS,	  60,  30,  1, 1, true,		"BurNing hands"},
		{ Magic::LONGSTRIDER,	 180,  30,	1, 1, false,	"Longstrider"},
		{ Magic::THUNDERWAVE,	  90,  60, 10,10, false,	"Thunerwave"},
		{ Magic::FLAMING_SPHERE, 180, 120,  1, 1, true,		"Flaming sphere"},
		{ Magic::SCORCHING_RAY,   60, 120,  1, 10,false,	"Scorching ray"},
		{ Magic::NONE,			   0,	0,	0, 0, false,	"None"},
	};

	int convertInnerToCell( int x ){
		return ( x - HALF_SIZE ) /  SCALE ;
	}
}



Magic::Magic(){

}

Magic* Magic::Instance(void){

	if (!pMag.get()){

		pMag.reset(new Magic);
	
	}
	return pMag.get();
}

int Magic::getLastTime( TYPE t ){
	return gData[t].iLastTime;
}

int Magic::getCastTime( TYPE t ){
	return gData[t].iCastTime;
}

Vector2 Magic::getScope( TYPE t ){
	return Vector2( gData[ t ].iScopeX, gData[ t ].dScopeY );
}

std::string Magic::getName(TYPE t){
	return gData[t].dName;
}


Magic::~Magic(void)
{

}

std::auto_ptr<Magic> Magic::pMag;