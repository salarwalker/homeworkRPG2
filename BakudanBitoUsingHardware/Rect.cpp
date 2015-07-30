#include "GameLib/GameLib.h"
#include "Rect.h"
#include "Vector2.h"

namespace{

	static const int SCRWIDTH = 16000 * 20;
	static const int SCRHEIGHT = 16000 * 15;
 
}//namespace using INNER UNIT
 
Rect::Rect(const Vector2& OriginPoint,double Width,double Height)
{
	mOrigin = Vector2( OriginPoint );
	if( Width < 0 ){
		Width = 0;
	}
	if( Height < 0 ){
		Height = 0;
	}
	mWH = Vector2( Width , Height );
}

Rect::Rect( const Rect& rRect ) {
	mOrigin = Vector2( rRect.getOriginVector() );
	mWH = Vector2( rRect.getWidth() ,rRect.getHeight() );
}

Vector2 Rect::getOriginVector() const{
	return Rect::mOrigin;
}

double Rect::getWidth() const{
	return Rect::mWH.x;	
}

double Rect::getHeight() const{
	return Rect::mWH.y;	
}

bool Rect::contains( const Rect& rect )const{
	Vector2 vRO = rect.getOriginVector();
	double dRW = rect.getWidth();
	double dRH = rect.getHeight();

	if(((vRO.x + dRW <= mOrigin.x   		) && (vRO.y + dRH <= mOrigin.y			))||
	   ((vRO.x 	     >= mOrigin.x + mWH.x   ) && (vRO.y + dRH <= mOrigin.y	 		))||
	   ((vRO.x + dRW <= mOrigin.x			) && (vRO.y	      >= mOrigin.y + mWH.y  ))||
	   ((vRO.x       >= mOrigin.x + mWH.x   ) && (vRO.y	      >= mOrigin.y + mWH.y  )))
	{
		return false;
	} else if( vRO.x + dRW <= mOrigin.x			||
			   vRO.x	   >= mOrigin.x + mWH.x ||
			   vRO.y + dRH <= mOrigin.y			||
			   vRO.y       >= mOrigin.y + mWH.y )
	{
		return false;
	} else
	{
		return true;
	}
}

void Rect::rotate( void ){
	double t = mWH.x;
	mWH.x = mWH.y;
	mWH.y = t;
}

Rect::~Rect(void)
{
	mOrigin=Vector2(0,0);
	mWH=Vector2(0,0);
}

Rect::Rect(){
}