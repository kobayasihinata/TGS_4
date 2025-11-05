#include "Explosion.h"
#include "../ObjectManager.h"

Explosion::Explosion()
{

}

Explosion::~Explosion()
{

}

void Explosion::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span, init_velocity);
	init_timer = _timer;

	ResourceManager* rm = ResourceManager::GetInstance();

	animation_image = rm->GetImages("Resource/Images/Effect/E_PuffAndStar.png", 60, 10, 6, 108, 116);

	image = animation_image[0];
}

//終了時処理
void Explosion::Finalize()
{

}

//更新処理
void Explosion::Update()
{
	__super::Update();

	//時間が過ぎたら自身を消す
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//描画処理
void Explosion::Draw()const
{
	__super::Draw();
}