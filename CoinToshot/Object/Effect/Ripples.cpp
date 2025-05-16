#include "Ripples.h"
#include "../ObjectManager.h"

Ripples::Ripples()
{

}

Ripples::~Ripples()
{

}

void Ripples::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span);
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
	__super::Draw();
}