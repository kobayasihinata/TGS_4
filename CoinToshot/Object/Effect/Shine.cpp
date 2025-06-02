#include "Shine.h"
#include "../ObjectManager.h"

Shine::Shine(int _color)
{
	color = _color;
}

Shine::~Shine()
{

}

void Shine::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span);
	init_timer = _timer;
}

//�I��������
void Shine::Finalize()
{

}

//�X�V����
void Shine::Update()
{
	//���Ԃ��߂����玩�g������
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//�`�揈��
void Shine::Draw()const
{
	DrawLineAA(local_location.x - 5,
			 local_location.y - 5,
			 local_location.x + 5,
			 local_location.y + 5,
		color, TRUE);
	DrawLineAA(local_location.x + 5,
			 local_location.y - 5,
			 local_location.x - 5,
			 local_location.y + 5,
		color, TRUE);
}