#pragma once
#include "../../Base/BossBase.h"
#include "../../Base/BulletData.h"
#include "../../Player/PlayerBullet.h"

#define RUSH_SPAN  300	//突撃の頻度
#define RUSH_SPEED 100	//突撃の速度
class Boss3 :
	public BossBase
{
private:

	int coin_num;				//コイン所持数
	int move_mode;				//移動タイプ格納

	//攻撃発射関連
	int bullet_type;			//弾種類

	float shot_rad;				//発射角度
	bool shot_once;				//一回だけ撃つ
	int shot_span;				//発射間隔
	float shot_speed;			//弾速度
public:
	static InGameScene* ingame;		//現在のシーンのポインタを保存
	static bool boss3_once;		//プレイヤー生成を一回だけにする
private:
	//コンストラクタをprivateにすることで、
//自クラスのメンバ関数でインスタンスを生成できないようにする
	Boss3() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	Boss3(Boss3& v) = default;
	Boss3& operator = (const Boss3& v) = delete;
	~Boss3() = default;
public:

	//インスタンスを取得する処理
	static Boss3* Get(InGameScene* _ingame);

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
	BulletData GetBulletData(float _shot_rad);

	//移動方向管理
	void MoveBoss3();

	//弾を撃つ
	void ShotBullet();
};

