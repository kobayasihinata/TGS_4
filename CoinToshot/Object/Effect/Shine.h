#pragma once
#include "../Base/EffectBase.h"

class Shine :
	public EffectBase
{
private:
	int color;				//�G�t�F�N�g�F
	int size;				//�傫��
	float angle;			//��]�p
	float move_angle;		//��]�p�ړ��l�ۑ�
	Vector2D random_move;	//�����_���ȕ����Ɉړ�����

public:
	Shine(int _color);
	~Shine();

	//����������
	void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)override;

	//�I��������
	void Finalize()override;

	//�X�V����
	void Update()override;

	//�`�揈��
	void Draw()const override;
};