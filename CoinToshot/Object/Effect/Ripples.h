#pragma once
#include "../Base/EffectBase.h"

class Ripples :
	public EffectBase
{
private:

public:
	Ripples();
	~Ripples();

	//����������
	void Initialize(ObjectManager* _manager, Vector2D init_location, int _timer, int _anim_span)override;

	//�I��������
	void Finalize()override;

	//�X�V����
	void Update()override;

	//�`�揈��
	void Draw()const override;
};