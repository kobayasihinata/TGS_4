#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

#define KNOCK_BACK 1.5f;		//�m�b�N�o�b�N���̔{��

class ActorBase :
	public ObjectBase
{
protected:
	int frame;	//�t���[������
	float hp;
	Vector2D old_location = { 0.0f,0.0f };	//1�t���[���O�̍��W
public:
	Vector2D velocity = { 0.0f,0.0f };

public:
	virtual void Update()override
	{
		frame++;
	}

	//�����蔻�肪��������̏���
	virtual void Hit(ObjectBase* hit_object)override
	{
		//�U���ƃA�C�e���ȊO�Ȃ�
		if (hit_object->GetObjectType() != ObjectList::eATTACK &&
			hit_object->GetObjectType() != ObjectList::eCOIN &&
			hit_object->GetObjectType() != ObjectList::eHEAL)
		{
			//�����O�̍��W��ۑ�
			float old = location.y;
			//�I�u�W�F�N�g������
			Push(hit_object);
		}
	}

	virtual void Damage(float _value, Vector2D _attack_loc)override
	{
		hp -= _value;
		//�m�b�N�o�b�N����

		//�v���C���[�̒��S���W
		Vector2D player_center = this->location + (this->box_size / 2);
		//�v���C���[�̒��S���W���U���̒��S���W���傫�����
		if (player_center.x > _attack_loc.x)
		{
			velocity.x = 5;
		}
		//�v���C���[�̒��S���W���U���̒��S���W��菬�������
		else
		{
			velocity.x = -5;
		}
		velocity.y = -5;
	}
	//�ړ�����
	void Move()
	{
		//�����̎��s
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//�ړ��O�̍��W�i�[
		old_location = location;
		//�ړ��̎��s
		location += velocity;
		//�ǂɓ������Ă�����O�̍��W�ɖ߂�

		//���[
		if (location.x <= -STAGE_SIZE)
		{
			location.x = -STAGE_SIZE;
		}
		//�E�[
		if (location.x + box_size.x > STAGE_SIZE)
		{
			location.x = STAGE_SIZE - box_size.x;
		}
		//��[
		if (location.y <= -STAGE_SIZE)
		{
			location.y = -STAGE_SIZE;
		}
		//���[
		if (location.y + box_size.y > STAGE_SIZE)
		{
			location.y = STAGE_SIZE - box_size.y;
		}
	}

	//�P�t���[���O�̍��W���擾����
	Vector2D GetOldLocation()const { return old_location; }
};

