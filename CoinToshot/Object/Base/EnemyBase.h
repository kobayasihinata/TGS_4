#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;	//カメラポインタ格納(プレイヤーの情報を渡すためだけに取得)
	float move_speed;		//移動速度
	int hit_damage;			//プレイヤーに与えるダメージ量

public:

	EnemyBase()
	{
		//カメラ取得
		camera = Camera::Get();
		move_speed = 0.f;		
		hit_damage = 0;			
		drop_coin = 0;	
		drop_coin_count = 0;
	}

	virtual void Update()override
	{
		__super::Update();

		MovetoPlayer();

		//HPの減った部分のアニメーション用
		if (hpbar_move > 1)
		{
			hpbar_move -= hpbar_move / 10;
		}
		else
		{
			hpbar_move = 0;
		}
	}

	virtual void Draw()const override
	{
		__super::Draw();

		//HPゲージ外枠
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0x000000,
			true
		);
		//HPゲージ内側
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0xffffff,
			false
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