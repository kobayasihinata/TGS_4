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
	stick_anim = 0;
	button_anim = 0;
	text_box_loc = 0;
	text_box_size = 0;

	for (int i = 0; i < 3; i++)
	{
		text_box[i] = MakeScreen(32, 32, TRUE);
	}
	for (int i = 0; i < TUTO_NUM; i++)
	{
		tuto_executed_flg[i] = false;
	}

	CreateTextBox();
	generate_text_box = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, TRUE);
}

void Tutorial::Update()
{
	//��������̃`���[�g���A�������s���Ȃ�A�t���O�𗧂Ă�
	tutorial_flg = now_tuto != TutoType::tNone;

	//���s���̃`���[�g���A���ɉ����āA����������
	switch (now_tuto)
	{
	case TutoType::tRule:

		//��莞�ԂŏI���^�C�v�̃`���[�g���A���͂��̍X�V//
		UpdateTimeTuto();

		break;
	case TutoType::tMove:

		UpdateMove();
		UpdateTimeTuto();
		break;
	case TutoType::tAim:
		//�v���C���[�̍s���ȂǁA���ԈȊO�ŏI���`���[�g���A���Ȃ�ʓr����
		UpdateAim();
		break;
	case TutoType::tAttack:
		UpdateAttack();
		UpdateTimeTuto();
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
}

void Tutorial::Draw()const
{
	int old = GetFontSize();
	DrawFormatString(100, 80, 0x00ff00, "%d", timer);

	//���s���̃`���[�g���A���ɉ����āA����������
	switch (now_tuto)
	{
	case TutoType::tRule:
		DrawRule();
		break;
	case TutoType::tMove:
		DrawMove();
		break;
	case TutoType::tAim:
		DrawAim();
		break;
	case TutoType::tAttack:
		DrawAttack();
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

	SetFontSize(old);
}

bool Tutorial::StartTutoRequest(TutoType _type, Vector2D _loc)
{
	//���̃`���[�g���A�������s���@�������͊��Ɉ��s�����`���[�g���A���Ȃ烊�N�G�X�g�͋p��
	if (tutorial_flg || tuto_executed_flg[_type])
	{
		return false;
	}
	//�������Ă��Ȃ��Ȃ�`���[�g���A���J�n
	else
	{
		now_tuto = _type;
		InitTuto(_type,_loc);
	}
}

void Tutorial::InitTuto(TutoType _type,Vector2D _loc)
{
	switch (now_tuto)
	{
	case TutoType::tRule:
		//�e�L�X�g�ʂɍ������傫���̔��摜�𐶐�
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 300, 100 };
		GenerateTextBox(text_box_size);
		//�\�����ɃQ�[�����~��Ԃ�
		tuto_stop_flg = true;
		timer = 240;
		break;
	case TutoType::tMove:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 300, 100 };
		timer = 240;
		break;
	case TutoType::tAim:
		//�e�L�X�g�ʂɍ������傫���̔��摜�𐶐�
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 300, 100 };
		GenerateTextBox(text_box_size);
		//�\�����ɃQ�[�����~��Ԃ�
		tuto_stop_flg = true;
		timer = 180;
		break;
	case TutoType::tAttack:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 300, 100 };
		timer = 180;
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
}

void Tutorial::CreateTextBox()const
{
	//�p�̕`��
	SetDrawScreen(text_box[0]);
	ClearDrawScreen();

	DrawBox(2, 2, 32, 32, 0x000000, TRUE);
	DrawLine(2, 2, 32, 2, 0xffffff);
	DrawLine(2, 2, 2, 32, 0xffffff);
	DrawCircle(2, 2, 3, 0x000000, TRUE);
	DrawCircle(2, 2, 3, 0xffffff, FALSE);


	//�ӂ̕`��
	SetDrawScreen(text_box[1]);
	ClearDrawScreen();

	DrawBox(0, 2, 32, 32, 0x000000, TRUE);
	DrawLine(0, 2, 32, 2, 0xffffff);


	//�����̕`��
	SetDrawScreen(text_box[2]);
	ClearDrawScreen();

	DrawBox(0, 0, 32, 32, 0x000000, TRUE);
	//for (int x = 0; x < 8; x++)
	//{
	//	for (int y = 0; y < 8; y++)
	//	{
	//		//�`�F�b�N
	//		DrawBox((x * 4), (y * 4), (x * 4) + 4, (y * 4) + 4, (x + y) % 2 == 0 ? 0xffffff : 0x000000, TRUE);
	//	}
	//}
	SetDrawScreen(DX_SCREEN_BACK);
}

void Tutorial::GenerateTextBox(Vector2D _size)const
{
	Vector2D size = { _size.x > 37 ? _size.x : 37,_size.y > 37 ? _size.y : 37 };

	SetDrawScreen(generate_text_box);
	ClearDrawScreen();

	//�l������`��
	DrawRotaGraphF(		  (TEXT_BOX / 2),		     (TEXT_BOX / 2), 1.0f, 0.f *3.14f,  text_box[0],TRUE);
	DrawRotaGraphF(size.x - (TEXT_BOX / 2),		     (TEXT_BOX / 2), 1.0f, 0.5f*3.14f,  text_box[0],TRUE);
	DrawRotaGraphF(size.x - (TEXT_BOX / 2),  size.y - (TEXT_BOX / 2), 1.0f, 1.f *3.14f,  text_box[0],TRUE);
	DrawRotaGraphF(          (TEXT_BOX / 2),  size.y - (TEXT_BOX / 2), 1.0f, 1.5f*3.14f,  text_box[0],TRUE);

	//����
	if (size.x > 64 && size.y > 64)
	{
		for (int i = 0; i < size.x - (TEXT_BOX * 2); i += TEXT_BOX)
		{
			//y��
			for (int j = 0; j < size.y - (TEXT_BOX * 2); j += TEXT_BOX)
			{
				DrawRotaGraphF((TEXT_BOX * 1.5f) + i, (TEXT_BOX * 1.5f) + j, 1.0f, 0.f, text_box[2], TRUE);
			}
		}

	}
	//���Ԃ�ӂŖ��߂�

	//X��
	for (int i = 0; i < size.x - (TEXT_BOX*2); i += TEXT_BOX)
	{
		DrawRotaGraphF((TEXT_BOX * 1.5) + i,  (TEXT_BOX / 2), 1.0f, 0.f * 3.14f, text_box[1], TRUE);
		DrawRotaGraphF((TEXT_BOX * 1.5) + i,  size.y - (TEXT_BOX / 2), 1.0f, 1.f * 3.14f, text_box[1], TRUE);
	}
	//y��
	for (int i = 0; i < size.y - (TEXT_BOX * 2); i += TEXT_BOX)
	{
		DrawRotaGraphF((TEXT_BOX / 2), (TEXT_BOX * 1.5) + i, 1.0f, 1.5f * 3.14f, text_box[1], TRUE);
		DrawRotaGraphF(size.x - (TEXT_BOX / 2), (TEXT_BOX * 1.5) + i, 1.0f, 0.5f * 3.14f, text_box[1], TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void Tutorial::UpdateTimeTuto()
{
	//�`���[�g���A�����I�������t���O��������
	if (--timer <= 0)
	{
		tutorial_flg = false;
		tuto_stop_flg = false;
		tuto_executed_flg[(int)now_tuto > 7 ? 7 : (int)now_tuto] = true;
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

void Tutorial::UpdatePracticeTuto()
{

}

void Tutorial::DrawRule()const
{
	SetFontSize(24);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "���[������", 0xffffff);
	SetFontSize(32);
	DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 20, "�Ђ�����R�C�����҂��I", 0xeeeeee);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::UpdateMove()
{
	//�X�e�B�b�N���񂷃A�j���[�V����
	if (timer % 10 == 0)
	{
		if (++stick_anim > 3)
		{
			stick_anim = 0;
		}
	}
}
void Tutorial::DrawMove()const
{
	SetFontSize(24);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "���X�e�B�b�N�F�ړ�", 0xffffff);
	DrawRotaGraphF(text_box_loc.x, text_box_loc.y + 50, 1.f, 0, UserData::button_image[1][l_stick[stick_anim]], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::UpdateAim()
{
	//�X�e�B�b�N���񂷃A�j���[�V����
	if (timer % 10 == 0)
	{
		if (++stick_anim > 3)
		{
			stick_anim = 0;
		}
	}
	if (tutorial_flg)
	{
		//�t�F�[�h���o
		if (--timer > FADE_TIME && text_alpha < 255)
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
	//�^�C�}�[���}�C�i�X�ɒB��������H�J�n
	if (timer < 0)
	{
		if (tuto_stop_flg == true)
		{
			tuto_stop_flg = false;
		}
		//�Ƃ肠�����Ő��b������Ə��`���[�g���A�����I�����čU���`���[�g���A��
		if (timer < -180)
		{
			///////�`���[�g���A���I�����͂��̏���
			tutorial_flg = false;
			tuto_stop_flg = false;
			tuto_executed_flg[(int)now_tuto > 7 ? 7 : (int)now_tuto] = true;
			now_tuto = TutoType::tNone;
			timer = 0;
			text_alpha = 0;
			///////
			StartTutoRequest(TutoType::tAttack);
		}
	}
}
void Tutorial::DrawAim()const
{
	//�����̃e�L�X�g�{�b�N�X
	if (timer > 0)
	{
		SetFontSize(24);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
		DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
		SetFontSize(32);
		DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 20, "�G�ɏƏ������킹�悤", 0xffffff);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//���H�̃^�[�Q�b�g
	else
	{
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "�E�X�e�B�b�N�F�Ə�", 0xffffff);
		DrawRotaGraphF(text_box_loc.x, text_box_loc.y + 50, 1.f, 0, UserData::button_image[1][r_stick[stick_anim]], TRUE);
	}
}

void Tutorial::UpdateAttack()
{
	//�X�e�B�b�N���񂷃A�j���[�V����
	if (timer % 10 == 0)
	{
		if (++button_anim > 1)
		{
			button_anim = 0;
		}
	}
}
void Tutorial::DrawAttack()const
{
	SetFontSize(24);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "�� or �E�g���K�[�F�U��", 0xffffff);
	DrawRotaGraphF(text_box_loc.x-40, text_box_loc.y + 50, 1.f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_LEFT_SHOULDER], TRUE);
	DrawRotaGraphF(text_box_loc.x+40, text_box_loc.y + 50, 1.f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_RIGHT_SHOULDER], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}