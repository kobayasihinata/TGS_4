#include "Magnet.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Magnet::Magnet()
{
	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Item/Magnet.png", 4, 4, 1, 64, 64);
	animation_image.push_back(tmp);
	image = animation_image[0][0];


	shine_image = rm->GetImages("Resource/Images/Effect/green_shine.png", 40, 8, 5, 96, 96);
	now_shine_image = 0;

	magnet_se = rm->GetSounds("Resource/Sounds/Player/Magnet.mp3");
}

Magnet::~Magnet()
{

}

void Magnet::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 5;
}

void Magnet::Finalize()
{

}

void Magnet::Update()
{
	Animation();

	//輝きアニメーション更新
	if (++now_shine_image > 39)
	{
		now_shine_image = 0;
	}
}

void Magnet::Draw()const
{
	//輝き描画
	DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, shine_image[now_shine_image], TRUE);

	__super::Draw();
}

void Magnet::Hit(ObjectBase* hit_object)
{
	//プレイヤーに触れたらスコア加算して消える
	if (hit_object->GetObjectType() == ePLAYER)
	{
		UserData::attraction_timer += COIN_ATTRACTON_TIMER;
		manager->DeleteObject(this);
		PlaySoundMem(magnet_se, DX_PLAYTYPE_BACK);
	}
}


