#include "Camera.h"
#include <memory>
#include "Character.h"
#include "Vector2.h"

namespace{
static const int HS_WIDTH = 160 * 1000;
static const int HS_HEIGHT = 120 * 1000;
static const int FS_WIDTH = 320 * 1000;
static const int FS_HEIGHT = 240 * 1000;
}

Camera::Camera(void)
{
	//get player position
	mCenPos = Vector2( 0 , 0 );
}

Camera::~Camera(void)
{
}

Vector2 Camera::getCamPosition(void) const{
	return mCenPos;
}

Vector2 Camera::getCamOrigin() const{
	return Vector2(mCenPos.x - HS_WIDTH,mCenPos.y - HS_HEIGHT);
}

void Camera::updateCamera( const Character& p )
{
	mCenPos.x = p.getInner().x;
	mCenPos.y = p.getInner().y;
	return;
}

bool Camera::contains(const Rect& rRect){
	Vector2 vRO = rRect.getOriginVector();
	double dRW = rRect.getWidth();
	double dRH = rRect.getHeight();

	if(((vRO.x + dRW <= mCenPos.x - HS_WIDTH) && (vRO.y + dRH <= mCenPos.y - HS_HEIGHT))||
	   ((vRO.x 	     >=mCenPos.x + HS_WIDTH) && (vRO.y + dRH <= mCenPos.y - HS_HEIGHT))||
	   ((vRO.x + dRW <=mCenPos.x - HS_WIDTH) && (vRO.y	     >= mCenPos.y + HS_HEIGHT))||
	   ((vRO.x       >=mCenPos.x + HS_WIDTH) && (vRO.y	     >= mCenPos.y + HS_HEIGHT)))
	{	
		return false;
	} else if(vRO.x + 3.0*dRW <= mCenPos.x - HS_WIDTH ||
			  vRO.x		  >= mCenPos.x + HS_WIDTH ||
			  vRO.y + 3.0*dRH <= mCenPos.y - HS_HEIGHT||
			  vRO.y       >= mCenPos.y + HS_HEIGHT)
	{
		return false;
	} else
	{
		return true;
	}
 }

Camera* Camera::Instance(void){

	if (!pCam.get()){

		pCam.reset(new Camera);
	
	}
	return pCam.get();

}

std::auto_ptr<Camera> Camera::pCam;