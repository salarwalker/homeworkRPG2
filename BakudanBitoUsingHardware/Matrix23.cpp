#include "Matrix23.h"
#include "Vector2.h"
#include "GameLib/Math.h"
using namespace GameLib;

Matrix23::Matrix23(){
}

void Matrix23::setTranslation( const Vector2& a ){
	m00 = m11 = 1.0;
	m01 = m10 = 0.0;
	m02 = a.x;
	m12 = a.y;
}

void Matrix23::setRotation( double r ){
	double s = sin( r );
	double c = cos( r );
	m00 = m11 = c;
	m01 = s;
	m10 = -s;
	m02 = m12 = 0.0;
}

void Matrix23::setScaling( const Vector2& a ){
	m00 = a.x;
	m11 = a.y;
	m01 = m10 = m02 = m12 = 0.0;
}

void Matrix23::operator*=( const Matrix23& m ){
	double tx, ty;
	tx = m00; ty = m01;
	m00 = tx * m.m00 + ty * m.m10;
	m01 = tx * m.m01 + ty * m.m11;
	m02 = tx * m.m02 + ty * m.m12 + m02;
	tx = m10; ty = m11;
	m10 = tx * m.m00 + ty * m.m10;
	m11 = tx * m.m01 + ty * m.m11;
	m12 = tx * m.m02 + ty * m.m12 + m12;
}

void Matrix23::multiply( Vector2* out, const Vector2& in ) const {
	double tx = in.x; //out和in也许相同，在此做下备份
	out->x = m00 * tx + m01 * in.y + m02; //左上*x + 中上*y + 右上*1
	out->y = m10 * tx + m11 * in.y + m12; //左下*x + 中下*y + 左下*1
}

//高速计算移动的行列式乘的函数
/*
a b c
d e f

和

1 0 tx
0 1 ty

相乘。扩张至3x3的话、如下

a b c   1 0 tx
d e f   0 1 ty
0 0 1   0 0 1

第3行怎么算都不变、只计算上两行

a b (a*tx + b*ty + c)
d e (d*tx + e*ty + f)

会得到这个结果
也就是，只改变了m02和m12，所以也只需计算m02和m12
*/

void Matrix23::translate( const Vector2& a ){
	m02 += m00 * a.x + m01 * a.y; 
	m12 += m10 * a.x + m11 * a.y;
}

//高速计算扩大缩小的行列式乘的函数
/*
a b c
d e f

和

sx 0 0
0 sy 0

相乘。

a b c   sx 0  0
d e f   0  sy 0
0 0 1   0  0  1

其中若计算上2行、

a*sx b*sy c
d*sx e*sy f

会得到这个结果。
即是，只需要计算m00,m01,m10,m11这4个元素
*/
void Matrix23::scale( const Vector2& a ){
	m00 *= a.x;
	m01 *= a.y;
	m10 *= a.x;
	m11 *= a.y;
}

//高速计算旋转的行列式乘的函数
/*
a b c
d e f

和

p -q 0
q p 0

相乘。p是cosα、q是sinα。（α是旋转角度）

a b c   p -q 0
d e f   q  p 0
0 0 1   0  0 1

只计算上两行、

(ap + bq) (-aq + bp) c
(dp + eq) (-dq + ep) f

即是，只需要计算m00,m01,m10,m11这4个元素
*/

void Matrix23::rotate( double r ){
	double c = cos( r );
	double s = sin( r );
	//m00从备份中取回。因为m00在第一行就有
	double t = m00;
	m00 = t * c + m01 * s;
	m01 = t * ( -s ) + m01 * c;
	t = m10; //备份m10
	m10 = t * c + m11 * s;
	m11 = t * ( -s ) + m11 * c;
}
