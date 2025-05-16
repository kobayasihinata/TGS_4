#include "Enemy4.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"
#include "../../Scene/InGameScene.h"

Enemy4::Enemy4(InGameScene* _ingame)
{
	ingame = _ingame;

	move_speed = ENEMY4_SPEED;
	hp = ENEMY4_HP;
	hit_damage = ENEMY4_DAMAGE;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = ENEMY4_DROPCOIN + (GetRand(2) - 1);

	coin_num = 0;	//持っているコイン
	steal_flg = false;	//コインを盗んだか
}

Enemy4::~Enemy4()
{

}

void Enemy4::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Enemy4::Finalize()
{

}

void Enemy4::Update()
{
	/*********ActorBaseの内の処理*********/
	frame++;

	//停止前に移動してた方向を保存する
	if (fabsf(velocity.x) > MOVE_LOWER_LIMIT)
	{
		last_velocity.x = velocity.x;
	}
	if (fabsf(velocity.y) > MOVE_LOWER_LIMIT)
	{
		last_velocity.y = velocity.y;
	}
	/*************************************/

	//行動の処理
	Enemy4Move();

	//移動
	Move();

	//アニメーション
	Animation();

	//死亡演出フラグが立っているなら
	if (death_flg)
	{

		//死にながらコインをまき散らす
		if (death_timer % 15 == 0 && drop_coin_count < drop_coin)
		{
			manager->CreateObject(
				eCOIN,
				this->location,
				Vector2D{40, 40},
				20.f,
				Vector2D{ (float)(GetRand(30) - 15),(float)(GetRand(30) - 15) });
			drop_coin_count++;
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

void Enemy4::Draw()const
{
	__super::Draw();
	//敵4
	DrawString(local_location.x, local_location.y, "enemy4", 0xffffff);
}

void Enemy4::Hit(ObjectBase* hit_object)
{
	//プレイヤーに当たったらコインを奪い、コインを奪ったフラグを立てる
	if (!steal_flg && hit_object->GetObjectType() == ePLAYER)
	{
		//コイン量が盗む量より多ければ規定量盗む
		if (UserData::coin > ENEMY4_STEAL)
		{
			UserData::coin -= ENEMY4_STEAL;
			coin_num = ENEMY4_STEAL;
		}
		//コイン量が盗む量より少なければ有り金全て盗る
		else
		{
			coin_num = UserData::coin;
			UserData::coin = 0;
		}
		drop_coin += coin_num * 2;
		move_speed = ENEMY4_ESCAPE_SPEED;
		steal_flg = true;

		std::string s = "-" + std::to_string(coin_num);
		ingame->CreatePopUp(this->location, s, GetRand(100), 0xff0000, -1, 60);
	}
}

void Enemy4::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc);
}

void Enemy4::Enemy4Move()
{		
	//死亡していなければこの処理
	if (!this->death_flg)
	{
		//プレイヤーへの角度計算
		double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		//プレイヤーからコインを盗んでいなければ、プレイヤーに向かって進む
		if (!steal_flg)
		{
			//移動の上限値設定
			if (fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
			if (fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);
		}
		//プレイヤーからコインを盗んでいれば、プレイヤーとは反対に進む
		else
		{
			//移動の上限値設定
			if (fabsf(velocity.x) < move_speed)velocity.x -= move_speed * cos(radian);
			if (fabsf(velocity.y) < move_speed)velocity.y -= move_speed * sin(radian);
		}
		//移動したい方向保存
		move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
	}
}
