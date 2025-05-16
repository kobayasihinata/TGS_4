#include "DxLib.h"
#include "GameSceneUI.h"
#include "Camera/Camera.h"
#include "../Utility/ResourceManager.h"
#include "../Object/Player/PlayerBullet.h"

void GameSceneUI::Initialize()
{
	frame = 0;
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
			DrawString(SCREEN_WIDTH / 2 - GetDrawFormatStringWidth("�R�C���s���I") / 2, 120, "�R�C���s���I", 0xff0000);
		}
	}

	//�e�̎�ޕ`��
	DrawBox(SCREEN_WIDTH / 2 - 100, 0, SCREEN_WIDTH / 2 + 100, 100, 0x000000, true);
	DrawBox(SCREEN_WIDTH / 2 - 100, 0, SCREEN_WIDTH / 2 + 100, 100, draw_color, false);

	SetFontSize(32);
	DrawFormatString(SCREEN_WIDTH / 2 - GetDrawFormatStringWidth(pBullet[UserData::bullet_type].name) / 2, 20, draw_color, "%s", pBullet[UserData::bullet_type].name);

	SetFontSize(16);
	DrawFormatString(SCREEN_WIDTH / 2 - 90, 80, draw_color, "����:%d��", pBullet[UserData::bullet_type].cost);
	DrawFormatString(SCREEN_WIDTH / 2, 80, draw_color, "�G�ђ�:%d��", pBullet[UserData::bullet_type].h_count);

	//�{�^������������摜��ς���
	
	DrawGraph(SCREEN_WIDTH / 2 - 150, 10, button_image[(int)InputPad::OnButton(L_TRIGGER)][L_TRIGGER], true);
	DrawGraph(SCREEN_WIDTH / 2 + 110, 10, button_image[(int)InputPad::OnButton(R_TRIGGER)][R_TRIGGER], true);
	//�����傫���ݒ�
	SetFontSize(24);

	//ui_data�̕`��
	for (const auto ui_data : ui_data)
	{
		//�������ߐݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 / ui_data.life_span)*ui_data.life_count);
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
