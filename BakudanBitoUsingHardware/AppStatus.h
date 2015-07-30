#pragma once
#include <memory>
#include "Magic.h"
#include <string>
class Character;
class AppStatus
{
public:
	enum ITEM{
	KEY,
	BOOK,
	KUSURIBIG,
	KUSURI,
	};

	

	~AppStatus(void);
	static AppStatus* instance(void);	
	int getGRounds(void) const;
	void addGRounds(void);
	void setIsRequestEnd(bool);
	bool getIsRequestEnd(void) const;	
	void addItem(ITEM);
	bool reduceItem( ITEM );
	int getItemNum( ITEM )const;
	bool isMagicUnlocked( Magic::TYPE )const;
	void unlockMagic();
	int getMaxHP();
	ITEM getCurrentItem()const;
	Magic::TYPE getCurrentMagic()const;
	void nextItem();
	void lastItem();
	void nextMagic();
	void lastMagic();
	static ITEM randomItem();
	static std::string getItemName(ITEM);
private:
	
	void resetItem(void);
	static std::auto_ptr<AppStatus> pAS;
	ITEM currItem;
	Magic::TYPE currMagic;
	int *pItems;
	AppStatus(void);
	int mGRounds;
	bool mIsRequestEnd;
	Character* pC;
	int mMaxHP;
};
 