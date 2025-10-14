#pragma once
#include "../../Base/BossBase.h"

#define RUSH_SPAN  300	//突撃の頻度
#define RUSH_SPEED 100	//突撃の速度
class Boss1 :
	public BossBase
{
private:
	InGameScene* ingame;		//現在のシーンのポインタを保存
	int cooldown_timer;			//突撃後の時間測定
	bool rush_flg;				//突進モーションフラグ
	int  rush_timer;			//突進時間測定
	double rush_radian;			//突進方向
	int rush_num;				//突進回数
	int rush_num_count;			//突進回数測定

public:
	//コンストラクタ
	Boss1(InGameScene* _ingame);
	//デストラクタ
	~Boss1();
	//初期化処理
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//終了時処理
	void Finalize()override;
	//更新処理
	void Update()override;
	//描画処理
	void Draw()const override;
	//当たり判定が被った時の処理
	void Hit(ObjectBase* hit_Object)override;
	//ダメージ処理
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;

	//攻撃モーション（突進）処理
	void Rush();

	//攻撃回数決定
	void SetRushNum();
};

