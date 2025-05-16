#include "Heal.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"

Heal::Heal()
{
	heal_num = 100;
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

}

void Heal::Draw()const
{
	__super::Draw();
	//コイン仮
	DrawCircleAA(local_location.x + (box_size.x / 2), local_location.y + (box_size.y / 2), (box_size.x / 2), 10, 0xff0000, true);
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


