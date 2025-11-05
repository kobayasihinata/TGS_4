#pragma once
#include "../Base/EffectBase.h"

class Shine :
	public EffectBase
{
private:
	int color;				//エフェクト色
	int size;				//大きさ
	float angle;			//回転用
	float move_angle;		//回転用移動値保存
	Vector2D random_move;	//ランダムな方向に移動する

public:
	Shine(int _color);
	~Shine();

	//初期化処理
	void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)override;

	//終了時処理
	void Finalize()override;

	//更新処理
	void Update()override;

	//描画処理
	void Draw()const override;
};