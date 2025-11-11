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
	drop_coin = BOSS3_DROPCOIN;

	boss_stop_flg = false;
	strength_level = 0;
	boss_speed[0] = BOSS3_SPEED;
	boss_speed[1] = BOSS3_SPEED * 2;
	move_mode = 0;
	can_shot = false;
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

	drop_coin = BOSS3_DROPCOIN + UserData::boss_coin;	//コインドロップ量に、所持コインを加算する

	//一回だけアニメーション実行
	if (!anim_once)
	{
		ingame->SetBossSpawnAnim(this, this->location, BOSS_ANIM);
		anim_once = true;
	}

	//移動方向決定処理
	MoveBoss3();

	//弾発射関連処理
	Bullet();


	//死亡していなければこの処理
	if (!death_flg && !anim_flg && !boss_stop_flg)
	{
		//移動
		Move();
	}

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
				Vector2D rand = { (float)(GetRand(50) - 25),(float)(GetRand(50) - 25) };
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
			manager->CreateEffect(elExplosion, this->location, { 0,0 });

			//終了時処理
			this->Finalize();
		}
	}

}

void Boss3::Draw()const
{
	__super::Draw();

	SetFontSize(17);
	DrawBoxAA(local_location.x - (box_size.x / 2)-2,
		local_location.y - box_size.y - 20,
		local_location.x + (box_size.x / 2)+12,
		local_location.y - box_size.y, 0xffffff, true);

	UserData::DrawCoin({ local_location.x-12,
	local_location.y - box_size.y - 10 },
		10);

	DrawFormatString(local_location.x,
		local_location.y - box_size.y - 20,
		UserData::boss_coin >= 0 ? 0x550000 : 0x0000ff,
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

	//投擲アニメーションが一周したら通常アニメーションに戻す
	if (image_line == 1 && anim_end_flg)
	{
		image_line = 0;
	}
	//投擲アニメーションが指定の画像まで再生されたら、弾を発射する
	if (!shot_once && image_line == 1 && image_num == 4)
	{
		//プレイヤーの位置で発射角度を決める
		shot_rad = atan2f(target_loc.y - this->location.y, target_loc.x - this->location.x);
		ShotBullet();
		UserData::boss_coin -= bBullet[bullet_type].cost;
		std::string s = "-" + std::to_string(bBullet[bullet_type].cost);
		ingame->CreatePopUp(this->location, s, 0x550000, -1);
		//一回だけ撃つ
		shot_once = true;
	}
	//指定の周期毎にアニメーションを投擲に変える
	if (!death_flg && 
		can_shot && 
		frame % boss3_bullet[bullet_type].attack_span == 0)
	{
		//投擲アニメーション開始
		image_line = 1;
		anim_timer = 0;
		image_num = 0;
		shot_count++;
		//一回だけ撃つ用変数リセット
		shot_once = false;
	}
}

BulletData Boss3::GetBulletData(float _shot_rad)
{
	BulletData _data;
	_data.damage = bBullet[bullet_type].damage;
	_data.b_angle = _shot_rad;
	_data.delete_time = bBullet[bullet_type].life_span;
	_data.h_count = bBullet[bullet_type].h_count;
	_data.location = this->location;
	_data.radius = bBullet[bullet_type].radius;
	_data.speed = bBullet[bullet_type].speed;
	_data.who = this;
	_data.b_type = (BulletType)bullet_type;

	for (int i = 0; i < 3; i++)
	{
		_data.color[i] = bBullet[bullet_type].color[i];
	}
	return _data;
}

void Boss3::ChangeMove()
{
	//コインの所持数に応じて移動方法変更

	//コイン収集中で、所持コインが指定量を超えたら攻撃開始
	if (move_mode == 0 && UserData::boss_coin > boss3_bullet[bullet_type].correct_coin)
	{
		move_mode = bullet_type + 1;
	}
	//攻撃中にコインがなくなるor指定の回数攻撃したら
	else if (move_mode != 0 && (UserData::boss_coin < bBullet[bullet_type].cost || shot_count >= boss3_bullet[bullet_type].shot_num))
	{
		//コイン収集モード
		move_mode = 0;
		//弾種類ランダム変更
		if (strength_level == 0)
		{
			bullet_type = GetRand(2);
		}
		else
		{
			bullet_type = GetRand(5);
		}
		//リセット
		shot_count = 0;
		//弾を撃てなくする
		can_shot = false;
	}
}

void Boss3::MoveBoss3()
{
	double radian;

	can_shot = true;	//デフォルトで弾が撃てる状態にする
	target_loc = camera->player_location;				//攻撃のターゲットをデフォルトでプレイヤーにする

	//挙動変更処理
	ChangeMove();

	boss_stop_flg = false;

	switch (move_mode)
	{
	case 0:	//コイン集め
		if (manager->CheckNearCoin(this) != nullptr)
		{
			radian = atan2(manager->CheckNearCoin(this)->GetLocation().y - this->location.y, manager->CheckNearCoin(this)->GetLocation().x - this->location.x);
			can_shot = false;
		}
		else
		{
			//コイン吸い寄せのせいでコインがなくなっていたら、プレイヤーに突撃
			if (UserData::attraction_flg || manager->CheckNearEnemy(this, this) == nullptr)
			{
				radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
			}
			//ただコインが無いだけなら、近くの敵を攻撃してコインを生む
			else
			{
				target_loc = manager->CheckNearEnemy(this, this)->GetLocation();
				radian = MoveAround(target_loc, 20);
				bullet_type = 0;
			}
		}
		break;
	case 1:	//プレイヤー攻撃(通常弾)
		radian = MoveAround(camera->player_location, BOSS3_PLAYER_DISTANCE);
		//左右に揺れる
		if (frame % 60 > 30)radian += 3.4;

		break;
	case 2:	//プレイヤー攻撃(散弾)
		//プレイヤーと離れていたら、プレイヤーに向けて移動
		if (UserData::Distance(camera->player_location, this->location) > BOSS3_PLAYER_DISTANCE/2 + 30)
		{
			radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);

		}
		//プレイヤーと近ければ、プレイヤーとは反対側に移動
		else if (UserData::Distance(camera->player_location, this->location) < BOSS3_PLAYER_DISTANCE/2 - 30)
		{
			radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x) * -1;
		}
		break;
	case 3:	//プレイヤー攻撃(強化弾)
		radian = MoveAround(camera->player_location, BOSS3_PLAYER_DISTANCE, true);
		break;
	case 4:	//プレイヤー攻撃(追尾弾)
		boss_stop_flg = true;
		break;
	case 5:	//プレイヤー攻撃(爆発弾)
		radian = MoveAround(camera->player_location, BOSS2_PLAYER_DISTANCE/2);
		break;
	case 6:	//プレイヤー攻撃(最強弾)
		boss_stop_flg = true;
		break;
	default:
		break;
	}

	//プレイヤーが遠くに居たら加速
	if (UserData::Distance(camera->player_location, this->location) > BOSS3_PLAYER_DISTANCE2)
	{
		move_speed = boss_speed[strength_level] * 5;
	}
	else
	{
		move_speed = boss_speed[strength_level];
	}

	//移動の上限値設定
	if (fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
	if (fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);

	//移動したい方向保存
	move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
}

void Boss3::ShotBullet()
{
	//一発以上撃つ弾種なら
	if (bBullet[bullet_type].bullet_num > 1)
	{
		float t = (bBullet[bullet_type].space * bBullet[bullet_type].bullet_num) / 2;
		for (int i = 0; i < bBullet[bullet_type].bullet_num; i++)
		{
			manager->CreateAttack(GetBulletData(shot_rad - t + i * bBullet[bullet_type].space));
		}
	}
	//単発なら
	else
	{
		manager->CreateAttack(GetBulletData(shot_rad));
	}
}

double Boss3::MoveAround(Vector2D _loc, int _distance, bool _direction)
{
	double radian;
	//プレイヤーと離れていたら、プレイヤーに向けて移動
	if (UserData::Distance(_loc, this->location) > _distance)
	{
		if (_direction)
		{
			radian = atan2(_loc.y - this->location.y, _loc.x - this->location.x) - 0.8;

		}
		else
		{
			radian = atan2(_loc.y - this->location.y, _loc.x - this->location.x) + 0.8;

		}
	}
	//プレイヤーと近ければ、プレイヤーとは反対側に移動
	else if (UserData::Distance(_loc, this->location) < _distance)
	{
		if (_direction)
		{
			radian = atan2(_loc.y - this->location.y, _loc.x - this->location.x) - 2.5;
		}
		else
		{
			radian = atan2(_loc.y - this->location.y, _loc.x - this->location.x) + 2.5;
		}

	}
	//横移動
	else
	{
		if (_direction)
		{
			radian = atan2(_loc.y - this->location.y, _loc.x - this->location.x) - 1.7;
		}
		else
		{
			radian = atan2(_loc.y - this->location.y, _loc.x - this->location.x) + 1.7;
		}
	}
	return radian;
}

void Boss3::SetStrength(int _level)
{
	//既にその強さレベルならスキップ
	if (strength_level != _level)
	{
		strength_level = _level;
		switch (strength_level)
		{
		case 0:
			hit_damage = BOSS3_DAMAGE;
			break;
		case 1:
			hit_damage = BOSS3_DAMAGE + 1;
			break;
		}
	}
}