#include "DxLib.h"
#include "GameSceneUI.h"
#include "Camera/Camera.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/DebugInformation.h"
#include "../Object/Player/PlayerBullet.h"

void GameSceneUI::Initialize()
{
	frame = 0;
	ui_font = CreateFontToHandle("���_�S�V�b�N", 30, 3, DX_FONTTYPE_ANTIALIASING);
	bullet_image = MakeScreen(200, 150, TRUE);
	old_bullet_type = UserData::bullet_type;
	bullet_change_timer = 0;
	change_anim_move = 200.f / PLATER_BULLET_CHANGE_CD;

	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/UI/button1.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/UI/button2.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);
}

void GameSceneUI::Update()
{
	//�t���[���v��
	if (++frame >= 6000)
	{
		frame = 0;
	}

	for (auto& ui_data : ui_data)
	{
		//���Ԃ��߂��������
		if (++ui_data.life_count >= ui_data.life_span)
		{
			delete_ui_data.push_back(ui_data);
		}
	}

	//�I�u�W�F�N�g�z�񂩂�폜���鏈��
	for (const auto& delete_data : delete_ui_data)
	{
		for (auto it = ui_data.begin(); it != ui_data.end();)
		{
			if (*it == delete_data)
			{
				it = ui_data.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	//�ő�l�̍X�V
	if (UserData::attraction_timer > max_attraction)max_attraction = UserData::attraction_timer;
	//�Q�[�W�̃��Z�b�g
	if (UserData::attraction_timer <= 0)max_attraction = 0;

	//�e�̎�ނ��ς���Ă���Ȃ�A�A�j���[�V��������
	if (old_bullet_type != UserData::bullet_type)
	{
		//�A�j���[�V�����I������
		if (++bullet_change_timer >= PLATER_BULLET_CHANGE_CD)
		{
			old_bullet_type = UserData::bullet_type;
			bullet_change_timer = 0;
		}
	}
	CreateBulletTypeImage();

	//�폜�����I�u�W�F�N�g�͏���
	delete_ui_data.clear();
}

void GameSceneUI::Draw()const
{
	int old = GetFontSize();
	//�J�������W�擾
	Camera* camera = Camera::Get();

	//�v���C���[���`��
	DrawFormatString(100, 10, 0xffffff, "HP:%d COIN:%d TIME:%d", (int)(UserData::player_hp), UserData::coin, UserData::timer / 60);

	UserData::DrawCoin({ (float)SCREEN_WIDTH - GetDrawFormatStringWidthToHandle(ui_font, "�~%d", UserData::coin) - 30, 30
		}, 25);
	DrawFormatStringToHandle(SCREEN_WIDTH - GetDrawFormatStringWidthToHandle(ui_font,"�~%d",UserData::coin)-10, 10, 0x000000, ui_font, "�~%d", UserData::coin);

	//�R�C���z���񂹏����̕`��
	if (UserData::attraction_timer > 0)
	{
		DrawBox(camera->player_location.x - camera->GetCameraLocation().x - max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 20,
			camera->player_location.x - camera->GetCameraLocation().x + max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y- 10,
			0x000000, true);
		DrawBoxAA(camera->player_location.x - camera->GetCameraLocation().x - max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 20,
			camera->player_location.x - camera->GetCameraLocation().x + max_attraction / 20 - ((max_attraction - UserData::attraction_timer)/10),
			camera->player_location.y - camera->GetCameraLocation().y - 10,
			0x00ffff, true);
		DrawBox(camera->player_location.x - camera->GetCameraLocation().x - max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 20,
			camera->player_location.x - camera->GetCameraLocation().x + max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 10,
			0xffffff, false);
	}

	//�{�^������������摜��ς���
	DrawGraph(SCREEN_WIDTH / 2 - 150, 10, button_image[(int)InputPad::OnPressed(L_TRIGGER)][L_TRIGGER], true);
	DrawGraph(SCREEN_WIDTH / 2 + 110, 10, button_image[(int)InputPad::OnPressed(R_TRIGGER)][R_TRIGGER], true);

	//�v���C���[���e���UI�Ɣ�����瓧�߂���
	if (camera->player_location.x - camera->GetCameraLocation().x > (SCREEN_WIDTH / 2) - 100 &&
		camera->player_location.x - camera->GetCameraLocation().x < (SCREEN_WIDTH / 2) + 100 &&
		camera->player_location.y - camera->GetCameraLocation().y < 140)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	}
	//�e�̎�ޕ`��
	DrawRotaGraph(SCREEN_WIDTH / 2, 75, 1.0f, 0, bullet_image, TRUE);


	//ui_data�̕`��
	for (const auto ui_data : ui_data)
	{
		//�������ߐݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 / ui_data.life_span) * ui_data.life_count);
		DrawFormatStringF(ui_data.location.x - camera->GetCameraLocation().x + 1.f,
			ui_data.location.y - camera->GetCameraLocation().y - (ui_data.life_count * ui_data.move_speed) + 1.f,
			0x000000,
			"%s",
			ui_data.text.c_str()
		);
		DrawFormatStringF(ui_data.location.x - camera->GetCameraLocation().x,
			ui_data.location.y - camera->GetCameraLocation().y - (ui_data.life_count * ui_data.move_speed),
			ui_data.text_color,
			"%s",
			ui_data.text.c_str()
		);
	}

	//�������߃��Z�b�g
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetFontSize(old);
}

void GameSceneUI::SetUIData(Vector2D _location, string _text, int _font_size, int _text_color, float _move, int _life_span)
{
	UIData data;
	data.font_size = _font_size;
	data.life_count = 0;
	data.life_span = _life_span;
	data.location = _location;
	data.move_speed = _move;
	data.text = _text;
	data.text_color = _text_color;

	ui_data.push_back(data);
}

void GameSceneUI::CreateBulletTypeImage()
{
	SetDrawScreen(bullet_image);
	ClearDrawScreen();

	int draw_color;
	//���ĂȂ��e��UI�͔��Â�����
	if (pBullet[UserData::bullet_type].cost <= UserData::coin)
	{
		draw_color = 0xffffff;
	}
	else
	{
		draw_color = 0xaaaaaa;
		//���ĂȂ��|��`����
		if (frame % 30 >= 15)
		{
			DrawString(100 - GetDrawFormatStringWidth("�R�C���s���I") / 2, 120, "�R�C���s���I", 0xff0000);
		}
	}

	//�e�ύX���L��ΕύX�O�ƕύX��̔���`��
	if (old_bullet_type != UserData::bullet_type)
	{
		DrawBullet({ change_anim_move * bullet_change_timer-200, 0 }, UserData::bullet_type);
		DrawBullet({ change_anim_move * bullet_change_timer, 0 }, old_bullet_type);
	}
	//�ύX�������Ȃ�ʏ�̕`��
	else
	{
		DrawBullet({ 0, 0 }, UserData::bullet_type);
	}

	//�����傫���ݒ�
	SetFontSize(24);

	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::DrawBullet(Vector2D _loc, int _type)const
{
	//���ĂȂ��e��UI�͔��Â�����
	int draw_color = pBullet[UserData::bullet_type].cost <= UserData::coin ? 0xffffff: 0xaaaaaa;

	//�e�̎�ޕ`��
	DrawBox(_loc.x, _loc.y, _loc.x + 200, _loc.y + 100, 0x000000, true);
	DrawBox(_loc.x, _loc.y, _loc.x + 200, _loc.y + 100, draw_color, false);

	SetFontSize(32);
	DrawFormatString(_loc.x + 100 - GetDrawFormatStringWidth(pBullet[_type].name) / 2, _loc.y + 20, draw_color, "%s", pBullet[_type].name);

	SetFontSize(16);
	DrawFormatString(_loc.x + 10, _loc.y + 80, draw_color, "����:%d��", pBullet[_type].cost);
	DrawFormatString(_loc.x + 100, _loc.y + 80, draw_color, "�G�ђ�:%d��", pBullet[_type].h_count);
}