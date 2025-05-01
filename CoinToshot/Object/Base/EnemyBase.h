#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;		//�J�����|�C���^�i�[(�v���C���[�̏���n�����߂����Ɏ擾)
	float move_speed;	//�ړ����x
	int hit_damage;	//�v���C���[�ɗ^����_���[�W��
	int drop_coin;		//�h���b�v����R�C���̗�

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
		//���g�������Ă��āA�v���C���[�ɓ���������_���[�W��^����
		if (!this->death_flg &&
			hit_object->GetObjectType() == ePLAYER)
		{
			hit_object->Damage(hit_damage, this->location);
		}

	}
	//�v���C���[�Ɍ������Ĉړ�����
	void MovetoPlayer()
	{
		//���S���Ă��Ȃ���΂��̏���
		if (!this->death_flg)
		{
			double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
	
			//�ړ��̏���l�ݒ�
			if(fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
			if(fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);
		}
	}

};