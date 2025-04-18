#include "Enemy3.h"
#include "../ObjectManager.h"

Enemy3::Enemy3()
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

Enemy3::~Enemy3()
{

}

void Enemy3::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Enemy3::Finalize()
{

}

void Enemy3::Update()
{
	__super::Update();

	//移動
	Move();

	//アニメーション
	Animation();
}

void Enemy3::Draw()const
{
	__super::Draw();
	//敵3
	DrawString(local_location.x, local_location.y, "enemy3", 0xffffff);
}

void Enemy3::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy3::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

