#pragma once
#include "Game\StaticObject.h"

class Wall :
	public StaticObject
{
public:
	 enum Flag{
		SPE = ( 1 << 0 ), //special or normal
		ONE = ( 1 << 1 ), //one or two part
		HOR = ( 1 << 2 ), //start with horizon or vertical 
		PLU = ( 1 << 3 ), //depend on type,plus or minus
	};
	  /*exp:U = SPE | ONE | PLU 
			D = SPE | ONE
		    L = SPE | ONE | HOR | PLU
			R = SPE | ONE | HOR 
		   |_ = SPE | PLU
		   ¡¹ = SPE
		    ¡¸= SPE | HOR |PLU
			7 = SPE | HOR
		   -  = HOR
		    | = PLU
			¿Ú= 0
      */	
	int mCellX;
	int mCellY;
private:
	unsigned mFlags; //±ê¼Ç


public:
	Wall( int CellX, int CellY, unsigned f );
	Wall();
	~Wall(void);
	void draw( const Image* image ) const;
};
