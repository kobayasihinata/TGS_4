#include "DxLib.h"
#include "GameSceneUI.h"

void GameSceneUI::Draw()const
{

	//ÉvÉåÉCÉÑÅ[HPï`âÊ
	DrawFormatString(0, 10, 0xffffff, "HP:%d LIFE:%d SCORE:%d TIME:%d", (int)(UserData::player_hp), UserData::life, UserData::score, UserData::timer / 60);
}
