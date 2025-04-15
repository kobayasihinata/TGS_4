#include "Attack.h"
#include "ObjectManager.h"

Attack::Attack(ObjectBase* _object, int _time)
{
	object = _object;
	time = _time;
	count_up = 0;
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
}

void Attack::Draw()const
{
	__super::Draw();
	//�U��
	DrawString(local_location.x, local_location.y, "attack", 0xffffff);
}

void Attack::Hit(ObjectBase* hit_object)
{
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
