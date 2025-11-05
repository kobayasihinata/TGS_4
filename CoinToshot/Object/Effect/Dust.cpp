#include "Dust.h"
#include "../ObjectManager.h"

Dust::Dust(int _color)
{
	color = _color;

	move_velocity = { 0,0 };
	box_size = { GetRand(10) + 5,GetRand(10) + 5 };
	init_timer = timer = GetRand(30) + 15;
}

Dust::~Dust()
{

}

void Dust::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span, init_velocity);
	init_timer = _timer;
	move_velocity = init_velocity;
}

//終了時処理
void Dust::Finalize()
{

}

//更新処理
void Dust::Update()
{
	//移動＆減速
	location += move_velocity;
	move_velocity.x -= move_velocity.x / 10;
	move_velocity.y -= move_velocity.y / 10;
	//時間が過ぎたら自身を消す
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//描画処理
void Dust::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 / init_timer) * timer);
	DrawBoxAA(local_location.x, local_location.y, local_location.x + box_size.x, local_location.y + box_size.y, color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}