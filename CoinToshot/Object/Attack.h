#pragma once
#include "Base/ObjectBase.h"

class Attack :
	public ObjectBase
{
private:
	ObjectBase* object;		//攻撃を生成したオブジェクト いなければnullptr
	int time;				//存在できる時間
	int count_up;			//生成されてからの時間測定
public:
	//コンストラクタ(_object = 攻撃を生成したオブジェクト いなければnullptr 
	// _time = 存在できる時間)
	Attack(ObjectBase* _object,int _time,float _angle = 0);
	//デストラクタ
	~Attack();
	//初期化処理
	void Initialize(ObjectManager* _manager , int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f)override;
	//終了時処理
	void Finalize()override;
	//更新処理
	void Update()override;
	//描画処理
	void Draw()const override;
	//当たり判定が被った時の処理
	void Hit(ObjectBase* hit_Object)override;
	//ダメージ処理 value=ダメージの値 location=攻撃の中心座標 
	void Damage(float _value,Vector2D _attack_loc)override {};
};

