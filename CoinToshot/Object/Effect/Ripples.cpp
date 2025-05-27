#include "Ripples.h"
#include "../ObjectManager.h"

Ripples::Ripples(int _color)
{
	color = _color;
}

Ripples::~Ripples()
{

}

void Ripples::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span);
	init_timer = _timer;
}

//終了時処理
void Ripples::Finalize()
{

}

//更新処理
void Ripples::Update()
{
	//時間が過ぎたら自身を消す
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//描画処理
void Ripples::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 / init_timer) * timer);
	DrawCircleAA(local_location.x, local_location.y, init_timer - timer, 20, color, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}