#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

class ActorBase :
	public ObjectBase
{
protected:
	int frame;		//�t���[������
	float max_hp;	//�ő�HP�L��
	float hp;		//HP�i�[
	float hpbar_move;	//HP�Q�[�W�����A�j���[�V����
	Vector2D old_location = { 0.0f,0.0f };	//1�t���[���O�̍��W

	Vector2D last_velocity;				//��~����i�l��MOVE_LOWER_LIMIT�������j���O��velocity
	Vector2D move_velocity;				//�ړ������������̕ۑ�(���ۂ̈ړ��ʂƂ͈قȂ�)

	int drop_coin;			//�h���b�v����R�C���̗�
	int drop_coin_count;	//�h���b�v����R�C���̌v��

	int damage_se;			//�_���[�W��
public:
	Vector2D velocity = { 0.0f,0.0f };

public:
	virtual void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f, float init_radius = 0.f)override
	{
		__super::Initialize(_manager, _object_type,init_location, init_size, init_radius);

		//������
		frame = 0;
		hpbar_move = 0.f;
		old_location = 0.f;
		last_velocity = 0.f;				
		move_velocity = 0.f;
		drop_coin_count = 0;

		//SE�Ǎ�
		ResourceManager* rm = ResourceManager::GetInstance();
		//SE�ǂݍ���
		damage_se = rm->GetSounds("Resource/Sounds/damage.mp3");
	}

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

	}

	//�����蔻�肪��������̏���
	virtual void Hit(ObjectBase* hit_object)override
	{
		//���g�������Ă銎�v���C���[�ł͂Ȃ��A���肪�����Ă��āA�U���ƃA�C�e���ȊO�Ȃ�
		if (!(this->death_flg) && 
			this->object_type != ePLAYER &&
			!hit_object->GetDeathFlg() &&
			hit_object->GetObjectType() != ObjectList::eATTACK &&
			hit_object->GetObjectType() != ObjectList::eCOIN &&
			hit_object->GetObjectType() != ObjectList::eHEAL &&
			hit_object->GetObjectType() != ObjectList::eMAGNET)
		{
			//������Ȃ��Ȃ�܂ŌJ��Ԃ�
			while (this->CheckHit(hit_object))
			{
				//�I�u�W�F�N�g������
				Push(hit_object);
			}
		}
	}

	virtual void Damage(float _value, Vector2D _attack_loc,int _knock_back = KNOCK_BACK)override
	{
		//���g��HP��1�ȏ�Ȃ炱�̏���
		if (hp > 0)
		{

			hp -= _value;
			//HP��0��菬�����Ȃ�����0�ɂ���
			if (hp < 0)hp = 0;

			//�_���[�W�ʂ����g�̍ő�HP���傫����΁A�_���[�W�͍ő�HP�Ƃ���
			if (_value > max_hp)
			{
				_value = max_hp;
			}
			//�����A�j���[�V����
			hpbar_move += _value * (HPBAR_SIZE / max_hp);

			//�_���[�WSE
			PlaySoundMem(damage_se, DX_PLAYTYPE_BACK);

			//�m�b�N�o�b�N����

			//�_���[�W���̒��S���W����m�b�N�o�b�N���������߂�
			double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
			velocity.x -= (_knock_back * cos(radian));
			velocity.y -= (_knock_back * sin(radian));

			//��
			if (hp <= 0)
			{
				this->death_flg = true;
				//�A�j���[�V�����ʒu�����Z�b�g
				image_num = 0;
				anim_end_flg = false;
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

