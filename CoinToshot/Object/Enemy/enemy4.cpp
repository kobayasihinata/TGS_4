#include "Enemy4.h"
#include "../ObjectManager.h"

Enemy4::Enemy4()
{

	////‰æ‘œ“Ç
	//ResourceManager* rm = ResourceManager::GetInstance();
	//std::vector<int>tmp;
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/1.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/2.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/3.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/4.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/5.png");
	//animation_image.push_back(tmp[0]);
	//image = animation_image[0];
}

Enemy4::~Enemy4()
{

}

void Enemy4::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
}

void Enemy4::Finalize()
{

}

void Enemy4::Update()
{
	__super::Update();

	//‘Ì‚ÉUŒ‚‚ğ“Z‚¤
	manager->CreateAttack(
		{ this->location.x - 1,this->location.y - 1 },
		{ this->box_size.x + 2 ,this->box_size.y + 2 },
		this,
		0);

	//ˆÚ“®
	Move();

	//ƒAƒjƒ[ƒVƒ‡ƒ“
	Animation();
}

void Enemy4::Draw()const
{
	__super::Draw();
	//“G3
	DrawString(local_location.x, local_location.y, "enemy4", 0xffffff);
}

void Enemy4::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy4::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

