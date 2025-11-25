#pragma once
#include "../../Base/BossBase.h"
#include "../../Base/BulletData.h"

#define RUSH_SPAN  300	//突撃の頻度
#define RUSH_SPEED 100	//突撃の速度
class Boss2 :
	public BossBase
{
private:
	InGameScene* ingame;		//現在のシーンのポインタを保存

	bool boss_move_flg;			//移動するか判断

	//攻撃発射関連
	float shot_rad;				//発射角度
	bool shot_once;				//一回だけ撃つ
	int shot_span;				//発射間隔
	float shot_speed;			//弾速度

	int shot_se;				//弾発射SE
public:
	//コンストラクタ
	Boss2(InGameScene* _ingame);
	//デストラクタ
	~Boss2();
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

	//弾を撃つ関連の処理
	void Bullet();
	
	// 弾を発射するための情報をまとめる
	BulletData GetBulletData();

	//プレイヤーの周囲を回る
	void MoveAround();
};

