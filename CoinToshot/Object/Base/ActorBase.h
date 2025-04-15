#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"

#define D_GRAVITY (9.807f)		//�d�͉����x(m/ss)
//#define D_GRAVITY (4.807f)		//�d�͉����x(m/ss)
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
	float g_velocity = 0.0f;						//�d�͑��x	
	bool on_floor;							//�n�ʂɐG��Ă��邩���f

public:
	virtual void Update()override
	{
		frame++;
		//�d�͑��x�̌v�Z(�n�ʂɐG��Ă���Ȃ�)
		if (!on_floor)
		{
			g_velocity += D_GRAVITY / 222.0f;
			velocity.y += g_velocity;
		}
		//Y���̉����x�����ʂ����������A�n�ʂɐG��Ă��Ȃ��Ɣ��f����
		if (fabsf(velocity.y) > 0.5f)
		{
			on_floor = false;
		}
	}

	//�����蔻�肪��������̏���
	virtual void Hit(ObjectBase* hit_object)override
	{
		//�U���ƃA�C�e���ȊO�Ȃ�
		if (hit_object->GetObjectType() != ObjectList::ATTACK &&
			hit_object->GetObjectType() != ObjectList::GOALFLAG &&
			hit_object->GetObjectType() != ObjectList::COIN &&
			hit_object->GetObjectType() != ObjectList::ENERGYDRINK)
		{
			//�����O�̍��W��ۑ�
			float old = location.y;
			//�I�u�W�F�N�g������
			BoxCollider::Push(this, hit_object);
			//���ɐG��Ă����ꍇ�t���O�𗧂Ă�
			if (old >= location.y)on_floor = true;
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
	}

	//�P�t���[���O�̍��W���擾����
	Vector2D GetOldLocation()const { return old_location; }
};
