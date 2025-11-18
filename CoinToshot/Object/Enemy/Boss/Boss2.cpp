#include "Boss2.h"
#include "../../ObjectManager.h"
#include "../../../Scene/InGameScene.h"
#include "../../../Utility/common.h"

Boss2::Boss2(InGameScene* _ingame)
{
	ingame = _ingame;
	move_speed = BOSS2_SPEED;
	max_hp = hp = BOSS2_HP;
	hit_damage = BOSS2_DAMAGE;
	shot_span = BOSS2_ATTACK_SPAN;
	shot_speed = BOSS2_ATTACK_SPEED;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = BOSS2_DROPCOIN + (GetRand(2) - 1);

	boss_move_flg = false;

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Boss2/Boss2_Idle.png", 18, 5, 4, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss2/Boss2_Throw.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss2/Boss2_Run.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss2/Boss2_Run_Throw.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss2/Boss2_Death.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);

	image = animation_image[0][0];

	shot_once = false;
}

Boss2::~Boss2()
{

}

void Boss2::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 1;
}

void Boss2::Finalize()
{

}

void Boss2::Update()
{
	__super::Update();

	//壁に当たっていたら前の座標に戻す
//左端
	if (location.x - (box_size.x / 2) <= -STAGE_SIZE)
	{
		location.x = -STAGE_SIZE + (box_size.x / 2);
	}
	//右端
	if (location.x + (box_size.x / 2) > STAGE_SIZE)
	{
		location.x = STAGE_SIZE - (box_size.x / 2);
	}
	//上端
	if (location.y - (box_size.y / 2) <= -STAGE_SIZE)
	{
		location.y = -STAGE_SIZE + (box_size.y / 2);
	}
	//下端
	if (location.y + (box_size.y / 2) > STAGE_SIZE)
	{
		location.y = STAGE_SIZE - (box_size.y / 2);
	}

	//一回だけアニメーション実行
	if (!anim_once)
	{
		ingame->SetBossSpawnAnim(this, this->location, BOSS_ANIM);
		anim_once = true;
	}

	//HPが半分を切ったら、移動開始
	if (!boss_move_flg && hp < BOSS2_HP / 2)
	{
		boss_move_flg = true;
	}
	if (boss_move_flg)
	{
		MoveAround();
	}

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
	ingame->SetBossHp("Boss2", hp, max_hp, hpbar_move);

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡アニメーションを表示
		image_line = 4;
		anim_span = 5;

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
			//BGMを元に戻す処理
			ingame->SetFadeDefault();
		}
	}
}

void Boss2::Draw()const
{
	__super::Draw();

}

void Boss2::Hit(ObjectBase* hit_object)
{
	__super::Hit(hit_object);

}

void Boss2::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc, _knock_back);
	//減少アニメーション
	hpbar_move += _value * (BOSS_BAR_SIZE / max_hp);

}

void Boss2::Bullet()
{
	//プレイヤーとの距離を参照して、弾の速度と頻度を変える
	if (UserData::Distance(camera->player_location, this->location) > SCREEN_WIDTH/1.5)
	{
		shot_span = (int)(BOSS2_ATTACK_SPAN/5);
		shot_speed = (BOSS2_ATTACK_SPEED * 3);
	}
	else
	{
		shot_span = BOSS2_ATTACK_SPAN;
		shot_speed = BOSS2_ATTACK_SPEED;
	}

	//投擲アニメーションが一周したら通常アニメーションに戻す
	if ((image_line == 1 || image_line == 3) && anim_end_flg)
	{
		image_line = boss_move_flg ? 2 : 0;
	}
	//投擲アニメーションが指定の画像まで再生されたら、弾を発射する
	if (!shot_once && (image_line == 1 || image_line == 3) && image_num == 4)
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
		image_line = boss_move_flg ? 3 : 1;
		anim_timer = 0;
		image_num = 0;
		//一回だけ撃つ用変数リセット
		shot_once = false;
	}
}

BulletData Boss2::GetBulletData()
{
	BulletData _data;
	_data.b_angle = shot_rad;
	_data.damage = BOSS2_DAMAGE;
	_data.delete_time = BOSS2_ATTACK_LIMIT;
	_data.h_count = 1;
	_data.location = this->location;
	_data.radius = 15;
	_data.speed = shot_speed;
	_data.who = this;
	_data.b_type = BulletType::bNormal;
	_data.color[0] = 200;
	_data.color[1] = 0;
	_data.color[2] = 0;
	return _data;
}

void Boss2::MoveAround()
{
	//死亡していなければこの処理
	if (!this->death_flg)
	{
		//移動速度を、HPが少ないほど速くする
		move_speed = ((BOSS2_HP / 2) - hp) / 2 + 1;
		double radian;
		//プレイヤーと離れていたら、プレイヤーに向けて移動
		if (UserData::Distance(camera->player_location, this->location) > BOSS2_PLAYER_DISTANCE+30)
		{
			radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x) + 0.8;

		}
		//プレイヤーと近ければ、プレイヤーとは反対側に移動
		else if (UserData::Distance(camera->player_location, this->location) < BOSS2_PLAYER_DISTANCE-30)
		{
			radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x) + 2.5;

		}
		//横移動
		else
		{
			radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x)+1.7;
		}

		//移動の上限値設定
		if (fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
		if (fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);

		//移動したい方向保存
		move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
	}
}