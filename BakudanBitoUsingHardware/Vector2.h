#ifndef INCLUDED_VECTOR2_H
#define INCLUDED_VECTOR2_H

class Vector2{
public:
	//默认构造函数（以下是重载）
	Vector2();
	//整数构造函数
	Vector2( int x, int y );
	//double构造函数
	Vector2( double x, double y );
	//Vector2构造函数
	Vector2( const Vector2& );
	//加法（以下是重载操作符）
	void operator+=( const Vector2& );
	//减法
	void operator-=( const Vector2& );
	//赋值
	Vector2 &operator=( const Vector2& );
	//数乘
	Vector2 &operator*( double );
	//加法
	Vector2 &operator+( const Vector2& );
	//减法
	Vector2 &operator-( const Vector2& );
	//设置加法
	void setAdd( const Vector2&, const Vector2& );
	//设置减法
	void setSub( const Vector2&, const Vector2& );
	//设置插值( this = a + u*ab + v*ac )
	void setInterporation( 
		const Vector2& a, 
		const Vector2& ab, 
		const Vector2& ac, 
		double u, 
		double v );

	double x;
	double y;
};

#endif
