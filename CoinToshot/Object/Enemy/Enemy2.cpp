#include "Enemy2.h"
#include "../ObjectManager.h"

Enemy2::Enemy2()
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

Enemy2::~Enemy2()
{

}

void Enemy2::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
}

void Enemy2::Finalize()
{

}

void Enemy2::Update()
{
	__super::Update();

	//移動
	Move();

	//アニメーション
	Animation();
}

void Enemy2::Draw()const
{
	__super::Draw();
	//敵2
	DrawString(local_location.x, local_location.y, "enemy2", 0xffffff);
}

void Enemy2::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy2::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

