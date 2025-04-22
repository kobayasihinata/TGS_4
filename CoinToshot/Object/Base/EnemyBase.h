#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

#define KNOCK_BACK 1.5f		//ノックバック時の倍率

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;		//カメラポインタ格納(プレイヤーの情報を渡すためだけに取得)
	int move_speed;	//移動速度


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
	//プレイヤーに向かって移動する
	void MovetoPlayer()
	{
		double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		velocity.x = move_speed * cos(radian);
		velocity.y = move_speed * sin(radian);
	}

};