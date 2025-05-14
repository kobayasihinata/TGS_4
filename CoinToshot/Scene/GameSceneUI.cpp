#include "DxLib.h"
#include "GameSceneUI.h"
#include "Camera/Camera.h"
#include "../Utility/ResourceManager.h"
#include "../Object/Player/PlayerBullet.h"

void GameSceneUI::Initialize()
{
	frame = 0;
	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/UI/button1.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/UI/button2.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);
}

void GameSceneUI::Update()
{
	//フレーム計測
	if (++frame >= 6000)
	{
		frame = 0;
	}

	for (auto& ui_data : ui_data)
	{
		//時間を過ぎたら消す
		if (++ui_data.life_count >= ui_data.life_span)
		{
			delete_ui_data.push_back(ui_data);
		}
	}

	//オブジェクト配列から削除する処理
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

	//削除したオブジェクトは消去
	delete_ui_data.clear();
}

void GameSceneUI::Draw()const
{
	int old = GetFontSize();
	//カメラ座標取得
	Camera* camera = Camera::Get();

	//プレイヤー情報描画
	DrawFormatString(100, 10, 0xffffff, "HP:%d COIN:%d TIME:%d", (int)(UserData::player_hp), UserData::coin, UserData::timer / 60);

	int draw_color;
	//撃てない弾のUIは薄暗くする
	if (pBullet[UserData::bullet_type].cost <= UserData::coin)
	{
		draw_color = 0xffffff;
	}
	else
	{
		draw_color = 0xaaaaaa;
		//撃てない旨を伝える
		if (frame % 30 >= 15)
		{
			DrawString(SCREEN_WIDTH / 2 - GetDrawFormatStringWidth("コイン不足！") / 2, 120, "コイン不足！", 0xff0000);
		}
	}

	//弾の種類描画
	DrawBox(SCREEN_WIDTH / 2 - 100, 0, SCREEN_WIDTH / 2 + 100, 100, 0x000000, true);
	DrawBox(SCREEN_WIDTH / 2 - 100, 0, SCREEN_WIDTH / 2 + 100, 100, draw_color, false);

	SetFontSize(32);
	DrawFormatString(SCREEN_WIDTH / 2 - GetDrawFormatStringWidth(pBullet[UserData::bullet_type].name) / 2, 20, draw_color, "%s", pBullet[UserData::bullet_type].name);

	SetFontSize(16);
	DrawFormatString(SCREEN_WIDTH / 2 - 90, 80, draw_color, "消費:%d枚", pBullet[UserData::bullet_type].cost);
	DrawFormatString(SCREEN_WIDTH / 2, 80, draw_color, "敵貫通:%d体", pBullet[UserData::bullet_type].h_count);

	//ボタンを押したら画像を変える
	
	DrawGraph(SCREEN_WIDTH / 2 - 150, 10, button_image[(int)InputPad::OnButton(L_TRIGGER)][L_TRIGGER], true);
	DrawGraph(SCREEN_WIDTH / 2 + 110, 10, button_image[(int)InputPad::OnButton(R_TRIGGER)][R_TRIGGER], true);
	//文字大きさ設定
	SetFontSize(24);

	//ui_dataの描画
	for (const auto ui_data : ui_data)
	{
		//文字透過設定
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
	//文字透過リセット
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
