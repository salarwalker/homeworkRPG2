#ifndef INCLUDED_ARRAY_2D_H
#define INCLUDED_ARRAY_2D_H

//二维数组类
//是否已经熟悉模板的使用了呢？如果还没的话请学习一下基础即可
//此类在声明时，把类T当作已经存在的类
//实际使用的时候把int bool等代入T使用
template< class T > class Array2D{
public:
	Array2D() : mArray( 0 ),mSize0( 0 ),mSize1( 0 ){}
	~Array2D(){
		SAFE_DELETE_ARRAY( mArray );
	}
	void setSize( int size0, int size1 ){
		SAFE_DELETE_ARRAY( mArray );
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[ size0 * size1 ];
	}
	
	int getSize() const{
		return mSize0 * mSize1;
	}

	T& operator()( int index0, int index1 ){
		return mArray[ index1 * mSize0 + index0 ];
	}
	const T& operator()( int index0, int index1 ) const {
		return mArray[ index1 * mSize0 + index0 ];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};

#endif
