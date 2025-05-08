#pragma once
#include "../Base/EnemyBase.h"
class Enemy4 :
	public EnemyBase
{
private:
	int coin_num;	//�����Ă���R�C��
	bool steal_flg;	//�R�C���𓐂񂾂�

public:
	//�R���X�g���N�^
	Enemy4();
	//�f�X�g���N�^
	~Enemy4();
	//����������
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//�I��������
	void Finalize()override;
	//�X�V����
	void Update()override;
	//�`�揈��
	void Draw()const override;
	//�����蔻�肪��������̏���
	void Hit(ObjectBase* hit_object)override;
	//�_���[�W����
	void Damage(float _value, Vector2D _attack_loc)override;

	//Enemy4��p�ړ�����
	void Enemy4Move();
};

