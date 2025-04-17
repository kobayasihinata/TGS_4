#include "Attack.h"
#include "ObjectManager.h"
#include "../Utility/common.h"

Attack::Attack(BulletData _bullet_data)
{
	object = _bullet_data.who;
	time = _bullet_data.delete_time;
	count_up = 0;
	move_velocity.x = _bullet_data.speed * cosf(_bullet_data.b_angle);
	move_velocity.y = _bullet_data.speed * sinf(_bullet_data.b_angle);

	//location.x += ((b_speed + acceleration * 0.08) * cosf(rad));
	//location.y += ((b_speed + acceleration * 0.08) * sinf(rad));
}

Attack::~Attack()
{

}

void Attack::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
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


	//�e�̈ړ�
	this->location += move_velocity;
}

void Attack::Draw()const
{
	__super::Draw();
	//�U��
	DrawString(local_location.x, local_location.y, "attack", 0xffffff);
}

void Attack::Hit(ObjectBase* hit_object)
{
	//�U�����m�����������ꍇ�A����
	if (hit_object->GetObjectType() == eATTACK)
	{
		return;
	}
	//�U�������̂��v���C���[�ł͂Ȃ��A�v���C���[�ɂ��̍U�����������Ă���Ȃ�
	if (object->GetObjectType() != ePLAYER && hit_object->GetObjectType() == ePLAYER)
	{
		//�_���[�W
		hit_object->Damage(1, this->location + (this->box_size / 2));
		//�U���̏���
		manager->DeleteObject(this);
	}

	//�U�������̂��v���C���[�ŁA�v���C���[�ȊO�ɍU�����������Ă���Ȃ�
	if (object->GetObjectType() == ePLAYER && hit_object->GetObjectType() != ePLAYER)
	{
		//�_���[�W
		hit_object->Damage(1, this->location + (this->box_size / 2));
		//�U���̏���
		manager->DeleteObject(this);
	}
}
