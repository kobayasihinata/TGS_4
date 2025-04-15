#pragma once
#include "../../Utility/Vector2D.h"

#define PUSH_SHIFT 0.6f		  //Push�֐��ŃI�u�W�F�N�g���ړ����������ɏ�悹���鐔�l
#define SURFACE_THICKNESS 15  //�\�ʂ̌���

class ObjectBase;
class ActorBase;

enum hitPosition
{
	top,
	bottom,
	left,
	right
};

class ShapeCollider
{
private:
	Vector2D location;			//��΍��W	
	Vector2D box_size;			//����̑傫��
	
public:
	Vector2D local_location;	//���΍��W

public:

	void Initialize();		//������

	bool CheckHit(ShapeCollider* object1, ShapeCollider* object2);			//�����蔻��̊m�F
	bool CheckHit(Vector2D _loc, Vector2D _size, ShapeCollider* object2);	//�����蔻��̊m�F(���g�̍��W�ƃT�C�Y��n���o�[�W����)
	bool CheckHit(Vector2D _loc1, Vector2D _size1, Vector2D _loc2, Vector2D _size2);	//�����蔻��̊m�F(���g�Ƒ���̍��W�ƃT�C�Y��n���o�[�W����)

	//���W�̎擾
	Vector2D GetLocation()const { return location; }
	//���[�J�����W�̎擾
	Vector2D GetLocalLocation()const { return local_location; }
	//����̃T�C�Y�擾
	Vector2D GetSize()const { return box_size; }
};


