#pragma once
#include "../Base/EnemyBase.h"
class Enemy5 :
	public EnemyBase
{
private:
	int block_anim_timer;	//�h��G�t�F�N�g�A�j���[�V����
	Vector2D attack_loc;	//�A�j���[�V�����p �U���̍��W
public:
	//�R���X�g���N�^
	Enemy5();
	//�f�X�g���N�^
	~Enemy5();
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
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;

};

