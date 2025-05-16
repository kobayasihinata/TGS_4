#include "Smoke.h"
#include "../ObjectManager.h"

Smoke::Smoke()
{

}

Smoke::~Smoke()
{

}

void Smoke::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span);
	init_timer = _timer;

	ResourceManager* rm = ResourceManager::GetInstance();
	animation_image = rm->GetImages("Resource/Images/Effect/Smoke.png", 12, 12, 1, 64, 66);
	image = animation_image[0];
}

//終了時処理
void Smoke::Finalize()
{

}

//更新処理
void Smoke::Update()
{
	__super::Update();

	//時間が過ぎたら自身を消す
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//描画処理
void Smoke::Draw()const
{
	__super::Draw();
}