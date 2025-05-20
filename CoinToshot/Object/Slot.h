#pragma once
#include "Base/ObjectBase.h"

#define REAL_NUM 9		//リールの要素数
//リール配列
static int ReelArray[3][REAL_NUM]
{
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	//{9,8,7,6,5,4,3,2,1},
};

class Slot :
	public ObjectBase
{
private:
	bool spin_flg;	//リールを回すか判断
	int reel[3];	//今止まっているリール
	int timer;		//フレーム測定
	int now_reel;	//今指しているリール
public:
	//コンストラクタ
	Slot();
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
};

