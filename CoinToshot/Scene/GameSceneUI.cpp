#include "DxLib.h"
#include "GameSceneUI.h"

void GameSceneUI::Update()
{
	for (auto ui_data : ui_data)
	{
		//時間を過ぎたら消す
		if (++ui_data.life_count <= ui_data.life_span)
		{
			delete_ui_data.push_back(ui_data);
		}
		//移動量を座標に加算
		ui_data.local_location.y -= (ui_data.life_count * ui_data.move_speed);
	
	}
}

void GameSceneUI::Draw()const
{
	//プレイヤーHP描画
	DrawFormatString(100, 10, 0xffffff, "HP:%d LIFE:%d SCORE:%d TIME:%d", (int)(UserData::player_hp), UserData::life, UserData::coin, UserData::timer / 60);

	for (const auto ui_data : ui_data)
	{
		DrawFormatStringF(ui_data.local_location.x,
			ui_data.local_location.y - (ui_data.life_count * ui_data.move_speed),
			ui_data.text_color,
			"%s",
			ui_data.text
			);
	}
}

void GameSceneUI::SetUIData(Vector2D _location, const char* _text,int _text_color, float _move, int _life_span)
{

}
