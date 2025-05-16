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
	//�l�p�`�p�̕ϐ�
	Vector2D box_size;			//����̑傫��

	////�O�p�`�p�̕ϐ�
	//Vector2D vert_location[3];	//���_���W
	//Vector2D vert_local_location[3];	//���_���W

	//�~�p�̕ϐ�
	float radius;				//���a

protected:
	Vector2D location;			//��Β��S���W	
	Vector2D local_location;	//���Β��S���W

public:

	void Initialize(Vector2D init_location = 0.0f, Vector2D init_size = 40.0f,float init_radius = 0.f);		//������(init_radius��0�Ȃ�l�p�`�Ƃ��Ĉ���)

	bool CheckHit(ShapeCollider* object);			//�����蔻��̊m�F(����̃|�C���^��n���o�[�W����)

	//bool CheckHit(Vector2D _loc2, float _radius);	//�����蔻��̊m�F(����̍��W�Ɣ��a��n���o�[�W����)

	bool CollisionCheckBtoB(Vector2D _loc, Vector2D _size)const;	//�����m�̓����蔻�� _loc,_size = ����̍��W�Ƒ傫��
	bool CollisionCheckBtoC(Vector2D _loc1, Vector2D _size1,Vector2D _loc2, float _radius2)const;	//���Ɖ~�̓����蔻�� _loc,_radius = ����̍��W�Ɣ��a
	bool CollisionCheckCtoC(Vector2D _loc, float _radius)const;	//�~���m�̓����蔻�� _loc,_radius = ����̍��W�Ɣ��a

	float DistanceSqrf(const float t_x1, const float t_y1, const float t_x2, const float t_y2)const;		//��_�̋��������߂�

	void Push(ShapeCollider* hit_object);			//�Q�̃I�u�W�F�N�g�̍��W���󂯂Ƃ�A�K�؂Ȉړ��������s��

	//���W�̎擾
	Vector2D GetLocation()const { return location; }
	//���[�J�����W�̎擾
	Vector2D GetLocalLocation()const { return local_location; }

	//����̃T�C�Y�擾(�l�p�`)
	Vector2D GetSize()const { return box_size; }
	//���a�擾(�~)
	float GetRadius()const { return radius; }

	//���[�J�����W�̐ݒ�
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
};


