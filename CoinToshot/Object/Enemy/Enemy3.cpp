#include "Enemy3.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy3::Enemy3()
{
	
	move_speed = ENEMY3_SPEED;
	max_hp = hp = ENEMY3_HP;
	hit_damage = ENEMY3_DAMAGE;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = ENEMY3_DROPCOIN + (GetRand(2) - 1);

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


	//指定の周期毎に弾を発射する
	if (!death_flg && frame % ENEMY3_ATTACK_SPAN == 0)
	{
		//プレイヤーの位置で発射角度を決める
		shot_rad = atan2f(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		manager->CreateAttack(GetBulletData());
	}

	//死亡演出フラグが立っているなら
	if (death_flg)
	{

		//死にながらコインをまき散らす
		if (death_timer % 20 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
			//煙エフェクト生成
			manager->CreateEffect(elSmoke, this->location + rand, true, 20);
		}

		//死亡演出時間を過ぎたら自身を削除
		if (--death_timer <= 0)
		{
			manager->DeleteObject(this);
			//演出中に出せなかったコインをまとめてドロップ
			for (int i = drop_coin_count; i < drop_coin; i++)
			{
				Vector2D rand = { (float)(GetRand(20) - 10),(float)(GetRand(20) - 10) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{ 40, 40 },
					20.f,
					rand);
			}

			//煙エフェクト生成
			manager->CreateEffect(elSmoke, this->location, true, 40);
		}
	}
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

void Enemy3::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc,_knock_back);
}

BulletData Enemy3::GetBulletData()
{
	BulletData _data;
	_data.b_angle = shot_rad;
	_data.damage = ENEMY3_DAMAGE;
	_data.delete_time = ENEMY3_ATTACK_LIMIT;
	_data.h_count = 1;
	_data.location = this->location;
	_data.radius = 15;
	_data.speed = ENEMY3_ATTACK_SPEED;
	_data.who = this;

	return _data;
}
