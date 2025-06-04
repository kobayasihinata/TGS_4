#include "Enemy3.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy3::Enemy3()
{
	
	move_speed = ENEMY3_SPEED;
	max_hp = hp = ENEMY3_HP;
	hit_damage = ENEMY3_DAMAGE;
	shot_span = ENEMY3_ATTACK_SPAN;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = ENEMY3_DROPCOIN + (GetRand(2) - 1);
	//レア個体ならコインドロップは2倍、HPは1.5倍、弾発射速度5倍
	if (rare_flg)
	{
		max_hp = hp *= 1.5f;
		drop_coin *= 2;
		shot_span /= 5;
	}
	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy3/Enemy3_Idle.png", 18, 5, 4, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy3/Enemy3_Throw.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy3/Enemy3_Death.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);

	image = animation_image[0][0];

	shot_once = false;
	
	//SE読み込み
	death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
}

Enemy3::~Enemy3()
{

}

void Enemy3::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 2;
}

void Enemy3::Finalize()
{

}

void Enemy3::Update()
{
	__super::Update();

	//移動
	Move();

	//生きてるなら、顔の方向だけ変える
	if (!death_flg)
	{
		if (camera->player_location.x > this->location.x)
		{
			move_velocity.x = 1.f;
		}
		else
		{
			move_velocity.x = -1.f;
		}
	}

	//アニメーション
	Animation();

	//投擲アニメーションが一周したら通常アニメーションに戻す
	if (image_line == 1 && anim_end_flg)
	{
		image_line = 0;
	}

	//投擲アニメーションが指定の画像まで再生されたら、弾を発射する
	if (!shot_once && image_line == 1 && image_num == 4)
	{
		//プレイヤーの位置で発射角度を決める
		shot_rad = atan2f(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		manager->CreateAttack(GetBulletData());
		//一回だけ撃つ
		shot_once = true;
	}
	//指定の周期毎にアニメーションを投擲に変える
	if (!death_flg && frame % shot_span == 0)
	{
		//投擲アニメーション開始
		image_line = 1;
		anim_timer = 0;
		image_num = 0;
		//一回だけ撃つ用変数リセット
		shot_once = false;
	}

	//死亡演出フラグが立っているなら
	if (death_flg)
	{

		//死亡アニメーションを表示
		image_line = 2;
		anim_span = 3;

		//死にながらコインをまき散らす
		if (++death_timer % 20 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
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
			//エフェクト
			manager->CreateEffect(elExplosion, this->location);
			//SE再生
			PlaySoundMem(death_se, DX_PLAYTYPE_BACK);

			//レア個体ならアイテム生成
			if (rare_flg)
			{
				//回復か磁石
				if (GetRand(1) == 0)
				{
					manager->CreateObject(
						eHEAL,
						this->location,
						Vector2D{40, 40});
				}
				else
				{
					manager->CreateObject(
						eMAGNET,
						this->location,
						Vector2D{40, 40});
				}
			}
		}
	}
}

void Enemy3::Draw()const
{
	__super::Draw();
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
	_data.b_type = BulletType::bNormal;
	return _data;
}
