#pragma once
#ifndef INCLUDED_RECT_H
#define INCLUDED_RECT_H
#include "Vector2.h"
class Rect
{
public:
	Rect(const Vector2&,double,double);
	Rect(const Rect&);
	~Rect(void);
	double getHeight() const;
	double getWidth() const;
	Vector2 getOriginVector() const;
	bool contains( const Rect& rect )const;
	Rect();
	void rotate( void );

private:
	Vector2 mOrigin;
	Vector2 mWH;
};

#endif