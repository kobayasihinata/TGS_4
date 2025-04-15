#include "Enemy1.h"
#include "../ObjectManager.h"

Enemy1::Enemy1()
{

	////画像読込
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

Enemy1::~Enemy1()
{

}

void Enemy1::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
}

void Enemy1::Finalize()
{

}

void Enemy1::Update()
{
	__super::Update();

	//体に攻撃を纏う
	manager->CreateAttack(
		{ this->location.x - 1,this->location.y - 1 },
		{ this->box_size.x + 2 ,this->box_size.y + 2 },
		this,
		0);

	//移動
	Move();

	//アニメーション
	Animation();

	//地面判定リセット
	on_floor = false;
}

void Enemy1::Draw()const
{
	__super::Draw();
	//敵1
	DrawString(local_location.x, local_location.y, "enemy1", 0xffffff);
}

void Enemy1::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);

}

void Enemy1::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

