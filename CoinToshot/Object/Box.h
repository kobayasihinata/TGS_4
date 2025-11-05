#pragma once
#include "Base/ActorBase.h"

#define ITEM_DROP 30  //レアドロップ確率

class Box :
	public ActorBase
{
private:

	int impact;			//箱の振動

	int death_se;				//死亡時SE
	int item_spawn_se;			//レアアイテム生成SE

public:
	//コンストラクタ
	Box();
	//デストラクタ
	~Box();
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
};

