#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

class ActorBase :
	public ObjectBase
{
protected:
	int frame;	//�t���[������
	float hp;
	Vector2D old_location = { 0.0f,0.0f };	//1�t���[���O�̍��W

	Vector2D last_velocity;				//��~����i�l��MOVE_LOWER_LIMIT�������j���O��velocity
	Vector2D move_velocity;				//�ړ������������̕ۑ�(���ۂ̈ړ��ʂƂ͈قȂ�)

	int drop_coin;			//�h���b�v����R�C���̗�
	int drop_coin_count;	//�h���b�v����R�C���̌v��
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

		//���S�������ɂȂ��Ă����i�����o�j
		if (death_flg)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, death_timer * 4);
		}

		//�摜�`��
		if (image != 0)
		{
			if (move_velocity.x > 0)
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, true, false);
			}
			else
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, true, true);
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

		//���S�������������i�����o�j
		if (death_flg)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}

	//�����蔻�肪��������̏���
	virtual void Hit(ObjectBase* hit_object)override
	{
		//���g�������Ă銎�v���C���[�ł͂Ȃ��A�U���ƃA�C�e���ȊO�Ȃ�
		if (!(this->death_flg) && 
			this->object_type != ePLAYER &&
			hit_object->GetObjectType() != ObjectList::eATTACK &&
			hit_object->GetObjectType() != ObjectList::eCOIN &&
			hit_object->GetObjectType() != ObjectList::eHEAL)
		{
			//�I�u�W�F�N�g������
			Push(hit_object);
		}
	}

	virtual void Damage(float _value, Vector2D _attack_loc)override
	{
		//���g��HP��1�ȏ�Ȃ炱�̏���
		if (hp > 0)
		{

			hp -= _value;
			//�m�b�N�o�b�N����

			//�_���[�W���̒��S���W����m�b�N�o�b�N���������߂�
			double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
			velocity.x -= (KNOCK_BACK * cos(radian));
			velocity.y -= (KNOCK_BACK * sin(radian));

			//��
			if (hp <= 0)
			{
				this->death_flg = true;
				//�A�j���[�V�����ʒu�����Z�b�g
				image_num = 0;
			}
		}
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

