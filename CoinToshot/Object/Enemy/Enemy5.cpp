#include "Enemy5.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy5::Enemy5()
{
	move_speed = ENEMY5_SPEED;
	max_hp = hp = ENEMY5_HP;
	hit_damage = ENEMY5_DAMAGE;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = ENEMY5_DROPCOIN + (GetRand(2) - 1);

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy5/Enemy5_Walk2.png", 24, 5, 5, 200, 200);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy5/Enemy5_Death.png", 15, 5, 3, 200, 200);
	animation_image.push_back(tmp);

	image = animation_image[0][0];
}

Enemy5::~Enemy5()
{

}

void Enemy5::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 5;
}

void Enemy5::Finalize()
{

}

void Enemy5::Update()
{
	__super::Update();

	//移動
	Move();

	//アニメーション
	Animation();

	for (auto& data : block_data)
	{
		//時間を過ぎたら消す
		if (--data.block_anim_timer <= 0)
		{
			delete_block_data.push_back(data);
		}
	}

	//オブジェクト配列から削除する処理
	for (const auto& delete_data : delete_block_data)
	{
		for (auto it = block_data.begin(); it != block_data.end();)
		{
			if (*it == delete_data)
			{
				it = block_data.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡時アニメーションに遷移
		image_line = 1;
		anim_span = 5;

		//死にながらコインをまき散らす
		if (++death_timer % 10 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand/3);
			drop_coin_count++;
			//煙エフェクト生成
			manager->CreateEffect(elSmoke, this->location + rand, true, 20);
		}

		//死亡演出時間を過ぎたら自身を削除
		if (anim_end_flg)
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

void Enemy5::Draw()const
{
	__super::Draw();
	//敵3
	DrawString(local_location.x, local_location.y, "enemy5", 0xffffff);

	for (const auto data : block_data)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawCircleAA(data.attack_loc.x - camera->GetCameraLocation().x, data.attack_loc.y - camera->GetCameraLocation().y, data.block_anim_timer * i % 30, 6, 0xff0000, false);
		}
	}
}

void Enemy5::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy5::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	//死に至るダメージを受けたならノックバックする
	if (hp - _value <= 0)
	{
		__super::Damage(_value, _attack_loc , _knock_back);
	}
	else
	{
		__super::Damage(_value, _attack_loc, 1);

	}

	//ブロックアニメーション開始
	BlockData data;
	data.block_anim_timer = 20;
	data.attack_loc = _attack_loc;
	block_data.push_back(data);
}

