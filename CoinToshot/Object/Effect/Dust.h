#pragma once
#include "../Base/EffectBase.h"

class Dust :
	public EffectBase
{
private:
	Vector2D move_velocity;	//移動方向
	Vector2D box_size;		//大きさ

	int timer;				//消えるまでの時間
	int color;				//描画の色

public:
	Dust(int _color);
	~Dust();

	//初期化処理
	void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)override;

	//終了時処理
	void Finalize()override;

	//更新処理
	void Update()override;

	//描画処理
	void Draw()const override;
};