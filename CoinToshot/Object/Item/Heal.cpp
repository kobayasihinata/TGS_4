#include "Heal.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Heal::Heal(InGameScene* _ingame, Vector2D _init_velocity)
{
	ingame = _ingame;
	velocity = _init_velocity;
	heal_num = 5;

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Item/Heal.png", 7, 5, 2, 32, 41);
	animation_image.push_back(tmp);
	image = animation_image[0][0];


	shine_image = rm->GetImages("Resource/Images/Effect/green_shine.png", 40, 8, 5, 96, 96);
	now_shine_image = 0;

	heal_se = rm->GetSounds("Resource/Sounds/Player/Heal.mp3");
}

Heal::~Heal()
{

}

void Heal::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 5;
}

void Heal::Finalize()
{

}

void Heal::Update()
{
	Move();

	Animation();

	//輝きアニメーション更新
	if (++now_shine_image > 39)
	{
		now_shine_image = 0;
	}
}

void Heal::Draw()const
{
	//輝き描画
	DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, shine_image[now_shine_image], TRUE);

	__super::Draw();
}

void Heal::Hit(ObjectBase* hit_object)
{
	//プレイヤーに触れたらスコア加算して消える
	if (hit_object->GetObjectType() == ePLAYER)
	{
		UserData::player_hp += heal_num;
		manager->DeleteObject(this);

		std::string s = "HP + " + std::to_string((int)heal_num);
		ingame->CreatePopUp(this->location, s,0x00ff22);
		PlaySoundMem(heal_se,DX_PLAYTYPE_BACK);
	}
}


