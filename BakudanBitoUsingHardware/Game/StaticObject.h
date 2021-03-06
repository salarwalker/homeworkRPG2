#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag{
		FLAG_NOTHING = 0,
		FLAG_WALL = 1,
		FLAG_CLOSEDGATE = 2, 
		FLAG_OPENEDGATE = 3,
		FLAG_CLOSEDBOX = 4,
		FLAG_OPENEDBOX = 5,
		FLAG_PORTAL = 6,
	};
	StaticObject();
	void set(int cellX,int cellY,Flag f);
	bool checkFlag( Flag );
	void setFlag( Flag );
	void clearFlag();
	static Flag randomMechanism();

	//绘制地板、墙壁、砖块、有炸弹和道具也绘制
	virtual void draw( const Image* ) const;
	int mCellX;
	int mCellY;
private:
	Flag mFlag; //标记
	
};

#endif
