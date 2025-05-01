#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;		//カメラポインタ格納(プレイヤーの情報を渡すためだけに取得)
	float move_speed;	//移動速度
	int hit_damage;	//プレイヤーに与えるダメージ量
	int drop_coin;		//ドロップするコインの量

public:

	EnemyBase()
	{
		//カメラ取得
		camera = Camera::Get();
	}

	virtual void Update()override
	{
		__super::Update();

		MovetoPlayer();

	}

	virtual void Draw()const override
	{
		__super::Draw();

		//hp描画
		DrawFormatStringF(local_location.x, local_location.y - (box_size.y / 2), 0xffffff, "HP:%f", this->hp);
	}

	virtual void Hit(ObjectBase* hit_object)override
	{
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
		}
	}

};