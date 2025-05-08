#pragma once
#include "../Base/EnemyBase.h"
class Enemy4 :
	public EnemyBase
{
private:
	int coin_num;	//持っているコイン
	bool steal_flg;	//コインを盗んだか

public:
	//コンストラクタ
	Enemy4();
	//デストラクタ
	~Enemy4();
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
	void Damage(float _value, Vector2D _attack_loc)override;

	//Enemy4専用移動処理
	void Enemy4Move();
};

