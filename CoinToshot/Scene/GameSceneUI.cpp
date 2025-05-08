#include "DxLib.h"
#include "GameSceneUI.h"
#include "Camera/Camera.h"

void GameSceneUI::Update()
{
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

	//�����傫���ݒ�
	SetFontSize(24);

	//ui_data�̕`��
	for (const auto ui_data : ui_data)
	{
		//�������ߐݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 / ui_data.life_span)*ui_data.life_count);
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
