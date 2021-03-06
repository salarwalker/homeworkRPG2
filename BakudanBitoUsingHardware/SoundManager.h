#ifndef INCLUDED_SOUND_MANAGER_H
#define INCLUDED_SOUND_MANAGER_H

#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"

//シングルトン
class SoundManager{
public:
	static SoundManager* instance();
	static void create();
	static void destroy();
	bool hasLoaded(); //载入是否结束？

	enum Bgm{
		BGM_TITLE, //标题画面
		BGM_GAME, //游戏中

		BGM_MAX,
	};
	enum Se{
		SE_EXPLOSION, //爆炸
		SE_DEATH, //死亡
		SE_CURSOR_MOVE, //取消音效
		SE_SELECTION, //选择音效
		SE_SET_BOMB, //爆弾設置音

		SE_MAX,
	};
	//播放BGM。一般每次只有一个BGM。另，使其循环播放。
	void playBgm( Bgm );
	void stopBgm();
	//播放SE（Sound Effect）。SE只在该播放时才播放一次。
	void playSe( Se );
private:
	SoundManager();
	~SoundManager();

	//Wave之前已经载入。
	GameLib::Sound::Wave mBgmWaves[ BGM_MAX ];
	GameLib::Sound::Wave mSeWaves[ SE_MAX ];
	//Player是播放时生成
	GameLib::Sound::Player mBgmPlayer; //只生成一个，预备使用
	static const int SE_PLAYER_MAX = 4; //同时播放的SE最多4个。由旧的开始停止。
	GameLib::Sound::Player mSePlayers[ SE_PLAYER_MAX ]; //这里填入数组，每帧检查一次，播放完了就删除。
	int mSePlayerPos; //SE播放的下标。

	static SoundManager* mInstance;
};

#endif
