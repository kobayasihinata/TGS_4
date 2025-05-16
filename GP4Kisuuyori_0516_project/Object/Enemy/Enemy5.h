#pragma once
#include "../Base/EnemyBase.h"
#include <vector>
using namespace std;

struct BlockData {
	int block_anim_timer;	//防御エフェクトアニメーション
	Vector2D attack_loc;	//アニメーション用 攻撃の座標

	bool operator ==  (const BlockData _data)
	{
		if (this->block_anim_timer != _data.block_anim_timer)return false;
		if (this->attack_loc != _data.attack_loc)return false;
		return true;
	}
};
class Enemy5 :
	public EnemyBase
{
private:
	std::vector<BlockData> block_data;	//攻撃を防いだ時にエフェクトを出す用
	std::vector<BlockData> delete_block_data;	//攻撃を防いだ時にエフェクトを出す用

public:
	//コンストラクタ
	Enemy5();
	//デストラクタ
	~Enemy5();
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

};

