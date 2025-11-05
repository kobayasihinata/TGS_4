#include "Walk.h"
#include "../ObjectManager.h"

Walk::Walk(bool _direction)
{
	direction = _direction;
}

Walk::~Walk()
{

}

void Walk::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span, init_velocity);
	init_timer = _timer;

	ResourceManager* rm = ResourceManager::GetInstance();

	animation_image = rm->GetImages("Resource/Images/Effect/Walk.png", 12, 12, 1, 62, 64);

	image = animation_image[0];
}

//終了時処理
void Walk::Finalize()
{

}

//更新処理
void Walk::Update()
{
	__super::Update();

	//時間が過ぎたら自身を消す
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//描画処理
void Walk::Draw()const
{
	//画像描画
	if (image != 0)
	{
		DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0.f, image, TRUE, direction);
	}
}