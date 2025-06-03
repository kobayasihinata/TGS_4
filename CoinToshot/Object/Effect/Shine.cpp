#include "Shine.h"
#include "../ObjectManager.h"

Shine::Shine(int _color)
{
	color = _color;
	size = 0;
	angle = 0.f;
	move_angle = (float)(GetRand(100)-50) / 500;
	random_move = { (float)(GetRand(500)-250) / 500,(float)(GetRand(500)-250) / 500 };
}

Shine::~Shine()
{

}

void Shine::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span);
	//�������Ԃ������_����
	timer = GetRand(25) + 5;
	init_timer = timer;
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

	//�T�C�Y��傫�������菬����������
	if (timer < (init_timer / 2))
	{
		size++;
	}
	else
	{
		size--;
	}
	//�p�x��ς���
	angle += move_angle;
	//�����_���Ȓl���ړ�
	location += random_move;
}

//�`�揈��
void Shine::Draw()const
{
	float a_cos1 = size * cosf(angle);
	float a_sin1 = size * sinf(angle);
	float a_cos2 = size * cosf(angle + 1.5f);
	float a_sin2 = size * sinf(angle + 1.5f);

	//���ŕ`��
	DrawLineAA(local_location.x+1 - a_cos1,
		local_location.y+1 - a_sin1,
		local_location.x+1 + a_cos1,
		local_location.y+1 + a_sin1,
		0xffffff, TRUE);
	DrawLineAA(local_location.x - a_cos2,
		local_location.y+1  - a_sin2,
		local_location.x+1  + a_cos2,
		local_location.y+1  + a_sin2,
		0xffffff, TRUE);
	DrawLineAA(local_location.x - a_cos1,
			   local_location.y - a_sin1,
			   local_location.x + a_cos1,
			   local_location.y + a_sin1,
		color, TRUE);			  					 
 	DrawLineAA(local_location.x - a_cos2,
			   local_location.y - a_sin2,
			   local_location.x + a_cos2,
			   local_location.y + a_sin2,
		color, TRUE);
}