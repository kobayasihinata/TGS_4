#include "Smoke.h"
#include "../ObjectManager.h"

Smoke::Smoke()
{

}

Smoke::~Smoke()
{

}

void Smoke::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span, init_velocity);
	init_timer = _timer;

	ResourceManager* rm = ResourceManager::GetInstance();

	switch (GetRand(4))
	{
	case 0:
		animation_image = rm->GetImages("Resource/Images/Effect/Smoke1.png", 12, 12, 1, 64, 66);
		break;
	case 1:
		animation_image = rm->GetImages("Resource/Images/Effect/Smoke3.png", 12, 12, 1, 64, 64);
		break;
	case 2:
		animation_image = rm->GetImages("Resource/Images/Effect/Smoke4.png", 12, 12, 1, 64, 64);
		break;
	case 3:
		animation_image = rm->GetImages("Resource/Images/Effect/Smoke5.png", 12, 12, 1, 64, 64);
		break;
	case 4:
		animation_image = rm->GetImages("Resource/Images/Effect/Smoke6.png", 12, 12, 1, 64, 64);
		break;
	default:
		break;
	}
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