#include "OptionScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

OptionScene::OptionScene()
{
	change_scene = GetNowSceneType();
	cursor = 0;
	current_item = oNull;

	volume_cursor = 0;//どこも選択されていない状態

	for (int i = 0; i < 10; i++)
	{
		//0以外は一定数加算
		if (i == 0)
		{
			se_volume[i] = 0;
			bgm_volume[i] = 0;
		}
		else
		{
			se_volume[i] = VOLUME + (((SE_MAX - VOLUME) / 8) * (i - 1));
			bgm_volume[i] = VOLUME + (((BGM_MAX - VOLUME) / 8) * (i - 1));
		}
	}
	end_cursor = 0;
	cont_type = UserData::control_type;
	check_change_flg = false;
	//BGM、SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	button_se = rm->GetSounds("Resource/Sounds/pop.wav");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	back_se = rm->GetSounds("Resource/Sounds/Hanahana/button.mp3");
	option_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	if (!CheckSoundMem(option_bgm))
	{
		ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, TRUE);
	}
}

OptionScene::~OptionScene()
{
	StopSoundMem(option_bgm);
}

void OptionScene::Initialize()
{
	__super::Initialize();
}

eSceneType OptionScene::Update(float _delta)
{
	change_scene = __super::Update(_delta);

	//選択された項目ごとの更新
	switch (current_item)
	{
	case OptionItem::oVolume:
		VolumeUpdate();
		break;
	case OptionItem::oControl:
		ControlUpdate();
		break;
	case OptionItem::oEnd:
		//ゲームメインからオプションへ移動していたのなら、ゲームメインへ
		if (UserData::old_scene != NULL)
		{
			change_scene = eSceneType::eInGame;
		}
		//そうでないならタイトルへ
		else
		{
			change_scene = eSceneType::eTitle;
		}
		break;
	case OptionItem::oNull:
		//下入力で項目下移動
		if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN))
		{
			if (++cursor >= OPTION_NUM)
			{
				cursor = 0;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		//上入力で項目上移動
		if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP))
		{
			if (--cursor < 0)
			{
				cursor = OPTION_NUM - 1;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		//Aボタンで現在選択している項目に移動
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
			current_item = cursor;
		}
		//Bボタンでオプション終了画面
		if (InputPad::OnButton(XINPUT_BUTTON_B))
		{
			ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
			current_item = OptionItem::oEnd;
		}
		break;
	}


#ifdef _DEBUG

	//入力機能の取得
	InputKey* input = InputKey::Get();

	//1キーでインゲーム画面に遷移する
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eInGame;
	}
#endif // _DEBUG
	return change_scene;
}

void OptionScene::Draw()const
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	UserData::DrawCoin({ SCREEN_WIDTH + 100, SCREEN_HEIGHT / 2 }, 900);
	int size = 64;
	SetFontSize(size);

	//選択された項目ごとの更新
	switch (current_item)
	{
	case OptionItem::oVolume:
		VolumeDraw();
		break;
	case OptionItem::oControl:
		ControlDraw();
		break;
	case OptionItem::oNull:
		for (int i = 0; i < OPTION_NUM; i++)
		{
			DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
			if (cursor == i)
			{
				UserData::DrawCoin({ 30.f, (float)135 + i * (SCREEN_HEIGHT / OPTION_NUM) }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
			}
		}
		SetFontSize(48);
		Vector2D _loc = { 700,900 };
		UserData::DrawButtonImage(_loc, L_STICK_UP, 72);
		UserData::DrawButtonImage({ _loc.x + 60,_loc.y }, L_STICK_DOWN, 72);
		UserData::DrawButtonImage({ _loc.x + 120,_loc.y }, XINPUT_BUTTON_DPAD_UP, 72);
		UserData::DrawButtonImage({ _loc.x + 180,_loc.y }, XINPUT_BUTTON_DPAD_DOWN, 72);
		DrawStringF(_loc.x + 210, _loc.y - 30, ":カーソル移動", 0x444400);
		UserData::DrawButtonAndString({ _loc.x + 170,  _loc.y + 70 }, XINPUT_BUTTON_A, ":決定", 0x444400);
		UserData::DrawButtonAndString({ _loc.x + 170,  _loc.y + 140 }, XINPUT_BUTTON_B, ":戻る", 0x444400);
		break;
	}
}

void OptionScene::Finalize()
{
	//設定保存
	UserData::WriteData();
}

eSceneType OptionScene::GetNowSceneType()const
{
	return eSceneType::eRanking;
}

void OptionScene::VolumeUpdate()
{
	//Bボタンで項目の選択を解除
	if (InputPad::OnButton(XINPUT_BUTTON_B))
	{
		current_item = OptionItem::oNull;
		ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
	}
	//下入力で項目下移動
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN))
	{
		if (++volume_cursor >= 2)
		{
			volume_cursor = 0;
		}
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}
	//上入力で項目上移動
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP))
	{
		if (--volume_cursor < 0)
		{
			volume_cursor = 1;
		}
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}

	//左右入力でカーソルを合わせた所の音量を調節
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) ||
		InputPad::GetPressedButton(L_STICK_LEFT))
	{
		switch (volume_cursor)
		{
		case 0:
			//下限に達したらそれ以下にはならない
			if (UserData::se_volume_num > 0)
			{
				UserData::se_volume_num--;
			}
			else
			{
				UserData::se_volume_num = 0;
			}
			UserData::se_volume = se_volume[UserData::se_volume_num];
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			break;
		case 1:
			//下限に達したらそれ以下にはならない
			if (UserData::bgm_volume_num > 0)
			{
				UserData::bgm_volume_num--;
			}
			else
			{
				UserData::bgm_volume_num = 0;
			}
			UserData::bgm_volume = bgm_volume[UserData::bgm_volume_num];
			ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, false);
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			break;
		}
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) ||
		InputPad::GetPressedButton(L_STICK_RIGHT))
	{
		switch (volume_cursor)
		{
		case 0:
			//上限に達したらそれ以上にはならない
			if (UserData::se_volume_num < 9)
			{
				UserData::se_volume_num++;
			}
			else
			{
				UserData::se_volume_num = 9;
			}
			UserData::se_volume = se_volume[UserData::se_volume_num];
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			break;
		case 1:
			//上限に達したらそれ以上にはならない
			if (UserData::bgm_volume_num < 9)
			{
				UserData::bgm_volume_num++;
			}
			else
			{
				UserData::bgm_volume_num = 9;
			}
			UserData::bgm_volume = bgm_volume[UserData::bgm_volume_num];
			ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, false);
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			break;
		}
	}
}

void OptionScene::VolumeDraw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	for (int i = 0; i < OPTION_NUM; i++)
	{
		DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


	Vector2D se_loc = { 700,150 };
	Vector2D bgm_loc = { 700,550 };
	int span = 40;	//ゲージの間隔
	DrawString(se_loc.x + (span*9/2) - (GetDrawStringWidth("SE",strlen("SE"))/2) +1, se_loc.y - 69, "SE", 0xffffff);
	DrawString(se_loc.x + (span*9/2) - (GetDrawStringWidth("SE",strlen("SE"))/2), se_loc.y - 70, "SE", 0x000000);
	DrawBox(se_loc.x, se_loc.y, se_loc.x + span*9, se_loc.y + 50, 0x555500, TRUE);
	DrawBox(se_loc.x, se_loc.y, se_loc.x + UserData::se_volume_num * span, se_loc.y + 50, 0xffff00, TRUE);
	DrawString(bgm_loc.x + (span * 9 / 2) - (GetDrawStringWidth("BGM", strlen("BGM")) / 2)+1, bgm_loc.y - 69, "BGM", 0xffffff);
	DrawString(bgm_loc.x + (span * 9 / 2) - (GetDrawStringWidth("BGM", strlen("BGM")) / 2), bgm_loc.y - 70, "BGM", 0x000000);
	DrawBox(bgm_loc.x, bgm_loc.y, bgm_loc.x + span * 9, bgm_loc.y + 50, 0x555500, TRUE);
	DrawBox(bgm_loc.x, bgm_loc.y, bgm_loc.x + UserData::bgm_volume_num * span, bgm_loc.y + 50, 0xffff00, TRUE);

	if (volume_cursor != -1)
	{
		UserData::DrawCoin({ se_loc.x-30, (float)se_loc.y + (volume_cursor * (bgm_loc.y - se_loc.y))+30 }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
	}

	SetFontSize(48);

	Vector2D _loc = { 700,900 };
	UserData::DrawButtonImage(_loc,					   L_STICK_UP,				72);
	UserData::DrawButtonImage({ _loc.x + 60,_loc.y },  L_STICK_DOWN,			72);
	UserData::DrawButtonImage({ _loc.x + 120,_loc.y }, XINPUT_BUTTON_DPAD_UP,	72);
	UserData::DrawButtonImage({ _loc.x + 180,_loc.y }, XINPUT_BUTTON_DPAD_DOWN, 72);
	DrawStringF(_loc.x + 210, _loc.y - 30, ":カーソル移動", 0x444400);
	UserData::DrawButtonImage({ _loc.x,_loc.y + 70 },     L_STICK_RIGHT,			   72);
	UserData::DrawButtonImage({ _loc.x + 60,_loc.y+70 },  L_STICK_LEFT,			   72);
	UserData::DrawButtonImage({ _loc.x + 120,_loc.y+70 }, XINPUT_BUTTON_DPAD_RIGHT,   72);
	UserData::DrawButtonImage({ _loc.x + 180,_loc.y+70 }, XINPUT_BUTTON_DPAD_LEFT, 72);
	DrawStringF(_loc.x + 210, _loc.y + 40, ":音量調整", 0x444400);
	UserData::DrawButtonAndString({ _loc.x+170,  _loc.y + 140 }, XINPUT_BUTTON_B, ":戻る", 0x444400);
}

void OptionScene::ControlUpdate()
{
	//Bボタンで項目の選択を解除
	if (InputPad::OnButton(XINPUT_BUTTON_B))
	{
		//操作タイプの変更があるなら
		if (UserData::control_type != cont_type)
		{
			check_change_flg = true;
		}
		else
		{
			current_item = OptionItem::oNull;
			ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
		}
	}
	//変更確認画面
	if (check_change_flg)
	{
		//Bボタンで項目の選択を解除
		//if (InputPad::OnButton(XINPUT_BUTTON_B))
		//{
		//	check_change_flg = false;
		//}
		//Aボタンで操作タイプ変更&設定画面終了
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			UserData::control_type = cont_type;
			current_item = OptionItem::oNull;
			ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		//操作タイプの変更
		if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) ||
			InputPad::GetPressedButton(L_STICK_LEFT))
		{
			if (--cont_type < 0)
			{
				cont_type = 2;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) ||
			InputPad::GetPressedButton(L_STICK_RIGHT))
		{
			if (++cont_type > 2)
			{
				cont_type = 0;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
	}
}

void OptionScene::ControlDraw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	for (int i = 0; i < OPTION_NUM; i++)
	{
		DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	Vector2D cont_text_loc = { 700,300 };
	DrawFormatString(cont_text_loc.x, cont_text_loc.y - 120, 0x000000, "操作タイプ : %d",cont_type);
	DrawTriangleAA(cont_text_loc.x - 70, cont_text_loc.y - 80,
		cont_text_loc.x - 30, cont_text_loc.y - 100,
		cont_text_loc.x - 30, cont_text_loc.y - 60,
		(InputPad::OnPressed(XINPUT_BUTTON_DPAD_LEFT) || InputPad::OnPressed(L_STICK_LEFT)) ? 0xffffff : 0x000000, true);
	DrawTriangleAA(cont_text_loc.x + 420, cont_text_loc.y - 80,
		cont_text_loc.x +380, cont_text_loc.y - 100,
		cont_text_loc.x +380, cont_text_loc.y - 60,
		(InputPad::OnPressed(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::OnPressed(L_STICK_RIGHT)) ? 0xffffff : 0x000000, true);

	switch (cont_type)
	{
		//LBRB発射 トリガー変更
	case 0:
		UserData::DrawButtonImage({ cont_text_loc.x,cont_text_loc.y }, XINPUT_BUTTON_LEFT_SHOULDER, 72);
		UserData::DrawButtonImage({ cont_text_loc.x + 60,cont_text_loc.y }, XINPUT_BUTTON_RIGHT_SHOULDER, 72);
		DrawStringF(cont_text_loc.x + 90, cont_text_loc.y - 40, ":弾発射", 0x000000);
		UserData::DrawButtonImage({ cont_text_loc.x,cont_text_loc.y + 70 }, L_TRIGGER, 72);
		UserData::DrawButtonImage({ cont_text_loc.x + 60,cont_text_loc.y+70 }, R_TRIGGER, 72);
		DrawStringF(cont_text_loc.x + 90, cont_text_loc.y +40, ":弾の種類変更", 0x000000);
		break;
		//B発射 LBRB変更
	case 1:
		UserData::DrawButtonImage({ cont_text_loc.x,cont_text_loc.y }, XINPUT_BUTTON_B, 72);
		DrawStringF(cont_text_loc.x + 30, cont_text_loc.y - 40, ":弾発射", 0x000000);
		UserData::DrawButtonImage({ cont_text_loc.x,cont_text_loc.y + 70 }, XINPUT_BUTTON_LEFT_SHOULDER, 72);
		UserData::DrawButtonImage({ cont_text_loc.x + 60,cont_text_loc.y + 70 }, XINPUT_BUTTON_RIGHT_SHOULDER, 72);
		DrawStringF(cont_text_loc.x + 90, cont_text_loc.y + 40, ":弾の種類変更", 0x000000);
		break;
		//B発射 LBRB変更
	case 2:
		DrawStringF(cont_text_loc.x + 30, cont_text_loc.y - 40, "キーマウ", 0x000000);
		break;
	default:
		break;
	}

	SetFontSize(48);
	Vector2D _loc = { 700,900 };
	UserData::DrawButtonImage({ _loc.x,_loc.y}, L_STICK_RIGHT, 72);
	UserData::DrawButtonImage({ _loc.x + 60,_loc.y}, L_STICK_LEFT, 72);
	UserData::DrawButtonImage({ _loc.x + 120,_loc.y}, XINPUT_BUTTON_DPAD_RIGHT, 72);
	UserData::DrawButtonImage({ _loc.x + 180,_loc.y}, XINPUT_BUTTON_DPAD_LEFT, 72);
	DrawStringF(_loc.x + 210, _loc.y - 30, ":操作タイプ変更", 0x444400);
	UserData::DrawButtonAndString({ _loc.x + 170,  _loc.y + 70 }, XINPUT_BUTTON_B, ":戻る", 0x444400);

	//操作タイプ変更確認画面
	if (check_change_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		UserData::DrawStringCenter({ SCREEN_WIDTH,SCREEN_HEIGHT }, "確認画面", 0xffffff);
	}
}

