#include "AppStatus.h"
#include <memory>
#include "GameLib/GameLib.h"
#include "Magic.h"
#include "GameLib/Framework.h"

namespace{
	struct MagicStatus{
		Magic::TYPE type;
		bool bUnlocked;
	};
	
	

	static std::string IN[]={
		"KEY to open the door"			,
		"Read BOOK to learn new Magic"	,
		"BOTTLE to heal 6hp"			,
		"LEAF to heal 3hp"				,
	};

	static MagicStatus gMSCharacter[]={
	
		{ Magic::MINOR_ILLUSION,true},
		{ Magic::RAY_OF_FROST,  false},
		{ Magic::SHOCKING_GRASP,false},
		{ Magic::LANTERNLIGHT,  false},
		{ Magic::BURNING_HANDS,	true},
		{ Magic::LONGSTRIDER,	false},
		{ Magic::THUNDERWAVE,	false},
		{ Magic::FLAMING_SPHERE,false},
		{ Magic::SCORCHING_RAY,	true},
		{ Magic::NONE,			false},
		};
}

AppStatus* AppStatus::instance(void){
	if (!pAS.get()){
		pAS.reset(new AppStatus);
		
	}
	return pAS.get();
}

AppStatus::AppStatus(void)
{	
	mMaxHP = 4;
	mIsRequestEnd=false;
	mGRounds=0;
	currMagic=Magic::SCORCHING_RAY;//BURNING_HANDS;
	currItem=KUSURI;
	resetItem();
}

AppStatus::~AppStatus(void)
{
	pC=0;
	SAFE_DELETE_ARRAY(pItems);
}

std::string AppStatus::getItemName(ITEM i){
return IN[static_cast<int>(i)];
}

void AppStatus::resetItem(){
	AppStatus::pItems = new int[KUSURI];
	for( int i = 0 ; i < KUSURI ; ++i ){
		*(pItems+i) = 0;
	}
	pItems[KUSURI] = 3;
	pItems[KEY] = 10;
}

AppStatus::ITEM AppStatus::randomItem(){
	GameLib::Framework f = GameLib::Framework::instance();
	return static_cast<ITEM>(f.getRandom(4));
}

void AppStatus::addItem(ITEM i){
	int &a = pItems[i];
	++a;
	if( a > 9 ){
		a = 9;	
	} 
}
bool AppStatus::reduceItem(ITEM i){
	int &a = pItems[i];
	--a;
	if( a < 0 ){
		a = 0;
		return false;
	} else {
		return true;
	}
}

int AppStatus::getItemNum( ITEM i )const{
	return *( pItems + i );
}

bool AppStatus::isMagicUnlocked( Magic::TYPE t )const{
	return gMSCharacter[t].bUnlocked;
}

void AppStatus::unlockMagic(){
	Magic::TYPE t=currMagic;
	int turns=0;
	do{
		int i=static_cast<int>(t)+1;
		if(i>=9){
			i=1;
		}
		t=static_cast<Magic::TYPE>(i);
		turns++;
		if(turns>9){
			return;
		}
	}while(isMagicUnlocked(t));
	gMSCharacter[t].bUnlocked=true;
	++mMaxHP;
}

Magic::TYPE AppStatus::getCurrentMagic() const{
	return currMagic;
}

AppStatus::ITEM AppStatus::getCurrentItem() const{
	return currItem;
}

void AppStatus::nextMagic(){
	do{
		int i=static_cast<int>(currMagic)+1;
		if(i>=9){
			i=1;
		}
		currMagic=static_cast<Magic::TYPE>(i);
	}while(!isMagicUnlocked(currMagic));
}

void AppStatus::lastMagic(){
	do{
		int i=static_cast<int>(currMagic)-1;
		if(i<=1){
			i=9;
		}
		currMagic=static_cast<Magic::TYPE>(i);
	}while(!isMagicUnlocked(currMagic));
}

void AppStatus::nextItem(){
	switch(currItem){
		case KEY:
			currItem=BOOK;
			break;
		case BOOK:
			currItem=KUSURIBIG;
			break;
		case KUSURIBIG:
			currItem=KUSURI;
			break;
		case KUSURI:
			currItem=KEY;
			break;
		default:
			break;
	}
}

void AppStatus::lastItem(){
	switch(currItem){
		case KEY:
			currItem=KUSURI;
			break;
		case BOOK:
			currItem=KEY;
			break;
		case KUSURIBIG:
			currItem=BOOK;
			break;
		case KUSURI:
			currItem=KUSURIBIG;
			break;
		default:
			break;
	}
}


int AppStatus::getMaxHP(){
	return mMaxHP;
}

int AppStatus::getGRounds(void) const{
	return mGRounds;
}

void AppStatus::addGRounds(void){
	 ++mGRounds;
	 return;
}

bool AppStatus::getIsRequestEnd(void) const{
	return mIsRequestEnd;
}

void AppStatus::setIsRequestEnd(bool Bool2Set){
	mIsRequestEnd=Bool2Set;
	return;
}

std::auto_ptr<AppStatus> AppStatus::pAS;