#include "Tutorial.h"
#include "../Utility/UserData.h"
#include "../Utility/DebugInformation.h"

Tutorial* Tutorial::Get()
{
	static Tutorial instance;
	//�������g�̃|�C���^��ԋp����
	return &instance;
}


void Tutorial::Initialize()
{
	camera = Camera::Get();
	tutorial_flg = false;		
	tuto_stop_flg = false;		
	now_tuto = TutoType::tNone;	
	timer = 0;					
	text_alpha = 0;		
	stick_anim = 0;
	button_anim = 0;
	aim_timer = 0;
	aim_success_flg = false;
	aim_success_timer = 0;	
	attack_sequence = 0;
	attack_success_timer = 0;
	tuto_end_timer = 0;

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

	ex_anim_timer = 0;
	now_image = 0;

	ResourceManager* rm = ResourceManager::GetInstance();
	ex_anim = rm->GetImages("Resource/Images/Effect/E_PuffAndStar.png", 60, 10, 6, 108, 116);
	ex_se = rm->GetSounds("Resource/Sounds/explsion_big.mp3");
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
		break;
	case TutoType::tBulletChange:
		UpdateBulletChange();
		UpdateTimeTuto();
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
		DrawBulletChange();
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

bool Tutorial::StartTutoRequest(TutoType _type, ObjectBase* _obj)
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
		tuto_object = _obj;
		InitTuto(_type);
		return true;
	}
}

void Tutorial::InitTuto(TutoType _type)
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
		timer = 24;
		break;
	case TutoType::tMove:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 300, 100 };
		timer = 24;
		break;
	case TutoType::tAim:
		//�e�L�X�g�ʂɍ������傫���̔��摜�𐶐�
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 300, 100 };
		GenerateTextBox(text_box_size);
		//�\�����ɃQ�[�����~��Ԃ�
		tuto_stop_flg = true;
		timer = 18;
		break;
	case TutoType::tAttack:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100 };
		text_box_size = { 250, 100 };
		GenerateTextBox(text_box_size);
		tuto_stop_flg = true;
		timer = 18;
		break;
	case TutoType::tBulletChange:
		text_box_loc = { SCREEN_WIDTH / 2, 150 };
		text_box_size = { 250, 100 };
		GenerateTextBox(text_box_size);
		tuto_stop_flg = true;
		ex_anim_flg = true;
		timer = 240;
		PlaySoundMem(ex_se, DX_PLAYTYPE_BACK);
		break;
	case TutoType::tEnemyBullet:
		break;
	case TutoType::tSlot:
		break;
	default:
		break;
	}
}

void Tutorial::TutoEnd()
{
	tutorial_flg = false;
	tuto_stop_flg = false;
	tuto_executed_flg[(int)now_tuto > 7 ? 7 : (int)now_tuto] = true;
	now_tuto = TutoType::tNone;
	timer = 0;
	text_alpha = 0;
}

bool Tutorial::CheckAim()
{
	//�p�x�������ύX����Ă��Ȃ��Ȃ�A�������Ȃ�
	if (player_aim_rad > 100)return false;
	//�U��
	Vector2D attack_loc = camera->player_location;
	float attack_rad = 20;

	for (int i = 0; i < 400; i++)
	{
		attack_loc.x += 2 * cosf(player_aim_rad);
		attack_loc.y += 2 * sinf(player_aim_rad);
		if (tuto_object->CollisionCheckBtoC(tuto_object->GetLocation(), tuto_object->GetSize(), attack_loc, attack_rad))
		{
			return true;
		}
	}	
	return false;
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
		TutoEnd();
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
		//������悤�ɂ���
		if (tuto_stop_flg)
		{
			tuto_stop_flg = false;
		}
		//�Ə���G�ɍ��킹����`���[�g���A�����I�����čU���`���[�g���A��
		if (!aim_success_flg && CheckAim())
		{
			if (++aim_timer > 90)
			{
				aim_success_flg = true;
				aim_timer = 0;
			}
		}
		//�Ə������ꂽ�烊�Z�b�g
		else
		{
			aim_timer = 0;
		}
		//�Ə��𐬌��������牉�o
		if (aim_success_flg)
		{
			//��莞�ԂŃ`���[�g���A���I��
			if (++aim_success_timer > 120)
			{
				//�ꉞ���Z�b�g
				aim_success_flg = false;
				aim_success_timer = 0;
				TutoEnd();
				StartTutoRequest(TutoType::tAttack, tuto_object);
			}
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
		//�Ə�
		if (aim_timer > 0)
		{
			DrawBoxAA(tuto_object->GetLocalLocation().x - 30,
				tuto_object->GetLocalLocation().y       - 70,
				tuto_object->GetLocalLocation().x       - 30 + (50.f / 30) * (aim_timer % 30),
				tuto_object->GetLocalLocation().y       - 40,
				0xffaa00, TRUE);
			DrawBoxAA(tuto_object->GetLocalLocation().x - 30,
				tuto_object->GetLocalLocation().y       - 70,
				tuto_object->GetLocalLocation().x       + 30,
				tuto_object->GetLocalLocation().y       - 40,
				0xffffff, FALSE);
			DrawFormatString(tuto_object->GetLocalLocation().x + 1, tuto_object->GetLocalLocation().y - 59, 0x000000, "%d", 3 - (aim_timer / 30));
			DrawFormatString(tuto_object->GetLocalLocation().x, tuto_object->GetLocalLocation().y - 60, 0xffff00, "%d", 3 - (aim_timer / 30));
		}
		//�Ə����킹�����`��
		if (aim_success_flg)
		{
			DrawBoxAA(tuto_object->GetLocalLocation().x - 30,
				tuto_object->GetLocalLocation().y - 70,
				tuto_object->GetLocalLocation().x + 30,
				tuto_object->GetLocalLocation().y - 40,
				0xffaa00, TRUE);
			DrawBoxAA(tuto_object->GetLocalLocation().x - 30,
				tuto_object->GetLocalLocation().y - 70,
				tuto_object->GetLocalLocation().x + 30,
				tuto_object->GetLocalLocation().y - 40,
				0xffffff, FALSE);
			DrawString(tuto_object->GetLocalLocation().x - 30, tuto_object->GetLocalLocation().y - 70, "nice!", aim_success_timer % 30 > 15 ? 0xaaaa00 : 0xffff00);
		}
	}
}

void Tutorial::UpdateAttack()
{
	timer--;
	switch (attack_sequence)
	{
	case 0: //�����e�L�X�g�\����

		//�t�F�[�h���o
		if (timer > FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (timer <= FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}

		//�\���I���Ŏ��H�菇�ڍs
		if (timer < 0)attack_sequence = 1;
		break;

	case 1:	//���H
		//�{�^���A�j���[�V����
		if (timer % 10 == 0)
		{
			if (++button_anim > 1)
			{
				button_anim = 0;
			}
		}
		//������悤�ɂ���
		if (tuto_stop_flg)
		{
			tuto_stop_flg = false;
		}
		//�G�����݂��Ă���Ԃ͍��W���擾
		if (!tuto_object->GetDeathFlg())
		{
			enemy_loc = tuto_object->GetLocation();
		}
		//�G������ł����琬���`��菇
		else {
			attack_sequence = 2;
		}
		break;

	case 2:	//�����`��菇
		if (++attack_success_timer > 120)
		{
			attack_sequence = 3;
			text_alpha = 0;
		}
		break;
	case 3: //��{�����������
		//�����Ȃ��悤�ɂ���
		if (!tuto_stop_flg)
		{
			tuto_stop_flg = true;
		}
		if (++tuto_end_timer > 180)
		{
			tuto_end_timer = 0;
			TutoEnd();
		}
		//�t�F�[�h���o
		if (tuto_end_timer <= FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (tuto_end_timer > 180 - FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}

		break;
	}
}
void Tutorial::DrawAttack()const
{
	switch (attack_sequence)
	{
	case 0: //�����e�L�X�g�\����
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
		DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
		
		SetFontSize(32);
		DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 50, "�R�C�����΂���", 0xffffff);
		DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 20, "   �G���U���I", timer % 30 > 15 ? 0xffffff : 0xff0000);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case 1:	//���H
		SetFontSize(24);
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "�� or �E�g���K�[�F�U��", 0xffffff);
		DrawRotaGraphF(text_box_loc.x - 40, text_box_loc.y + 50, 1.f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_LEFT_SHOULDER], TRUE);
		DrawRotaGraphF(text_box_loc.x + 40, text_box_loc.y + 50, 1.f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_RIGHT_SHOULDER], TRUE);
		break;
	case 2:	//�����`��菇
		DrawString(enemy_loc.x - camera->GetCameraLocation().x - 25, enemy_loc.y - camera->GetCameraLocation().y - 70, "nice!", attack_success_timer % 30 > 15 ? 0xaaaa00 : 0xff4400);
		break;
	case 3: //��{�����������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
		DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
		
		SetFontSize(32);
		DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 50, "��{����͈ȏ�I", 0xffffff);
		DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 20, "�����ē|���ĉ҂��I", 0xffffff);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	}
}


void Tutorial::UpdateBulletChange()
{
	if (ex_anim_flg && ++ex_anim_timer % 2 == 0)
	{
		if (now_image < ex_anim.size() - 1)
		{
			now_image++;
		}
		else
		{
			//�A�j���[�V�����I��
			ex_anim_flg = false;
		}
	}
}
void Tutorial::DrawBulletChange()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
	SetFontSize(24);
	DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 40, "�e�̕ύX���A�����b�N�I", 0xffffff);
	DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 20, "�󋵂ɉ����Ďg�������悤", 0xffffff);
	SetFontSize(18);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y + 30 }, "���E�g���K�[�ŕύX", 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//�����A�j���[�V����
	if (ex_anim_flg)
	{
		DrawRotaGraphF(SCREEN_WIDTH / 2 - 130, 30, 1.f, 0, ex_anim[now_image], TRUE);
		DrawRotaGraphF(SCREEN_WIDTH / 2 + 130, 30, 1.f, 0, ex_anim[now_image], TRUE);
	}
}