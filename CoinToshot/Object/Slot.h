#pragma once
#include "Base/ObjectBase.h"

#define REEL_NUM 9		//リールの要素数
#define PEKA 50			//光る確率 〇分の１
#define REEL_WAIT 120	//待ち時間
#define BIG_BONUS 240	//ドロップコイン量

class InGameScene;

//リール配列
static int ReelArray[3][REEL_NUM]
{
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{9,8,7,6,5,4,3,2,1},
};

class Slot :
	public ObjectBase
{
private:
	InGameScene* ingame;		//現在のシーンのポインタを保存

	int frame;		//フレーム測定
	bool can_stop;	//プレイヤーが触れている間だけリールに触れる
	bool peka_flg;	//目押しが可能な状態か判断
	bool bet_once;	//コイン消費用
	int reel[3];	//今止まっているリール
	int reel_wait;	//リールの時間
	int stop_reel_num;	//今止まっているリールの数
	int timer;		//フレーム測定
	int now_reel[3];	//今指しているリール

	int drop_coin;			//ドロップするコインの量
	int drop_coin_count;	//ドロップするコインの計測

	Vector2D real_location;	//リール描画位置
	Vector2D real_size;		//リール描画箱大きさ
	Vector2D hana_location;	//華描画位置

	int reel_se;			//リール回転音SE
	int button_se;			//ボタン停止SE
	int bonus_se;			//ボーナスSE
	bool bonus_se_play_once;//ボーナスSEを一回だけ再生
public:
	//コンストラクタ
	Slot(InGameScene* _ingame);
	//デストラクタ
	~Slot();
	//初期化処理
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//終了時処理
	void Finalize()override;
	//更新処理
	void Update()override;
	//描画処理
	void Draw()const override;
	//当たり判定が被った時の処理
	void Hit(ObjectBase* hit_object)override;
	//ダメージ処理
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;
	//自動消化処理
	void AutoPlay();
	//目押しの処理
	void BonusStop();
	//他のスロットに使うボタンが押されているか調べる(_button = 押そうとしているボタン)
	bool CheckButton(int _button);
	//７が揃っているか調べる
	bool CheckBigBonus()const;
};

