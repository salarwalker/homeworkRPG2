#pragma once
#include <memory>
#include "Rect.h"

class Character;
class Vector2;

class Camera
{
public:
	static Camera* Instance(void);
	~Camera(void);
	bool contains(const Rect&);
	Vector2 getCamPosition() const;
	void updateCamera( const Character& );
	Vector2 getCamOrigin() const;
private:
	Camera(void);
	Vector2 mCenPos;
	static std::auto_ptr<Camera> pCam;
};
