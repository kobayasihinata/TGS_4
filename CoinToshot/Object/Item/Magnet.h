#pragma once
#include "../Base/ObjectBase.h"
#include <vector>

class Magnet :
	public ObjectBase
{
private:

	std::vector<int> shine_image;	//オーラ画像格納
	int now_shine_image;			//現在描画オーラ画像

	int magnet_se;					//取得時SE
public:
	//コンストラクタ
	Magnet();
	//デストラクタ
	~Magnet();
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
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override {};
};

