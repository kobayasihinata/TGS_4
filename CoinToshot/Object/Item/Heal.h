#pragma once
#include "../Base/ObjectBase.h"

class Heal :
	public ObjectBase
{
private:
	float heal_num;	//�񕜗�
public:
	//�R���X�g���N�^
	Heal();
	//�f�X�g���N�^
	~Heal();
	//����������
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//�I��������
	void Finalize()override;
	//�X�V����
	void Update()override;
	//�`�揈��
	void Draw()const override;
	//�����蔻�肪��������̏���
	void Hit(ObjectBase* hit_Object)override;
	//�_���[�W����
	void Damage(float _value, Vector2D _attack_loc)override {};
};

