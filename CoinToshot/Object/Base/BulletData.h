#pragma once
#include "../../Utility/Vector2D.h"
#include "ObjectBase.h"

struct BulletData {
	Vector2D location;	//���W
	float radius;		//���a
	float speed;		//���x
	ObjectBase* who;			//�N���ł��o������
	float b_angle;		//�p�x
	//int b_num;		 //�e�𓯎��ɉ�������
	int h_count;		//�ђʏo����G�̐�
	int delete_time;	//�e�������܂ł̎���
	//int b_type;		 //�e�̎��
};
