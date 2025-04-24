#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

#define KNOCK_BACK 1.5f		//�m�b�N�o�b�N���̔{��

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;		//�J�����|�C���^�i�[(�v���C���[�̏���n�����߂����Ɏ擾)
	int move_speed;	//�ړ����x
	int hit_damage;	//�v���C���[�ɗ^����_���[�W��

public:

	EnemyBase()
	{
		//�J�����擾
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

		//hp�`��
		DrawFormatStringF(local_location.x, local_location.y - (box_size.y / 2), 0xffffff, "HP:%f", this->hp);
	}

	virtual void Hit(ObjectBase* hit_object)override
	{
		//�v���C���[�ɓ���������_���[�W��^����
		if (hit_object->GetObjectType() == ePLAYER)
		{
			hit_object->Damage(hit_damage, this->location);
		}

	}
	//�v���C���[�Ɍ������Ĉړ�����
	void MovetoPlayer()
	{
		double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		velocity.x = move_speed * cos(radian);
		velocity.y = move_speed * sin(radian);
	}

};