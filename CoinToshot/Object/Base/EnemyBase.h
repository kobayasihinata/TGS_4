#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

#define KNOCK_BACK 1.5f		//�m�b�N�o�b�N���̔{��

class EnemyBase :
	public ActorBase
{
private:

	int move_speed;	//�ړ����x


public:

	virtual void Update()override
	{
		__super::Update();

		MovetoPlayer();
	}
	//�v���C���[�Ɍ������Ĉړ�����
	void MovetoPlayer()
	{
			
	}

};