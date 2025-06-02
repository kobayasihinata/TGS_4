#pragma once
#include "../Base/ActorBase.h"
#include "../../Scene/Camera/Camera.h"
class InGameScene;

class Coin :
	public ActorBase
{
private:
	InGameScene* ingame;		//現在のシーンのポインタを保存
	class Camera* camera;		//カメラポインタ格納(プレイヤーの情報を受け取るため)

	int color_r;
	int color_g;
	int color_b;
	//音源
	int get_se;				//コイン獲得SE

public:
	float add_num;	//コイン追加数
	bool check_flg;	//コインが合体するかを一度だけ判断する用	

public:
	//コンストラクタ
	Coin(InGameScene* _ingame,Vector2D _init_velocity = 0.f);
	//デストラクタ
	~Coin();
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

