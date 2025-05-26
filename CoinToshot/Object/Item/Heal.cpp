#include "Heal.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"

Heal::Heal()
{
	heal_num = 5;

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy1/Enemy1_Walk.png", 24, 5, 5, 96, 96);
	animation_image.push_back(tmp);
	image = animation_image[0][0];
}

Heal::~Heal()
{

}

void Heal::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Heal::Finalize()
{

}

void Heal::Update()
{
	Animation();
}

void Heal::Draw()const
{
	__super::Draw();
}

void Heal::Hit(ObjectBase* hit_object)
{
	//プレイヤーに触れたらスコア加算して消える
	if (hit_object->GetObjectType() == ePLAYER)
	{
		UserData::player_hp += heal_num;
		manager->DeleteObject(this);
	}
}


