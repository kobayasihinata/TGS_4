#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

#define KNOCK_BACK 10		//�m�b�N�o�b�N���̔{��

class ActorBase :
	public ObjectBase
{
protected:
	int frame;	//�t���[������
	float hp;
	Vector2D old_location = { 0.0f,0.0f };	//1�t���[���O�̍��W

	Vector2D last_velocity;				//��~����i�l��MOVE_LOWER_LIMIT�������j���O��velocity

public:
	Vector2D velocity = { 0.0f,0.0f };

public:
	virtual void Update()override
	{
		frame++;
		//��~�O�Ɉړ����Ă�������ۑ�����
		if (fabsf(velocity.x) > MOVE_LOWER_LIMIT)
		{
			last_velocity.x = velocity.x;
		}
		if (fabsf(velocity.y) > MOVE_LOWER_LIMIT)
		{
			last_velocity.y = velocity.y;
		}
	}

	virtual void Draw()const override
	{
		//�摜�`��
		if (image != 0)
		{
			if (last_velocity.x > 0)
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, false, false);
			}
			else
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, false, true);
			}
		}
#ifdef _DEBUG
		//�����蔻��̕`��

		//�l�p
		if (this->radius <= 0.f)
		{
			Vector2D::DrawBoxV2(local_location - (box_size / 2), local_location + (box_size / 2), 0xff0000, false);
		}
		//�~
		else
		{
			DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xff0000, false);
		}
#endif // _DEBUG
	}

	//�����蔻�肪��������̏���
	virtual void Hit(ObjectBase* hit_object)override
	{
		//���g���v���C���[�ł͂Ȃ��A�U���ƃA�C�e���ȊO�Ȃ�
		if (this->object_type != ePLAYER &&
			hit_object->GetObjectType() != ObjectList::eATTACK &&
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

		//�_���[�W���̒��S���W����m�b�N�o�b�N���������߂�
		double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
		location.x -= (KNOCK_BACK * cos(radian));
		location.y -= (KNOCK_BACK * sin(radian));
	}

	//�ړ�����
	void Move()
	{

		DebugInfomation::Add("velox", last_velocity.x);
		DebugInfomation::Add("veloy", last_velocity.y);
		//�����̎��s
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//�ړ��O�̍��W�i�[
		old_location = location;
		//�ړ��̎��s
		location += velocity;
		//�ǂɓ������Ă�����O�̍��W�ɖ߂�

		//���[
		if (location.x - (box_size.x/2) <= -STAGE_SIZE)
		{
			location.x = -STAGE_SIZE + (box_size.x / 2);
		}
		//�E�[
		if (location.x + (box_size.x / 2) > STAGE_SIZE)
		{
			location.x = STAGE_SIZE - (box_size.x / 2);
		}
		//��[
		if (location.y - (box_size.y / 2) <= -STAGE_SIZE)
		{
			location.y = -STAGE_SIZE + (box_size.y / 2);
		}
		//���[
		if (location.y + (box_size.y / 2) > STAGE_SIZE)
		{
			location.y = STAGE_SIZE - (box_size.y / 2);
		}
	}

	//�P�t���[���O�̍��W���擾����
	Vector2D GetOldLocation()const { return old_location; }
};

