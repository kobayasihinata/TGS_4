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
	old_cursor = 0;
	cursor_loc = { 50, 100 };
	cursor_size_y = 70;
	cursor_span = (SCREEN_HEIGHT / OPTION_NUM);
	current_item = oNull;

	volume_cursor = 0;//どこも選択されていない状態
	volume_cursor_loc = { 700,150 };
	volume_cursor_size;
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
	cont_left_box_loc = { 650,220 };
	cont_right_box_loc = { 1100,220 };
	cont_box_size = { 50,50 };
	cont_type_timer = 0;
	cont_type_cursor = 0;
	old_cont_type = cont_type = UserData::control_type;
	check_change_loc = { SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 150 };
	check_change_size = { 100,50 };
	check_change_flg = false;
	//BGM、SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	button_se = rm->GetSounds("Resource/Sounds/pop.wav");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	back_se = rm->GetSounds("Resource/Sounds/Hanahana/button.mp3");
	option_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	if (!CheckSoundMem(option_bgm))
	{
		ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, 0,true);
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
		if (UserData::CheckCursorMove(DOWN))
		{
			if (++cursor >= OPTION_NUM)
			{
				cursor = 0;
			}
		}
		//上入力で項目上移動
		if (UserData::CheckCursorMove(UP))
		{
			if (--cursor < 0)
			{
				cursor = OPTION_NUM - 1;
			}
		}
		//マウスカーソルの位置に合わせて項目変更(キーマウ操作なら)
		if (UserData::control_type == 2)
		{
			for (int i = 0; i < OPTION_NUM; i++)
			{
				if (UserData::CheckCursor({ cursor_loc.x,cursor_loc.y + (i * cursor_span) }, { option_text_size[i],cursor_size_y}))
				{
					cursor = i;
				}
			}
		}
		//カーソルの位置が変わっていたらSE
		if (old_cursor != cursor)
		{
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			old_cursor = cursor;
		}
		//Aボタンで現在選択している項目に移動
		if (UserData::CheckEnter())
		{
			ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
			current_item = cursor;
			old_cont_type = UserData::control_type;
		}
		//Bボタンでオプション終了画面
		if (UserData::CheckCancel())
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
			DrawFormatString(cursor_loc.x, cursor_loc.y + (i * cursor_span), 0x000000, "%s", option_text[i]);
			//DrawBoxAA(cursor_loc.x, cursor_loc.y + (i * cursor_span), cursor_loc.x + option_text_size[i], cursor_loc.y + cursor_size_y + (i * cursor_span), 0xff0000, false);
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
	if (UserData::CheckCancel())
	{
		current_item = OptionItem::oNull;
		ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
	}
	//下入力で項目下移動
	if (UserData::CheckCursorMove(DOWN))
	{
		if (++volume_cursor >= 2)
		{
			volume_cursor = 0;
		}
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}
	//上入力で項目上移動
	if (UserData::CheckCursorMove(UP))
	{
		if (--volume_cursor < 0)
		{
			volume_cursor = 1;
		}
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}

	//左右入力でカーソルを合わせた所の音量を調節
	if (UserData::CheckCursorMove(LEFT))
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
			ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, 0,false);
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			break;
		}
	}
	if (UserData::CheckCursorMove(RIGHT))
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
			ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, 0,false);
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			break;
		}
	}

	//マウスカーソルで音量を変更する処理
	if (UserData::control_type == 2 && UserData::CheckEnter(eInputState::Held))
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				//音量調節
				if (UserData::CheckCursor({ volume_cursor_loc.x + (i * 40),volume_cursor_loc.y + (j * 400) }, { 40,50 }))
				{
					volume_cursor = j;
					switch (volume_cursor)
					{
					case 0:
						if (UserData::se_volume_num != i + 1)
						{
							UserData::se_volume_num = i + 1;
							UserData::se_volume = se_volume[UserData::se_volume_num];
							ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
						}
						break;
					case 1:
						if (UserData::bgm_volume_num != i + 1)
						{
							UserData::bgm_volume_num = i + 1;
							UserData::bgm_volume = bgm_volume[UserData::bgm_volume_num];
							ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, 0, false);
							ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
						}
						break;
					default:
						break;
					}
				}
			}
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

	
	int span = 40;	//ゲージの間隔
	DrawString(volume_cursor_loc.x + (span*9/2) - (GetDrawStringWidth("SE",strlen("SE"))/2) +1, volume_cursor_loc.y - 69, "SE", 0xffffff);
	DrawString(volume_cursor_loc.x + (span*9/2) - (GetDrawStringWidth("SE",strlen("SE"))/2), volume_cursor_loc.y - 70, "SE", 0x000000);
	DrawBox(volume_cursor_loc.x, volume_cursor_loc.y, volume_cursor_loc.x + span*9, volume_cursor_loc.y + 50, 0x555500, TRUE);
	DrawBox(volume_cursor_loc.x, volume_cursor_loc.y, volume_cursor_loc.x + UserData::se_volume_num * span, volume_cursor_loc.y + 50, 0xffff00, TRUE);
	DrawString(volume_cursor_loc.x + (span * 9 / 2) - (GetDrawStringWidth("BGM", strlen("BGM")) / 2)+1, volume_cursor_loc.y - 69 + 400, "BGM", 0xffffff);
	DrawString(volume_cursor_loc.x + (span * 9 / 2) - (GetDrawStringWidth("BGM", strlen("BGM")) / 2), volume_cursor_loc.y - 70 + 400, "BGM", 0x000000);
	DrawBox(volume_cursor_loc.x, volume_cursor_loc.y + 400, volume_cursor_loc.x + span * 9, volume_cursor_loc.y + 50 + 400, 0x555500, TRUE);
	DrawBox(volume_cursor_loc.x, volume_cursor_loc.y + 400, volume_cursor_loc.x + UserData::bgm_volume_num * span, volume_cursor_loc.y + 50 + 400, 0xffff00, TRUE);

	//for (int i = 0; i < 9; i++)
	//{
	//	for (int j = 0; j < 2; j++)
	//	{
	//		DrawBox(volume_cursor_loc.x + (i * 40),
	//			volume_cursor_loc.y + (j * 400),
	//			volume_cursor_loc.x + (i * 40)+40,
	//			volume_cursor_loc.y + (j * 400)+50, 0xff0000, false);
	//	}
	//}
	if (volume_cursor != -1)
	{
		UserData::DrawCoin({ volume_cursor_loc.x-30, (float)volume_cursor_loc.y + (volume_cursor * 400)+30 }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
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
	if (UserData::CheckCancel() || (UserData::control_type != 2 && UserData::CheckEnter()))
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
	if (check_change_flg && ++cont_type_timer > 5)
	{
		//左右入力で終了のカーソル移動
		if (UserData::CheckCursorMove(LEFT))
		{
			if (--cont_type_cursor < 0)
			{
				cont_type_cursor = 1;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		if (UserData::CheckCursorMove(RIGHT))
		{
			if (++cont_type_cursor > 1)
			{
				cont_type_cursor = 0;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		
		//マウス処理
		if (UserData::control_type == 2)
		{
			for (int i = 0; i < 2; i++)
			{
				if (cont_type_cursor != i && UserData::CheckCursor({ check_change_loc.x+(i*300),check_change_loc.y },check_change_size))
				{
					cont_type_cursor = i;
					ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
				}

			}

		}
		//キャンセルボタンで項目の選択を解除
		if (UserData::CheckCancel())
		{
			check_change_flg = false;
			cont_type_timer = 0;
			ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
		}
		//決定ボタンでカーソルが合っている場所ごとの処理
		if (UserData::CheckEnter())
		{
			switch (cont_type_cursor)
			{
				//カーソル左で操作タイプを元に戻す&設定画面終了
			case 0:
				UserData::control_type = old_cont_type;
				cont_type = old_cont_type;
				current_item = OptionItem::oNull;
				check_change_flg = false;
				cont_type_timer = 0;
				ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
				break;
				//カーソル右で操作タイプ変更&設定画面終了
			case 1:
				UserData::control_type = cont_type;
				current_item = OptionItem::oNull;
				check_change_flg = false;
				cont_type_timer = 0;
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		//操作タイプの変更
		if (UserData::CheckCursorMove(LEFT))
		{
			if (--cont_type < 0)
			{
				cont_type = 2;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}
		if (UserData::CheckCursorMove(RIGHT))
		{
			if (++cont_type > 2)
			{
				cont_type = 0;
			}
			ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
		}

		//マウス処理
		if (UserData::control_type == 2)
		{
			if (UserData::CheckEnter())
			{
				if (UserData::CheckCursor(cont_left_box_loc - (cont_box_size / 2), cont_box_size))
				{
					if (--cont_type < 0)
					{
						cont_type = 2;
					}
					ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
				}
				if (UserData::CheckCursor(cont_right_box_loc - (cont_box_size / 2), cont_box_size))
				{
					if (++cont_type > 2)
					{
						cont_type = 0;
					}
					ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
				}
			}
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
	DrawTriangleAA(cont_left_box_loc.x - 20, cont_left_box_loc.y,
		cont_left_box_loc.x + 20, cont_left_box_loc.y - 20,
		cont_left_box_loc.x + 20, cont_left_box_loc.y + 20,
		UserData::CheckCursorMove(LEFT) ? 0xffffff : 0x000000, true);
	DrawTriangleAA(cont_right_box_loc.x + 20, cont_right_box_loc.y,
		cont_right_box_loc.x - 20, cont_right_box_loc.y - 20,
		cont_right_box_loc.x - 20, cont_right_box_loc.y + 20,
		UserData::CheckCursorMove(RIGHT) ? 0xffffff : 0x000000, true);

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
		DrawString(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 100, "操作方法を変更しますか？", 0xffffff);
		DrawString(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 150, "いいえ", cont_type_cursor == 0 ? 0xffff00 : 0xffffff);
		DrawBox(check_change_loc.x, check_change_loc.y, check_change_loc.x + check_change_size.x, check_change_loc.y + check_change_size.y, 0xff0000, false);
		DrawBox(check_change_loc.x+300, check_change_loc.y, check_change_loc.x+300 + check_change_size.x, check_change_loc.y + check_change_size.y, 0xff0000, false);
		DrawString(SCREEN_WIDTH / 2 + 120, SCREEN_HEIGHT / 2 + 150, "はい", cont_type_cursor == 1 ? 0xffff00 : 0xffffff);
		UserData::DrawCoin({ (float)SCREEN_WIDTH / 2 - 200 + (cont_type_cursor * 300), SCREEN_HEIGHT / 2 + 185 }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
	}
}

