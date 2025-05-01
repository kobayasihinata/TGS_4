#include "Enemy1.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy1::Enemy1()
{
	move_speed = ENEMY1_SPEED;
	hp = ENEMY1_HP;
	hit_damage = ENEMY1_DAMAGE;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = ENEMY1_DROPCOIN + (GetRand(2) - 1);

	////画像読込
	//ResourceManager* rm = ResourceManager::GetInstance();
	//std::vector<int>tmp;
	//tmp = rm->GetImages("Resource/Images/Enemy1/0_Skeleton_Crusader_Run Slashing_000.png");
	//animation_image.push_back(tmp);
	////tmp = rm->GetImages("Resource/Images/Player/Run2.png", 12, 12, 1, 64, 64);
	////animation_image.push_back(tmp);
	//image = animation_image[0][0];
	
}

Enemy1::~Enemy1()
{

}

void Enemy1::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Enemy1::Finalize()
{

}

void Enemy1::Update()
{
	__super::Update();

	//移動
	Move();

	//アニメーション
	Animation();

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡演出時間を過ぎたら自身を削除
		if (--death_timer <= 0)
		{
			manager->DeleteObject(this);
			//コインをドロップ
			for (int i = 0; i < drop_coin; i++)
			{
				manager->CreateObject({ Vector2D{this->location.x + (float)(GetRand(50) - 25),this->location.y + (float)(GetRand(50) - 25)},Vector2D{40,40},eCOIN, 20.f });
			}
		}
	}
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
	
	//location -= (velocity * 10);
}

void Enemy1::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);

}

