#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"
#include <vector>

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;	//カメラポインタ格納(プレイヤーの情報を渡すためだけに取得)
	float move_speed;		//移動速度
	int hit_damage;			//プレイヤーに与えるダメージ量

	bool rare_flg;					//レア個体フラグ
	std::vector<int> shine_image;	//オーラ画像格納
	int now_shine_image;			//現在描画オーラ画像

	int death_se;				//死亡時SE
public:

	EnemyBase()
	{
		//カメラ取得
		camera = Camera::Get();
		move_speed = 0.f;		
		hit_damage = 0;			
		drop_coin = 0;	
		drop_coin_count = 0;

		//レア個体抽選
		rare_flg = (GetRand(30) == 0) ? true : false;

		if (rare_flg)
		{
			//画像読込
			ResourceManager* rm = ResourceManager::GetInstance();
			std::vector<int>tmp;
			shine_image = rm->GetImages("Resource/Images/Effect/yellow_shine.png", 40, 8, 5, 96, 96);
			now_shine_image = 0;
		}
	}

	virtual void Update()override
	{
		__super::Update();

		MovetoPlayer();

		//レア個体ならオーラアニメーションの更新
		if (rare_flg)
		{
			if (++now_shine_image > 39)
			{
				now_shine_image = 0;
			}
		}
	}

	virtual void Draw()const override
	{
		//レア個体なら、オーラの描画をする
		if (rare_flg)
		{
			DrawRotaGraphF(local_location.x, local_location.y, box_size.x/40, 0, shine_image[now_shine_image], TRUE);
		}

		__super::Draw();

		//HPゲージ内側
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0x000000,
			true
		);
		//HPゲージ本体
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)) + hpbar_move,
			local_location.y - (box_size.y / 2),
			0xffff00,
			true
		);
		//HPゲージ減少アニメーション
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)) + hpbar_move,
			local_location.y - (box_size.y / 2),
			0xff0000,
			true
		);
		//HPゲージ外枠
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0xffffff,
			false
		);

#ifdef _DEBUG
		//hp描画
		DrawFormatStringF(local_location.x, local_location.y - (box_size.y / 2), 0xffffff, "HP:%f", this->hp);
#endif // _DEBUG

	}

	virtual void Hit(ObjectBase* hit_object)override
	{
		__super::Hit(hit_object);

		//自身が生きていて、プレイヤーに当たったらダメージを与える
		if (!this->death_flg &&
			hit_object->GetObjectType() == ePLAYER)
		{
			hit_object->Damage(hit_damage, this->location);
		}

	}

	void Move()override
	{
		//減速の実行
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//移動前の座標格納
		old_location = location;
		//移動の実行
		location += velocity;
	}
	//プレイヤーに向かって移動する
	void MovetoPlayer()
	{
		//死亡していなければこの処理
		if (!this->death_flg)
		{
			double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
	
			//移動の上限値設定
			if(fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
			if(fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);

			//移動したい方向保存
			move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
		}
	}

};