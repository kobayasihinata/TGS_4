#pragma once
#include "../Base/EffectBase.h"

class Walk :
	public EffectBase
{
private:
	bool direction;		//エフェクト向き
public:
	Walk(bool _direction);
	~Walk();

	//初期化処理
	void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)override;

	//終了時処理
	void Finalize()override;

	//更新処理
	void Update()override;

	//描画処理
	void Draw()const override;
};