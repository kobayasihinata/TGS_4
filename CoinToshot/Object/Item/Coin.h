#pragma once
#include "../Base/ObjectBase.h"

class InGameScene;

class Coin :
	public ObjectBase
{
private:
	InGameScene* ingame;		//���݂̃V�[���̃|�C���^��ۑ�

public:
	float add_num;	//�R�C���ǉ���
	bool check_flg;	//�R�C�������̂��邩����x�������f����p	

public:
	//�R���X�g���N�^
	Coin(InGameScene* _ingame);
	//�f�X�g���N�^
	~Coin();
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

