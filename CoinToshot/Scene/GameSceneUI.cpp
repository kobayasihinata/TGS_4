#include "DxLib.h"
#include "GameSceneUI.h"

void GameSceneUI::Draw()const
{

	//�v���C���[HP�`��
	DrawFormatString(0, 10, 0xffffff, "HP:%d LIFE:%d SCORE:%d TIME:%d", (int)(UserData::player_hp), UserData::life, UserData::score, UserData::timer / 60);
}
