#ifndef INCLUDED_MATRIX23_H
#define INCLUDED_MATRIX23_H

class Vector2;

class Matrix23{
public:
	Matrix23(); //使用默认函数来构造
	//移動行列式生成
	void setTranslation( const Vector2& );
	//旋转行列式生成
	void setRotation( double radian );
	//拡大縮小行列式生成
	void setScaling( const Vector2& );
	//向量乘
	void multiply( Vector2* out, const Vector2& in ) const;
	//行列式乘
	void operator*=( const Matrix23& );
	//移動行列式右乘
	void translate( const Vector2& );
	//回転行列式右乘
	void rotate( double radian );
	//拡大縮小行列式右乘
	void scale( const Vector2& );
private:
	double m00, m01, m02;
	double m10, m11, m12;
};

#endif
