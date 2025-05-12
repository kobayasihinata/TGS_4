#pragma once

#define FRAME_RATE 60			//フレームレート

#define SCREEN_WIDTH  1280		//画面横幅
#define SCREEN_HEIGHT 720		//画面高さ

#define STAGE_SIZE 2000			//ステージの大きさ（中心から上下左右に2000pxずつ)

#define DEFAULT_TIMELIMIT 18000	//制限時間初期値(フレーム単位)

#define F_PI (float)(3.1415)				//円周率

#define DEFAULT_ANIM_SPAN 10	//アニメーション周期
#define DEFAULT_DEATH_TIMER 60	//死亡演出の時間

#define MOVE_LOWER_LIMIT 0.3f	//停止とみなすvelocityの下限
#define KNOCK_BACK 10			//ノックバック時の倍率

#define RANKING_DATA 10			//登録するランキングデータの数

//コインの設定
#define COIN_FUSION_RANGE 35.f	//コインが融合する範囲

//プレイヤーのパラメータ関連
#define DEFAULT_HP 2			//プレイヤー初期HP
#define DEFAULT_LIFE 0			//プレイヤー初期残機
#define PLAYER_WIDTH 40			//プレイヤー幅
#define PLAYER_HEIGHT 40		//プレイヤー高さ
#define PLAYER_SPEED 4.f		//プレイヤー移動速度
#define PLAYER_DAMAGE_CD 120	//プレイヤー被弾後無敵時間（フレーム単位）
#define PLAYER_DAMAGE_STOP 30	//プレイヤー被弾後移動不能時間（フレーム単位）

//敵のパラメータ関連
#define ENEMY1_HP 2				//敵１HP
#define ENEMY1_SPEED 0.5f		//敵１速度
#define ENEMY1_DAMAGE 1			//敵１与ダメ
#define ENEMY1_DROPCOIN 5		//敵１コインドロップ量

#define ENEMY2_HP 5				//敵２HP
#define ENEMY2_SPEED 1.2f		//敵２速度
#define ENEMY2_DAMAGE 3			//敵２与ダメ
#define ENEMY2_DROPCOIN 10		//敵２コインドロップ量

#define ENEMY3_HP 3				//敵３HP
#define ENEMY3_SPEED 0.f		//敵３速度
#define ENEMY3_DAMAGE 3			//敵３与ダメ
#define ENEMY3_ATTACK_SPAN 180	//敵３攻撃周期
#define ENEMY3_ATTACK_SPEED 5	//敵３弾速度
#define ENEMY3_ATTACK_LIMIT 180	//敵３弾残留時間
#define ENEMY3_DROPCOIN 7		//敵３コインドロップ量

#define ENEMY4_HP 3				//敵４HP
#define ENEMY4_SPEED 3.f		//敵４速度
#define ENEMY4_ESCAPE_SPEED 0.5f//敵４逃げ速度
#define ENEMY4_STEAL 10			//敵４盗むコイン量
#define ENEMY4_DAMAGE 0			//敵４与ダメ
#define ENEMY4_DROPCOIN 5		//敵４コインドロップ量

#define ENEMY5_HP 10			//敵５HP
#define ENEMY5_SPEED 0.5f		//敵５速度
#define ENEMY5_DAMAGE 2			//敵５与ダメ
#define ENEMY5_DROPCOIN 30		//敵５コインドロップ量