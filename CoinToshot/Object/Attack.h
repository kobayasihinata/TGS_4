#pragma once
#include "Base/ObjectBase.h"
#include "Base/BulletData.h"
#include "Player/PlayerBullet.h"

class Attack :
	public ObjectBase
{
private:
	ObjectBase* object;			//攻撃を生成したオブジェクト いなければnullptr
	float damage;				//攻撃力を格納
	int time;					//存在できる時間
	int count_up;				//生成されてからの時間測定
	Vector2D move_velocity;		//移動方向
	int hit_count;				//当たったオブジェクトの数を測定
	int hit_max;				//いくつのオブジェクトに当たれるか
	ObjectBase* old_hit_object;	//ひとつ前に当たったオブジェクトを保存
	BulletType bullet_type;		//自身の弾種類を格納
public:
	//コンストラクタ(_object = 攻撃を生成したオブジェクト いなければnullptr 
	// _bullet_data = 発射時に必要な情報)
	Attack(BulletData _bullet_data);
	//デストラクタ
	~Attack();
	//初期化処理
	void Initialize(ObjectManager* _manager , int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f,float init_radius = 0.f)override;
	//終了時処理
	void Finalize()override;
	//更新処理
	void Update()override;
	//描画処理
	void Draw()const override;
	//当たり判定が被った時の処理
	void Hit(ObjectBase* hit_Object)override;
	//ダメージ処理 value=ダメージの値 location=攻撃の中心座標 
	void Damage(float _value,Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override {};

	//弾の情報を取得
	BulletData GetBulletData(float _shot_rad);
};

