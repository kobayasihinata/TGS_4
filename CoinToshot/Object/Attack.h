#pragma once
#include "Base/ObjectBase.h"
#include "Base/BulletData.h"

class Attack :
	public ObjectBase
{
private:
	ObjectBase* object;			//�U���𐶐������I�u�W�F�N�g ���Ȃ����nullptr
	int time;					//���݂ł��鎞��
	int count_up;				//��������Ă���̎��ԑ���
	Vector2D move_velocity;		//�ړ�����

public:
	//�R���X�g���N�^(_object = �U���𐶐������I�u�W�F�N�g ���Ȃ����nullptr 
	// _bullet_data = ���ˎ��ɕK�v�ȏ��)
	Attack(BulletData _bullet_data);
	//�f�X�g���N�^
	~Attack();
	//����������
	void Initialize(ObjectManager* _manager , int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f,float init_radius = 0.f)override;
	//�I��������
	void Finalize()override;
	//�X�V����
	void Update()override;
	//�`�揈��
	void Draw()const override;
	//�����蔻�肪��������̏���
	void Hit(ObjectBase* hit_Object)override;
	//�_���[�W���� value=�_���[�W�̒l location=�U���̒��S���W 
	void Damage(float _value,Vector2D _attack_loc)override {};
};

