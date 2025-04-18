#pragma once
#include "Base/ObjectBase.h"

class Block :
	public ObjectBase
{
private:

public:
	//�R���X�g���N�^
	Block();
	//�f�X�g���N�^
	~Block();
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
};

