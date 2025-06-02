#pragma once
#include "../Base/EffectBase.h"

class Shine :
	public EffectBase
{
private:
	int color;	//�G�t�F�N�g�F
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