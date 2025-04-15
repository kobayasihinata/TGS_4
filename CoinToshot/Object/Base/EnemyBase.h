#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

#define KNOCK_BACK 1.5f		//ノックバック時の倍率

class EnemyBase :
	public ActorBase
{
private:

	int move_speed;	//移動速度


public:

	virtual void Update()override
	{
		__super::Update();

		MovetoPlayer();
	}
	//プレイヤーに向かって移動する
	void MovetoPlayer()
	{
			
	}

};