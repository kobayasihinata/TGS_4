#include "Boss3.h"
#include "../../ObjectManager.h"
#include "../../../Scene/InGameScene.h"
#include "../../../Utility/common.h"

InGameScene* Boss3::ingame;
bool Boss3::boss3_once;

Boss3* Boss3::Get(InGameScene* _ingame)
{
	if (!boss3_once)
	{
		boss3_once = true;
		ingame = _ingame;
		static Boss3 instance;
		//自分自身のポインタを返却する
		return &instance;
	}
	else
	{
		return nullptr;
	}
}

void Boss3::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 1;
	
	move_speed = BOSS3_SPEED;
	max_hp = hp = BOSS3_HP;
	hit_damage = BOSS3_DAMAGE;
	shot_span = BOSS3_ATTACK_SPAN;
	drop_coin = BOSS3_DROPCOIN;

	move_mode = 0;
	bullet_type = 1;
	shot_speed = 0;

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
}

void Boss3::Finalize()
{
	//リセット
	boss3_once = false;
	UserData::boss_coin = 0;
}

void Boss3::Update()
{
	__super::Update();

	drop_coin = BOSS3_DROPCOIN + UserData::boss_coin;

	//一回だけアニメーション実行
	if (!anim_once)
	{
		ingame->SetBossSpawnAnim(this, this->location, BOSS_ANIM);
		anim_once = true;
	}

	MoveBoss3();

	//登場アニメーション中は動かない
	if (!anim_flg)
	{
		//移動
		Move();
	}

	//弾発射関連処理
	Bullet();

	//アニメーション
	Animation();

	//HPゲージ表示
	ingame->SetBossHp("Boss3", hp, max_hp, hpbar_move);

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡アニメーションを表示
		image_line = 2;
		anim_span = 5;

		//死にながらコインをまき散らす
		if (++death_timer % 5 == 0 && drop_coin_count < drop_coin)
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
				Vector2D rand = { (float)(GetRand(25) - 12),(float)(GetRand(25) - 12) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{ 40, 40 },
					20.f,
					rand);
			}
			for (int i = 0; i < 3; i++)
			{
				Vector2D rand = { (float)(GetRand(25) - 12),(float)(GetRand(25) - 12) };
				//回復か磁石
				if (GetRand(50 - UserData::player_hp) > 10)
				{
					manager->CreateObject(
						eHEAL,
						this->location + rand,
						Vector2D{ 40, 40 },
						20.f,
						rand);
				}
				else
				{
					manager->CreateObject(
						eMAGNET,
						this->location + rand,
						Vector2D{ 40, 40 },
						20.f,
						rand);
				}
			}
			//SE再生
			ResourceManager::rPlaySound(item_spawn_se, DX_PLAYTYPE_BACK);
			ResourceManager::rPlaySound(death_se, DX_PLAYTYPE_BACK);

			//エフェクト
			manager->CreateEffect(elExplosion, this->location);
		}
	}

}

void Boss3::Draw()const
{
	__super::Draw();

	//SetFontSize(24);
	UserData::DrawCoin({ local_location.x - GetDrawFormatStringWidth("×%d", UserData::boss_coin),
	local_location.y + 45 },
		15);
	DrawFormatString(local_location.x - GetDrawFormatStringWidth("×%d", UserData::boss_coin),
		local_location.y + 22,
		0x550000,
		"×%d",
		UserData::boss_coin);
}

void Boss3::Hit(ObjectBase* hit_object)
{
	__super::Hit(hit_object);

}

void Boss3::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc, _knock_back);
	//減少アニメーション
	hpbar_move += _value * (BOSS_BAR_SIZE / max_hp);

}

void Boss3::Bullet()
{
	shot_span = BOSS3_ATTACK_SPAN;

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
		ShotBullet();
		UserData::boss_coin -= pBullet[bullet_type].cost;
		std::string s = "-" + std::to_string(pBullet[bullet_type].cost);
		ingame->CreatePopUp(this->location, s, 0x550000, -1);
		//一回だけ撃つ
		shot_once = true;
	}
	//指定の周期毎にアニメーションを投擲に変える
	if (!death_flg && frame % shot_span == 0 && move_mode == 1)
	{
		//投擲アニメーション開始
		image_line = 1;
		anim_timer = 0;
		image_num = 0;
		//一回だけ撃つ用変数リセット
		shot_once = false;
	}
}

BulletData Boss3::GetBulletData(float _shot_rad)
{
	BulletData _data;
	_data.damage = pBullet[bullet_type].damage;
	_data.b_angle = _shot_rad;
	_data.delete_time = pBullet[bullet_type].life_span;
	_data.h_count = pBullet[bullet_type].h_count;
	_data.location = this->location;
	_data.radius = pBullet[bullet_type].radius;
	_data.speed = pBullet[bullet_type].speed;
	_data.who = this;
	_data.b_type = (BulletType)bullet_type;
	for (int i = 0; i < 3; i++)
	{
		//０を下回らないようにする
		if (pBullet[bullet_type].color[i] - 120 < 0)
		{
			_data.color[i] = 0;
		}
		else
		{
			_data.color[i] = pBullet[bullet_type].color[i] - 150;
		}
	}
	return _data;
}

void Boss3::MoveBoss3()
{
	//死亡していなければこの処理
	if (!this->death_flg)
	{
		double radian;

		//コインの所持数に応じて移動方法変更
		if (UserData::boss_coin > 20)
		{
			move_mode = 1;
		}
		//攻撃中にコインがなくなったら
		else if (move_mode == 1 && UserData::boss_coin < pBullet[bullet_type].cost)
		{
			move_mode = 0;
		}
		switch (move_mode)
		{
		case 0:	//コイン集め
			if (manager->CheckNearCoin(this) != nullptr)
			{
				radian = atan2(manager->CheckNearCoin(this)->GetLocation().y - this->location.y, manager->CheckNearCoin(this)->GetLocation().x - this->location.x);
			}
			else
			{
				radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
			}
			break;
		case 1:	//プレイヤー攻撃
			//プレイヤーと離れていたら、プレイヤーに向けて移動
			if (UserData::Distance(camera->player_location, this->location) > BOSS3_PLAYER_DISTANCE + 30)
			{
				radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x) + 0.8;

			}
			//プレイヤーと近ければ、プレイヤーとは反対側に移動
			else if (UserData::Distance(camera->player_location, this->location) < BOSS3_PLAYER_DISTANCE - 30)
			{
				radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x) + 2.5;

			}
			//横移動
			else
			{
				radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x) + 1.7;
			}

			break;
		default:
			break;
		}

		//移動の上限値設定
		if (fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
		if (fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);

		//移動したい方向保存
		move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
	}
}

void Boss3::ShotBullet()
{
	//一発以上撃つ弾種なら
	if (pBullet[bullet_type].bullet_num > 1)
	{
		float t = (pBullet[bullet_type].space * pBullet[bullet_type].bullet_num) / 2;
		for (int i = 0; i < pBullet[bullet_type].bullet_num; i++)
		{
			manager->CreateAttack(GetBulletData(shot_rad - t + i * pBullet[bullet_type].space));
		}
	}
	//単発なら
	else
	{
		manager->CreateAttack(GetBulletData(shot_rad));
	}
}
