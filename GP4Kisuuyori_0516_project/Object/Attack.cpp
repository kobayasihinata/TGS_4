#include "Attack.h"
#include "ObjectManager.h"
#include "../Utility/common.h"

Attack::Attack(BulletData _bullet_data)
{
	damage = _bullet_data.damage;
	object = _bullet_data.who;
	time = _bullet_data.delete_time;
	count_up = 0;
	move_velocity.x = _bullet_data.speed * cosf(_bullet_data.b_angle);
	move_velocity.y = _bullet_data.speed * sinf(_bullet_data.b_angle);
	hit_count = 0;
	hit_max = _bullet_data.h_count;
	old_hit_object = nullptr;

	//location.x += ((b_speed + acceleration * 0.08) * cosf(rad));
	//location.y += ((b_speed + acceleration * 0.08) * sinf(rad));
}

Attack::~Attack()
{

}

void Attack::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Attack::Finalize()
{

}

void Attack::Update()
{
	//��莞�Ԍo�߂����玩�g���폜����
	if (++count_up > time)
	{
		//�U���̏���
		manager->DeleteObject(this);
	}

	//�g��𐶐�
	if (count_up % 5 == 0)
	{
		manager->CreateEffect(elRipples, this->location, false, 30);
	}
	//�e�̈ړ�
	this->location += move_velocity;
}

void Attack::Draw()const
{
	__super::Draw();
	//�R�C����(�v���C���[)
	if (object->GetObjectType() == ePLAYER)
	{
		DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xffaa00, true);
	}
	//�R�C����(�G)
	else
	{
		DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xff0000, true);
	}
}

void Attack::Hit(ObjectBase* hit_object)
{
	//���������I�u�W�F�N�g���ЂƂO�̓������̂Ȃ�X�L�b�v
	if (old_hit_object == hit_object)
	{
		return;
	}
	//���������I�u�W�F�N�g�����S���o���Ȃ�X�L�b�v
	if (hit_object->GetDeathFlg())
	{
		return;
	}

	//�U�����m�����������ꍇ���A�����Ă�R�C���Ɠ��������ꍇ�A����
	if (hit_object->GetObjectType() == eATTACK || hit_object->GetObjectType() == eCOIN)
	{
		return;
	}

	//�U�������̂��v���C���[�ł͂Ȃ��A�v���C���[�ɂ��̍U�����������Ă���Ȃ�
	if (object->GetObjectType() != ePLAYER && hit_object->GetObjectType() == ePLAYER)
	{
		//�_���[�W
		hit_object->Damage(damage, this->location);
		//������������ۑ�����
		old_hit_object = hit_object;
		//�w�肵���񐔃I�u�W�F�N�g�ɓ������Ă���΁A�U���̏���
		if (++hit_count >= hit_max)
		{
			manager->DeleteObject(this);
		}
	}

	//�U�������̂��v���C���[�ŁA�v���C���[�ȊO�ɍU�����������Ă���Ȃ�
	if (object->GetObjectType() == ePLAYER && hit_object->GetObjectType() != ePLAYER)
	{
		//�_���[�W
		hit_object->Damage(damage, this->location);
		//������������ۑ�����
		old_hit_object = hit_object;
		//�w�肵���񐔃I�u�W�F�N�g�ɓ������Ă���΁A�������͐����Ă�G�T�ɒe������������A�U���̏���
		if (++hit_count >= hit_max || (hit_object->GetObjectType() == eENEMY5 && !hit_object->GetDeathFlg()))
		{
			manager->DeleteObject(this);
		}
	}
}
