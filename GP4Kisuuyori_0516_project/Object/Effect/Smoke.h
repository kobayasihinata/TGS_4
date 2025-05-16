#pragma once
#include "../Base/EffectBase.h"

class Smoke :
	public EffectBase
{
private:

public:
	Smoke();
	~Smoke();

	//����������
	void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)override;

	//�I��������
	void Finalize()override;

	//�X�V����
	void Update()override;

	//�`�揈��
	void Draw()const override;
};