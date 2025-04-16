#include "Coin.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Coin::Coin(InGameScene* _ingame)
{
	add_num = 1;
	ingame = _ingame;
}

Coin::~Coin()
{

}

void Coin::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
}

void Coin::Finalize()
{

}

void Coin::Update()
{

}

void Coin::Draw()const
{
	__super::Draw();
	//コイン仮
	DrawCircleAA(local_location.x + (box_size.x / 2), local_location.y + (box_size.y / 2), (box_size.x / 2), 10, 0xffff00, true);
}

void Coin::Hit(ObjectBase* hit_object)
{
	//プレイヤーに触れたらスコア加算して消える
	if (hit_object->GetObjectType() == ePLAYER)
	{
		UserData::coin += add_num;
		//ポップアップ表示
		ingame->CreatePopUp(this->location, "+1", GetRand(100), 0xffff00, 1, 60);
		manager->DeleteObject(this);
	}
}


