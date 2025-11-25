#pragma once
#define DEBUG_SKIP true			//デバッグ用チュートリアルスキップモード
#define DEBUG_PLAYER false		//クリアと死をすぐ確認したい時用
#define WINDOW_MODE true		//ウィンドウモードかどうか

#define SE_MAX 13000	//SE音量の上限
#define BGM_MAX 11000	//BGM音量の上限

#define FRAME_RATE 60			//フレームレート

#define SCREEN_WIDTH  (WINDOW_MODE ? 1280 : 1920) 	//画面横幅
#define SCREEN_HEIGHT (WINDOW_MODE ? 720 : 1080) 	//画面高さ

#define STAGE_SIZE 2000			//ステージの大きさ（中心から上下左右に2000pxずつ)

#define DEFAULT_TIMELIMIT 18000	//制限時間初期値(フレーム単位)

#define F_PI (float)(3.1415)	//円周率

#define DEFAULT_ANIM_SPAN 10	//アニメーション周期
#define DEFAULT_DEATH_TIMER 60	//死亡演出の時間

#define MOVE_LOWER_LIMIT 0.3f	//停止とみなすvelocityの下限
#define KNOCK_BACK 10			//ノックバック時の倍率

#define RANKING_DATA 10			//登録するランキングデータの数

#define HPBAR_WIDTH 50			//HPゲージの幅
#define HPBAR_HEIGHT 10			//HPゲージの高さ

#define FIRST_BONUS_TIME (DEFAULT_TIMELIMIT / 3) * 2	//時間経過ボーナス一回目の時間
#define SECOND_BONUS_TIME (DEFAULT_TIMELIMIT / 3)	//時間経過ボーナス二回目の時間
#define FIRST_BONUS_NUM 50		//時間経過ボーナス一回目の枚数
#define SECOND_BONUS_NUM 100		//時間経過ボーナス二回目の枚数

//コインの設定
#define COIN_FUSION_RANGE 10.f	//コインが融合する範囲
#define COIN_ATTRACTON_TIMER 600//コイン吸い寄せ時間

//プレイヤーのパラメータ関連
#define DEFAULT_HP 10				//プレイヤー初期HP
#define PLAYER_WIDTH 40				//プレイヤー幅
#define PLAYER_HEIGHT 40			//プレイヤー高さ
#define PLAYER_SPEED 4.f			//プレイヤー移動速度
#define PLAYER_DAMAGE_CD 120		//プレイヤー被弾後無敵時間（フレーム単位）
#define PLAYER_DAMAGE_STOP 30		//プレイヤー被弾後移動不能時間（フレーム単位）
#define PLATER_BULLET_CHANGE_CD 30	//プレイヤー弾種類変更可能間隔

//敵のパラメータ関連
#define ENEMY1_HP 2					//敵１HP
#define ENEMY1_WIDTH 40				//敵１幅
#define ENEMY1_HEIGHT 40			//敵１高さ
#define ENEMY1_SPEED 0.5f			//敵１速度
#define ENEMY1_DAMAGE 1				//敵１与ダメ
#define ENEMY1_DROPCOIN 6			//敵１コインドロップ量

#define ENEMY2_HP 5					//敵２HP
#define ENEMY2_WIDTH 50				//敵２幅
#define ENEMY2_HEIGHT 50			//敵２高さ
#define ENEMY2_SPEED 1.2f			//敵２速度
#define ENEMY2_DAMAGE 3				//敵２与ダメ
#define ENEMY2_DROPCOIN 14			//敵２コインドロップ量

#define ENEMY3_HP 1					//敵３HP
#define ENEMY3_WIDTH 50				//敵３幅
#define ENEMY3_HEIGHT 50			//敵３高さ
#define ENEMY3_SPEED 0.f			//敵３速度
#define ENEMY3_DAMAGE 2				//敵３与ダメ
#define ENEMY3_ATTACK_SPAN 180		//敵３攻撃周期
#define ENEMY3_ATTACK_SPEED 5		//敵３弾速度
#define ENEMY3_ATTACK_LIMIT 180		//敵３弾残留時間
#define ENEMY3_DROPCOIN 8			//敵３コインドロップ量

#define ENEMY4_HP 3					//敵４HP
#define ENEMY4_WIDTH 40				//敵４幅
#define ENEMY4_HEIGHT 40			//敵４高さ
#define ENEMY4_SPEED 3.f			//敵４速度
#define ENEMY4_ESCAPE_SPEED 0.5f	//敵４逃げ速度
#define ENEMY4_STEAL 15				//敵４盗むコイン量
#define ENEMY4_DAMAGE 0				//敵４与ダメ
#define ENEMY4_DROPCOIN 7			//敵４コインドロップ量

#define ENEMY5_HP 20				//敵５HP
#define ENEMY5_WIDTH 90				//敵５幅
#define ENEMY5_HEIGHT 130			//敵５高さ
#define ENEMY5_SPEED 0.2f			//敵５速度
#define ENEMY5_DAMAGE 2				//敵５与ダメ
#define ENEMY5_DROPCOIN 30			//敵５コインドロップ量

#define BOSS_BAR_SIZE 500			//ボスHPゲージ幅

#define BOSS1_HP 70				//ボス１HP
#define BOSS1_WIDTH 100				//ボス１幅
#define BOSS1_HEIGHT 100			//ボス１高さ
#define BOSS1_SPEED 1.f				//ボス１速度
#define BOSS1_DAMAGE 2				//ボス１与ダメ
#define BOSS1_DROPCOIN 100			//ボス１コインドロップ量

#define BOSS2_HP 80				//ボス２HP
#define BOSS2_WIDTH 100				//ボス２幅
#define BOSS2_HEIGHT 100			//ボス２高さ
#define BOSS2_SPEED 1.f				//ボス２速度
#define BOSS2_DAMAGE 3				//ボス２与ダメ
#define BOSS2_ATTACK_SPAN 180		//ボス２攻撃周期
#define BOSS2_ATTACK_SPEED 8		//ボス２弾速度
#define BOSS2_ATTACK_LIMIT 500		//ボス２弾残留時間
#define BOSS2_PLAYER_DISTANCE 400	//ボス２プレイヤーとの間合い
#define BOSS2_DROPCOIN 130			//ボス２コインドロップ量

#define BOSS3_HP 100				//ボス３HP
#define BOSS3_WIDTH 40				//ボス３幅
#define BOSS3_HEIGHT 40				//ボス３高さ
#define BOSS3_SPEED 2.5f			//ボス３速度
#define BOSS3_DAMAGE 3				//ボス３与ダメ
#define BOSS3_PLAYER_DISTANCE 200	//ボス３プレイヤーとの間合い
#define BOSS3_PLAYER_DISTANCE2 600	//ボス３プレイヤーとどれくらい離れたら加速するか
#define BOSS3_POWERUP_TIME 5400		//ボス３強化されるまでの時間
#define BOSS3_DROPCOIN 100			//ボス３コインドロップ量

//音の減衰関連 SA:減衰の開始距離(StartAttenuation) SD:どこまで聞こえるか(SoundDistance)

#define DEFAULT_START_ATTENUATION 600.f		//何ピクセル先から減衰が始まるか
#define DEFAULT_SOUND_DISTANCE	1200.f		//何ピクセル先の音まで聞こえるか(音の距離減衰処理用)

#define SA_BOSS3_SHOT 1000.f	//ボス３攻撃
#define SD_BOSS3_SHOT 2000.f	//ボス３攻撃

#define SA_ENEMY_HIT 1000.f		//敵被弾音
#define SD_ENEMY_HIT 1000.f		//敵被弾音