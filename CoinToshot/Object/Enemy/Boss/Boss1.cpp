#include "Boss1.h"
#include "../../ObjectManager.h"
#include "../../../Scene/InGameScene.h"
#include "../../../Utility/common.h"

Boss1::Boss1(InGameScene* _ingame)
{
	ingame = _ingame;
	move_speed = BOSS1_SPEED;
	max_hp = hp = BOSS1_HP;
	hit_damage = BOSS1_DAMAGE;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = BOSS1_DROPCOIN + (GetRand(2) - 1);

	target_loc = 0;
	cooldown_timer = 0;
	rush_flg = false;
	rush_timer = 0;
	rush_radian = 0;
	rush_num = 0;
	rush_num_count = 0;

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Boss1/Boss1_Walk1.png", 24, 5, 5, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss1/Boss1_Idle1.png", 18, 5, 4, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss1/Boss1_Sliding1.png", 6, 5, 2, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Boss1/Boss1_Death1.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);

	image = animation_image[0][0];
}

Boss1::~Boss1()
{

}

void Boss1::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 1;
}

void Boss1::Finalize()
{

}

void Boss1::Update()
{
	__super::Update();

	//一回だけアニメーション実行
	if (!anim_once)
	{
		ingame->SetBossSpawnAnim(this,this->location, BOSS_ANIM);
		target_loc = this->location;
		this->location += 600;
		SetLocalLocation(camera->GetCameraLocation());
		rush_flg = true;
		rush_timer = 60;
		anim_once = true;
	}

	//登場アニメーション中は動かない
	if (!anim_flg)
	{
		//プレイヤーをターゲットとする
		target_loc = camera->player_location;

		//移動
		Move();
	}
	//登場アニメーション
	else
	{
		//突進中は移動可能
		if ((fabs(velocity.x) > BOSS1_SPEED * 2 || fabs(velocity.y) > BOSS1_SPEED * 2))
		{
			//移動
			Move();
		}
		else
		{
			image_line = 1;
		}
	}

	//アニメーション
	Animation();
	
	//HPゲージ表示
	ingame->SetBossHp("Boss1", hp, max_hp, hpbar_move);

	//突進処理
	Rush();

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡アニメーションを表示
		image_line = 3;
		anim_span = 5;

		//突進していたらやめる
		rush_flg = false;

		//死にながらコインをまき散らす
		if (++death_timer % 10 == 0 && drop_coin_count < drop_coin)
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
						Vector2D{40, 40},
						20.f,
						rand);
				}
				else
				{
					manager->CreateObject(
						eMAGNET,
						this->location + rand,
						Vector2D{40, 40},
						20.f,
						rand);
				}
			}

			//SE再生
			ResourceManager::rPlaySound(item_spawn_se, DX_PLAYTYPE_BACK);
			ResourceManager::rPlaySound(death_se, DX_PLAYTYPE_BACK);

			//エフェクト
			manager->CreateEffect(elExplosion, this->location,{0,0});
			//BGMを元に戻す処理
			ingame->SetCrossFadeDefault();
		}
	}
}

void Boss1::Draw()const
{
	__super::Draw();
	
	if (rush_flg)
	{
		DrawLineAA(local_location.x,
			local_location.y,
			local_location.x + RUSH_SPEED*9* cos(rush_radian),
			local_location.y + RUSH_SPEED*9* sin(rush_radian), 0xff0000);
	}
}

void Boss1::Hit(ObjectBase* hit_object)
{
	__super::Hit(hit_object);

	//自身が生きていて、タックル中で、敵に当たったらダメージを与える
	if ((fabs(velocity.x) > BOSS1_SPEED * 2 || fabs(velocity.y) > BOSS1_SPEED * 2) &&
		!this->death_flg &&
		hit_object->IsEnemy())
	{
		hit_object->Damage(hit_damage, this->location, 10 + fabs(velocity.x) + fabs(velocity.y));
	}
}

void Boss1::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{

	//ボスの速度に応じてノックバック量を変える
	__super::Damage(_value, _attack_loc, _knock_back);

	//減少アニメーション
	hpbar_move += _value * (BOSS_BAR_SIZE / max_hp);

}

void Boss1::Rush()
{
	//突進中でなければプレイヤーに向けて移動する
	if (!rush_flg)
	{
		MovetoPlayer();

		//一定速度より速いなら、アニメーションを切り替える
		if (abs(velocity.x) > BOSS1_SPEED * 2 || abs(velocity.y) > BOSS1_SPEED * 2)
		{
			//スライディング
			image_line = 2;
		}
		else
		{
			//歩き
			image_line = 0;
		}

	}

	//突進回数決定
	SetRushNum();

	//突進開始までの時間計測
	if (!rush_flg && ++cooldown_timer >= RUSH_SPAN)
	{
		rush_flg = true;
		cooldown_timer = 0;
	}
	if (rush_flg)
	{
		//一定速度より速いなら、アニメーションを切り替える
		if (abs(velocity.x) > BOSS1_SPEED * 2 || abs(velocity.y) > BOSS1_SPEED * 2)
		{
			//スライディング
			image_line = 2;
		}
		if (++rush_timer < 120)
		{
			//直前までプレイヤーに方向を定める
			if (rush_timer < 100)
			{
				rush_radian = atan2(target_loc.y - this->location.y, target_loc.x - this->location.x);
			}
			//突進チャージ中＆移動していなければ、停止アニメーション
			if (fabs(velocity.x) < 0.1f && fabs(velocity.y) < 0.1f)
			{
				image_line = 1;
			}
		}
		//一定時間経過で発射
		else
		{
			//移動の上限値設定
			velocity.x += RUSH_SPEED * cos(rush_radian);
			velocity.y += RUSH_SPEED * sin(rush_radian);

			//指定の回数突進が終わったらフラグを下げる
			if (++rush_num_count >= rush_num)
			{
				rush_flg = false;
				rush_num_count = 0;
			}
			rush_timer = 0;
		}
	}
}

void Boss1::SetRushNum()
{
	//HP残量に応じて突進回数を変える
	if (hp > (float)BOSS1_HP / 3 * 2)
	{
		rush_num = 1;
	}
	else if (hp > (float)BOSS1_HP / 3)
	{
		rush_num = 2;
	}
	else
	{
		rush_num = 3;
	}
}