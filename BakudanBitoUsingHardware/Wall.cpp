#include "Wall.h"
#include "Image.h" 

Wall::Wall( int CellX, int CellY, unsigned f ):
mFlags( f ),
mCellX( CellX ),
mCellY( CellY ){
}

Wall::~Wall(void)
{
}

Wall::Wall(){
}

void Wall::draw( const Image* image ) const{
	  /*exp:U = SPE | ONE |       PLU 
			D = SPE | ONE
		    L = SPE | ONE | HOR | PLU
			R = SPE | ONE | HOR 
		   |_ = SPE |             PLU
		   �� = SPE
		    ��= SPE |       HOR | PLU
			7 = SPE |		HOR
		   -  =				HOR
		    | =				HOR | PLU
			��=					  PLU a block
			[]= 0 a floor
      */
	int srcX = -1;
	int srcY = -1;
	if( mFlags & (~SPE |~ONE |~HOR |~PLU )){
		srcX = 48; //	��
		srcY = 32;
	}else if ( mFlags & ( SPE | ONE |~HOR | PLU )){
		srcX = 48; //	U
		srcY = 32;
	}else if ( mFlags & ( SPE | ONE |~HOR |~PLU )){
		srcX = 48; //	S
		srcY = 32;
	}else if ( mFlags & ( SPE | ONE | HOR | PLU )){
		srcX = 48; //	L
		srcY = 32;
	}else if ( mFlags & ( SPE | ONE | HOR |~PLU )){
		srcX = 48; //	R
		srcY = 32;
	}else if ( mFlags & ( SPE |~ONE |~HOR | PLU )){
		srcX = 48; //	|_
		srcY = 32;
	}else if ( mFlags & ( SPE |~ONE |~HOR |~PLU )){
		srcX = 48; //	��
		srcY = 32;
	}else if ( mFlags & ( SPE |~ONE | HOR |~PLU )){
		srcX = 48; //	7
		srcY = 32;
	}else if ( mFlags & (~SPE |~ONE | HOR |~PLU )){
		srcX = 48; //	[]= 0 a floor
		srcY = 32;
	}else if ( mFlags & (~SPE |~ONE |~HOR | PLU )){
		srcX = 16; //	|
		srcY = 32;
	}else if ( mFlags & ( SPE | ONE | HOR |~PLU )){
		srcX = 48; //	[]= 0 a floor
		srcY = 32;}
	else {
			srcX = 0; 
			srcY = 32;//other 
	}
	image->draw(mCellX*16000+8000, mCellY*16000+8000, srcX, srcY, 16, 16 );

}