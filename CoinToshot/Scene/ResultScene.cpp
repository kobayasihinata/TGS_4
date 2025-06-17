#include "ResultScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

ResultScene::ResultScene()
{
	now_disp = DispScene::dIsClear;

	start_anim_timer= 0;
	result_anim_timer = 0;
	bonus_anim_timer = 0;
	add_anim_num = ((int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)) / (BONUS_ANIM_TIME / 5);
	add_anim_coin = UserData::coin;
	add_coin_once = false;
	disp_se_once = true;

	name_string_loc = { 500,70 };
	key_box_loc = { 400,200 };
	current_x = 0;
	current_y = 0;

	//���g�̃f�[�^�����Z�b�g���Ă���
	UserData::my_ranking_data = { -1,"���肦�Ȃ�",-1 };

	//BGM�ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();
	result_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	button_se = rm->GetSounds("Resource/Sounds/enter.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	erase_se = rm->GetSounds("Resource/Sounds/explosion.wav");
	//�N���A���Q�[���I�[�o�[����SE��ς���
	disp_se = UserData::is_clear ? rm->GetSounds("Resource/Sounds/Coin/Get.mp3"): rm->GetSounds("Resource/Sounds/bishi.wav");
	coin_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	SetVolumeSoundMem(8000, result_bgm);
	if (!CheckSoundMem(result_bgm))
	{
		PlaySoundMem(result_bgm, DX_PLAYTYPE_LOOP);
	}
}

ResultScene::~ResultScene()
{
	StopSoundMem(result_bgm);
}

void ResultScene::Initialize()
{
	__super::Initialize();
}

eSceneType ResultScene::Update(float _delta)
{
	if (!disp_se_once)
	{
		PlaySoundMem(disp_se, DX_PLAYTYPE_BACK);
		disp_se_once = true;
	}
	switch (now_disp)
	{
	case DispScene::dIsClear:	//�Q�[���N���A���I�[�o�[���\��
		start_anim_timer++;
		//��莞�Ԍo�ߌ�AA�{�^�����������Ƃ�
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//���o���I�����Ă�����{�[�i�X���Z���o�J�n
			if (start_anim_timer > START_ANIM_TIME)
			{
				now_disp = DispScene::dDispResult;
				PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
			}
			//�I�����Ă��Ȃ���Ή��o�X�L�b�v
			else
			{
				//�X�L�b�v�͉��o�I����15�t���[���O�܂�
				start_anim_timer = start_anim_timer >= START_ANIM_TIME - SKIP_TIME ? start_anim_timer : START_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dDispResult://���ʂ�\��
		result_anim_timer++;
		//�^�C�}�[�Ō��ʂ����Ԃɕ\��
		if (result_anim_timer == RESULT_ANIM_TIME / 5 ||
			result_anim_timer == (RESULT_ANIM_TIME / 5) * 2 ||
			result_anim_timer == (RESULT_ANIM_TIME / 5) * 3)
		{
			//SE�Đ�
			disp_se_once = false;
		}
		//A�{�^�����������Ƃ�
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//���o���I�����Ă�����{�[�i�X���Z���o�J�n
			if (result_anim_timer > RESULT_ANIM_TIME)
			{
				now_disp = DispScene::dBonusPoint;
				PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
			}
			//�I�����Ă��Ȃ���Ή��o�X�L�b�v
			else
			{
				//�X�L�b�v�͉��o�I����15�t���[���O�܂�
				result_anim_timer = result_anim_timer >= RESULT_ANIM_TIME - SKIP_TIME ? result_anim_timer : RESULT_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dBonusPoint://�_�����Z��\��
		bonus_anim_timer++;
		if (bonus_anim_timer ==  BONUS_ANIM_TIME / 5 ||
			bonus_anim_timer == (BONUS_ANIM_TIME / 5) * 2 ||
			bonus_anim_timer == (BONUS_ANIM_TIME / 5) * 3)
		{
			//SE�Đ�
			disp_se_once = false;
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5) * 4)
		{
			if (!add_coin_once)
			{
				UserData::coin += (int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60);
				add_coin_once = true;
			}
			//���Z���o
			if (bonus_anim_timer < BONUS_ANIM_TIME - SKIP_TIME)
			{
				add_anim_coin += add_anim_num;
				if (bonus_anim_timer % 3 == 0)
				{
					PlaySoundMem(coin_se, DX_PLAYTYPE_BACK);
				}
			}

		}
		//A�{�^�����������Ƃ�
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//���o���I�����Ă�����J��
			if (bonus_anim_timer > BONUS_ANIM_TIME)
			{
				PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
				//�����L���O�ŉ��ʂ̃X�R�A�������Ă�����
				if (UserData::coin > UserData::ranking_data[9].coin)
				{
					//���O���͂ɑJ��
					now_disp = DispScene::dEnterName;
				}
				//�����N�O�Ȃ烉���L���O�\���V�[���ɔ��
				else
				{
					return eSceneType::eRanking;
				}

			}
			//�I�����Ă��Ȃ���Ή��o�X�L�b�v
			else
			{
				//�X�L�b�v�͉��o�I���̈��t���[���O�܂�
				bonus_anim_timer = bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? bonus_anim_timer : BONUS_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dEnterName:
		//���O����
		return EnterName();
		break;
	default:
		break;
	}

#ifdef _DEBUG
	//���͋@�\�̎擾
	InputKey* input = InputKey::Get();

	//1�L�[�Ń^�C�g����ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}
#endif // _DEBUG

	return __super::Update(_delta);
}

void ResultScene::Draw()const
{
	int bg_color, text_color1, text_color2, text_color3;
	DrawString(10, 10, "Result", GetColor(255, 255, 255));

	switch (now_disp)
	{
	case DispScene::dIsClear:
		if (UserData::is_clear)
		{
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, start_anim_timer * (255 / (START_ANIM_TIME / 2)));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffbb, TRUE);
			SetFontSize(64);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 -1,SCREEN_HEIGHT / 2 - 51 }, "Congratulations...!", 0x000000);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 +1,SCREEN_HEIGHT / 2 - 49 }, "Congratulations...!", 0x000000);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2   ,SCREEN_HEIGHT / 2 - 50 }, "Congratulations...!", 0xffff00);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else
		{
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, start_anim_timer * (255 / (START_ANIM_TIME / 2)));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
			SetFontSize(64);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 - 1,SCREEN_HEIGHT / 2 - 51 }, "Nice try!", 0xffffff);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 + 1,SCREEN_HEIGHT / 2 - 49 }, "Nice try!", 0xffffff);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2   ,SCREEN_HEIGHT / 2 - 50 },  "Nice try!", 0xbbbb00);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}

		if (start_anim_timer > START_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, 60);
		}

		break;
	case DispScene::dDispResult:
		if (UserData::is_clear)
		{
			bg_color = 0xffffbb;
			text_color1 = 0x888800;
			text_color2 = 0xaaaa00;
			text_color3 = 0xffbb00;
		}
		else
		{
			bg_color = 0x555500;
			text_color1 = 0xaaaa00;
			text_color2 = 0xbbbb00;
			text_color3 = 0xaa6600;
		}

		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_color, TRUE);
		SetFontSize(64);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "Result", result_anim_timer % 120 > 60 ? text_color1 : text_color2);
		SetFontSize(36);
		//�^�C�}�[�Ō��ʂ����Ԃɕ\��
		if (result_anim_timer > RESULT_ANIM_TIME / 5)
		{
			DrawString(SCREEN_WIDTH / 2 - 120, 250, "Time:", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)), 250, text_color1, "%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			if(DEFAULT_TIMELIMIT - UserData::timer == DEFAULT_TIMELIMIT)DrawString(SCREEN_WIDTH / 2 + 130, 250, "�� MAX!!", result_anim_timer % 30 > 15 ? text_color1 : text_color3);
		}
		if (result_anim_timer > (RESULT_ANIM_TIME / 5) * 2)
		{
			DrawString(SCREEN_WIDTH / 2 - 80, 310, "HP:", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)UserData::player_hp), 310, text_color1, "%d", (int)UserData::player_hp);
		}
		if (result_anim_timer > (RESULT_ANIM_TIME / 5) * 3)
		{
			UserData::DrawCoin({ SCREEN_WIDTH / 2 - 75, 395 }, 20);
			DrawString(SCREEN_WIDTH / 2 - 35, 370, ":", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", UserData::coin), 370, text_color1, "%d", UserData::coin);
		}

		if (result_anim_timer > RESULT_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, 60);
		}
		break;
	case DispScene::dBonusPoint:
		if (UserData::is_clear)
		{
			bg_color = 0xffffbb;
			text_color1 = 0x888800;
			text_color2 = 0xaaaa00;
			text_color3 = 0xffbb00;
		}
		else
		{
			bg_color = 0x555500;
			text_color1 = 0x999900;
			text_color2 = 0x666600;
			text_color3 = 0x995500;
		}

		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_color, TRUE);
		SetFontSize(64);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "Bonus!!", bonus_anim_timer % 30 > 15 ? text_color1 : text_color3);
		SetFontSize(36);			

		DrawString(SCREEN_WIDTH / 2 - 120, 250, "Time:", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)), 250, text_color1, "%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));

		DrawString(SCREEN_WIDTH / 2 - 80, 310, "HP:", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)UserData::player_hp), 310, text_color1, "%d", (int)UserData::player_hp);

		UserData::DrawCoin({ SCREEN_WIDTH / 2 - 75, 395 }, 20);
		DrawString(SCREEN_WIDTH / 2 - 35, 370, ":", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? UserData::coin : add_anim_coin), 370, text_color1, "%d", bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? UserData::coin : add_anim_coin);

		//�{�[�i�X���Z�`��
		if (bonus_anim_timer > BONUS_ANIM_TIME / 5)
		{
			DrawFormatString(SCREEN_WIDTH / 2 + 130, 250, 0xffbb00, "=    �~%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 175, 270 }, 20);
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5)*2)
		{
			DrawFormatString(SCREEN_WIDTH / 2 + 130, 310, 0xffbb00, "=    �~%d", (int)UserData::player_hp * 10);
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 175, 330 }, 20);
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5) * 3)
		{
			SetFontSize(48);
			DrawFormatString(SCREEN_WIDTH / 2 + 130, 365, bonus_anim_timer % 30 > 15 ? 0xffbb00 : 0xff0000, "+    �~%d", (int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 200, 390 }, 30);
		}
		if (bonus_anim_timer > BONUS_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, 60);
		}
		break;
	case DispScene::dEnterName:
		EnterNameDraw();
		break;
	default:
		break;
	}

}

void ResultScene::Finalize()
{

}

eSceneType ResultScene::GetNowSceneType()const
{
	return eSceneType::eResult;
}

eSceneType ResultScene::EnterName()
{
	//�\���L�[�����X�e�B�b�N�ō��ڂ̈ړ�
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) || InputPad::GetPressedButton(L_STICK_LEFT))
	{
		//�󔒂��I������Ȃ��Ȃ�܂ŌJ��Ԃ�(�󔒂��΂�)
		do{
			//�ړ�����
			if (--current_x < 0)
			{
				current_x = KEY_WIDTH - 1;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::GetPressedButton(L_STICK_RIGHT))
	{
		//�󔒂��I������Ȃ��Ȃ�܂ŌJ��Ԃ�(�󔒂��΂�)
		do {
			//�ړ�����
			if (++current_x > KEY_WIDTH - 1)
			{
				current_x = 0;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP))
	{
		//�󔒂��I������Ȃ��Ȃ�܂ŌJ��Ԃ�(�󔒂��΂�)
		do {
			//�ړ�����
			if (--current_y < 0)
			{
				current_y = KEY_HEIGHT - 1;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN))
	{
		//�󔒂��I������Ȃ��Ȃ�܂ŌJ��Ԃ�(�󔒂��΂�)
		do {
			//�ړ����
			if (++current_y > KEY_HEIGHT - 1)
			{
				current_y = 0;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}

	//A�{�^���������ĕ����̒ǉ�(���݂̓��͂Ɖ�ʓ��̕����̍��v��10���������Ȃ�)
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
		CreateMoveString(key[current_y][current_x], { key_box_loc.x + (current_x * 40),key_box_loc.y + (current_y * 40) }, { name_string_loc.x + 150,name_string_loc.y + 25 }, { 400,50 }, true);
	}

	//B�{�^���������Ĉꕶ������(���݂̓��͂�0�������傫���Ȃ�)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_B))
	{
		//���݂̃e�L�X�g�����v�Z
		int size = 0;
		char t;
		for (auto text : name)
		{
			size += GetDrawFormatStringWidth("%c", text);
			t = text;
		}
		CreateMoveString(t, { name_string_loc.x + 80 + size,name_string_loc.y }, { SCREEN_WIDTH/2,SCREEN_HEIGHT }, {SCREEN_WIDTH,100},false, { (float)GetRand(10) - 5,-5 });
		name.erase(name.end()-1);
		PlaySoundMem(erase_se, DX_PLAYTYPE_BACK);
	}

	//START�{�^���œ��͏I��(�ꕶ���ȏ���͂���Ă���Ȃ�)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_START))
	{
		UserData::my_ranking_data.name = name.c_str();
		UserData::my_ranking_data.coin = UserData::coin;
		UserData::ranking_data[9] = UserData::my_ranking_data;
		SortRanking();
		UserData::WriteRankingData();
		PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
		return eSceneType::eRanking;
	}


	for (auto& string_data : string_data)
	{
		string_data = MoveString(string_data);

		//�ړI�n�ɂ��ǂ蒅���������&���O����
		if (string_data.velocity.y > 0.f &&
			string_data.goal_location.x - string_data.goal_size.x/2 <= string_data.location.x &&
			string_data.goal_location.x + string_data.goal_size.x / 2 >= string_data.location.x &&
			string_data.goal_location.y - string_data.goal_size.y / 2 <= string_data.location.y &&
			string_data.goal_location.y + string_data.goal_size.y / 2 >= string_data.location.y
			)
		{
			if(name.size() < 10)delete_string_data.push_back(string_data);
			if(string_data.is_add && name.size() < 10)name.push_back(string_data.text);
		}
		//��ʊO�ɏo�Ă�����
		if (string_data.location.x <-50 ||
			string_data.location.x > SCREEN_WIDTH + 50 ||
			string_data.location.y > SCREEN_HEIGHT + 50)
		{
			delete_string_data.push_back(string_data);
		}
	}

	//�I�u�W�F�N�g�z�񂩂�폜���鏈��
	for (const auto& delete_data : delete_string_data)
	{
		for (auto it = string_data.begin(); it != string_data.end();)
		{
			if (*it == delete_data)
			{
				it = string_data.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	delete_string_data.clear();


	if (++frame > 3600)
	{
		frame = 0;
	}
	return GetNowSceneType();
}

void ResultScene::EnterNameDraw()const 
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	SetFontSize(24);

	DrawString(SCREEN_WIDTH / 2+1, 401, ":����",  0x000000);
	DrawString(SCREEN_WIDTH / 2,   400, ":����",  0xffff00);
	DrawString(SCREEN_WIDTH / 2+1, 441, ":�ꎚ����",0x000000);
	DrawString(SCREEN_WIDTH / 2, 440, ":�ꎚ����",0xffff00);
	DrawString(SCREEN_WIDTH / 2+1, 481, ":�m��",    0x000000);
	DrawString(SCREEN_WIDTH / 2, 480, ":�m��",    0xffff00);

	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 420 }, XINPUT_BUTTON_A, 50);
	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 460 }, XINPUT_BUTTON_B, 50);
	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 480 }, XINPUT_BUTTON_START, 50);

	SetFontSize(32);
	//�����̕`��
	for (int y = 0; y < KEY_HEIGHT; y++)
	{
		for (int x = 0; x < KEY_WIDTH; x++)
		{
			//�I������Ă��鍀�ڂ̐F��ς���
			if (current_x == x && current_y == y)
			{
				DrawFormatString(key_box_loc.x+1 + x * 40, key_box_loc.y+1 + y * 40, 0x000000, "%c", key[y][x]);
				DrawFormatString(key_box_loc.x + x * 40, key_box_loc.y + y * 40, 0xff9900, "%c", key[y][x]);
			}
			else
			{
				DrawFormatString(key_box_loc.x+1 + x * 40, key_box_loc.y + 1 + y * 40, 0x000000, "%c", key[y][x]);
				DrawFormatString(key_box_loc.x + x * 40, key_box_loc.y + y * 40, 0xffff88, "%c", key[y][x]);
			}
		}
	}
	//���݂̓���
	DrawFormatString(name_string_loc.x+1, name_string_loc.y+1, 0x000000, "name:%s", name.c_str());
	DrawFormatString(name_string_loc.x, name_string_loc.y, 0xffff00, "name:%s", name.c_str());
	if ((int)frame % 30 > 15)DrawLine(name_string_loc.x + 100, name_string_loc.y + GetFontSize(), name_string_loc.x + 400, name_string_loc.y + GetFontSize(), 0xffff00);

	SetFontSize(32);
	//ui_data�̕`��
	for (const auto string_data : string_data)
	{
		DrawFormatStringF(string_data.location.x+1,
			string_data.location.y+1,
			0x000000,
			"%c",
			string_data.text
		);
		DrawFormatStringF(string_data.location.x,
			string_data.location.y,
			0xff9900,
			"%c",
			string_data.text
		);
	}
}

void ResultScene::SortRanking()
{
	RankingData work;	//���ёւ��p�̕ϐ�

	// �I��@�\�[�g
	for (int i = 0; i < RANKING_DATA - 1; i++) {
		for (int j = i + 1; j < RANKING_DATA; j++) {
			if (UserData::ranking_data[i].coin <= UserData::ranking_data[j].coin) {
				work = UserData::ranking_data[i];
				UserData::ranking_data[i] = UserData::ranking_data[j];
				UserData::ranking_data[j] = work;
			}
		}
	}

	// ���ʕt��
	for (int i = 0; i < RANKING_DATA; i++) {
		UserData::ranking_data[i].num = 1;
	}

	// ���_�������ꍇ�́A�������ʂƂ���
	// �����ʂ��������ꍇ�̎��̏��ʂ̓f�[�^�������Z���ꂽ���ʂƂ���
	for (int i = 0; i < RANKING_DATA - 1; i++) {
		for (int j = i + 1; j < RANKING_DATA; j++) {
			if (UserData::ranking_data[i].coin > UserData::ranking_data[j].coin) {
				UserData::ranking_data[j].num++;
			}
		}
	}
	for (int i = 0; i < RANKING_DATA; i++)
	{
		//���g�̏��ʕۑ�
		if (UserData::my_ranking_data.name == UserData::ranking_data[i].name &&
			UserData::my_ranking_data.coin == UserData::ranking_data[i].coin)
		{
			UserData::my_ranking_data.num = i+1;
			break;
		}
	}
}

StringData ResultScene::MoveString(StringData _data)
{
	StringData ret = _data;
	//X���̌���
	if (ret.velocity.x > 0.f)ret.velocity.x -= DECELERATION;
	if (ret.velocity.x < 0.f)ret.velocity.x += DECELERATION;
	if (fabsf(ret.velocity.x) <= DECELERATION)ret.velocity.x = 0.f;

	//�d��
	ret.velocity.y += GRAVITY;
	ret.location += ret.velocity;

	return ret;
}

void ResultScene::CreateMoveString(const char _c, Vector2D _loc, Vector2D _goal_loc, Vector2D _goal_size, bool _is_add, Vector2D _velocity)
{
	StringData data;
	data.text = _c;
	data.location = _loc;
	data.goal_location = _goal_loc;
	data.goal_size = _goal_size;
	data.velocity = _velocity == 0.f ? Vector2D{(data.goal_location.x - data.location.x) / 20,(data.goal_location.y - data.location.y) / 10 -5 } : _velocity;
	data.speed = 1.f;
	data.angle = 0.f;
	data.is_add = _is_add;
	string_data.push_back(data);
}