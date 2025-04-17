#pragma once
#include "../../Utility/Vector2D.h"

#define PUSH_SHIFT 0.6f		  //Push�֐��ŃI�u�W�F�N�g���ړ����������ɏ�悹���鐔�l
#define SURFACE_THICKNESS 15  //�\�ʂ̌���

enum hitPosition
{
	top,
	bottom,
	left,
	right
};

class ShapeCollider
{
protected:
	Vector2D location;			//���S���W	
	Vector2D box_size;			//����̑傫��
	Vector2D local_location;	//���΍��W

public:

	void Initialize(Vector2D init_location = 0.0f, Vector2D init_size = 40.0f);		//������

	bool CheckHit(ShapeCollider* object2);			//�����蔻��̊m�F(����̃|�C���^��n���o�[�W����)
	bool CheckHit(Vector2D _loc2, Vector2D _size2);	//�����蔻��̊m�F(����̍��W�ƃT�C�Y��n���o�[�W����)

	void Push(ShapeCollider* hit_object);			//�Q�̃I�u�W�F�N�g�̍��W���󂯂Ƃ�A�K�؂Ȉړ��������s��

	//���W�̎擾
	Vector2D GetLocation()const { return location; }
	//���[�J�����W�̎擾
	Vector2D GetLocalLocation()const { return local_location; }
	//����̃T�C�Y�擾
	Vector2D GetSize()const { return box_size; }

	//���[�J�����W�̐ݒ�
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
};


