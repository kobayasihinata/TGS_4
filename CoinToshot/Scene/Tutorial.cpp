#include "Tutorial.h"
#include "../Utility/UserData.h"

Tutorial* Tutorial::Get()
{
	static Tutorial instance;
	//�������g�̃|�C���^��ԋp����
	return &instance;
}


void Tutorial::Initialize()
{
	tutorial_flg = false;		
	tuto_stop_flg = false;		
	now_tuto = TutoType::tNone;	
	timer = 0;					
	text_alpha = 0;		

	for (int i = 0; i < 3; i++)
	{
		text_box[i] = MakeScreen(32, 32, TRUE);
	}
	CreateTextBox();
}

void Tutorial::Update()
{
	//��������̃`���[�g���A�������s���Ȃ�A�t���O�𗧂Ă�
	tutorial_flg = now_tuto != TutoType::tNone;

	//���s���̃`���[�g���A���ɉ����āA����������
	switch (now_tuto)
	{
	case TutoType::tRule:
		tuto_stop_flg = true;
		break;
	case TutoType::tMove:
		break;
	case TutoType::tAim:
		break;
	case TutoType::tAttack:
		break;
	case TutoType::tBulletChange:
		break;
	case TutoType::tEnemyBullet:
		break;
	case TutoType::tSlot:
		break;
	default:
		break;
	}

	//�`���[�g���A�����I�������t���O��������
	if (--timer <= 0)
	{
		tutorial_flg = false;
		tuto_stop_flg = false;
		now_tuto = TutoType::tNone;
		timer = 0;
	}

	if (tutorial_flg)
	{
		//�t�F�[�h���o
		if (timer > FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (timer <= FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}
	}
	else
	{
		text_alpha = 0;
	}
}

void Tutorial::Draw()const
{
	DrawFormatString(100, 80, 0x00ff00, "%d", timer);
	//���s���̃`���[�g���A���ɉ����āA����������
	switch (now_tuto)
	{
	case TutoType::tRule:
		DrawRule();
		break;
	case TutoType::tMove:
		DrawString(100, 100, "�������", 0x00ff00);
		break;
	case TutoType::tAim:
		DrawString(100, 100, "�G�C������", 0x00ff00);
		break;
	case TutoType::tAttack:
		DrawString(100, 100, "�U������", 0x00ff00);
		break;
	case TutoType::tBulletChange:
		DrawString(100, 100, "�e�ύX����", 0x00ff00);
		break;
	case TutoType::tEnemyBullet:
		DrawString(100, 100, "�G�U������", 0x00ff00);
		break;
	case TutoType::tSlot:
		DrawString(100, 100, "�X���b�g����", 0x00ff00);
		break;
	default:
		break;
	}
}

bool Tutorial::StartTutoRequest(TutoType _type)
{
	//���̃`���[�g���A�������s���Ȃ烊�N�G�X�g�͋p��
	if (tutorial_flg)
	{
		return false;
	}
	//�������Ă��Ȃ��Ȃ�`���[�g���A���J�n
	else
	{
		now_tuto = _type;
		timer = 60;
	}
}


void Tutorial::DrawRule()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawTextBox({ 200,200 }, { 200, 100 });
	DrawString(100, 100, "���[������", 0x00ff00);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::CreateTextBox()const
{
	//�p�̕`��
	SetDrawScreen(text_box[0]);
	ClearDrawScreen();

	DrawBox(2, 2, 32, 32, 0x000000, TRUE);
	DrawLine(2, 2, 32, 2, 0xffffff);
	DrawLine(2, 2, 2, 32, 0xffffff);
	DrawCircle(2, 2, 4, 0x000000, TRUE);
	DrawCircle(2, 2, 4, 0xffffff, FALSE);


	//�ӂ̕`��
	SetDrawScreen(text_box[1]);
	ClearDrawScreen();

	DrawBox(2, 2, 32, 32, 0x000000, TRUE);
	DrawLine(2, 2, 32, 2, 0xffffff);


	//�����̕`��
	SetDrawScreen(text_box[2]);
	ClearDrawScreen();

	DrawBox(0, 0, 32, 32, 0x000000, TRUE);

	SetDrawScreen(DX_SCREEN_BACK);
}

void Tutorial::DrawTextBox(Vector2D _loc, Vector2D _size)const
{
	for (int i = 0; i < 3; i++)
	{
		DrawGraph(_loc.x + 33 * i, _loc.y, text_box[i], TRUE);
	}
}