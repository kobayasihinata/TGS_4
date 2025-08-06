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

	volume_cursor = -1;//どこも選択されていない状態
	end_cursor = 0;

	//BGM、SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	button_se = rm->GetSounds("Resource/Sounds/pop.wav");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	option_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	if (!CheckSoundMem(option_bgm))
	{
		ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_BACK, TRUE);
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
		EndUpdate();
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
		//右入力orAボタンで現在選択している項目に移動
		if (InputPad::OnButton(XINPUT_BUTTON_A) ||
			InputPad::OnButton(XINPUT_BUTTON_DPAD_RIGHT) ||
			InputPad::OnButton(L_STICK_RIGHT))
		{
			ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
			current_item = cursor;
		}
		//Bボタンでオプション終了画面
		if (InputPad::OnButton(XINPUT_BUTTON_B))
		{
			ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
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
	int size = 48;
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
	case OptionItem::oEnd:
		EndDraw();
		break;
	case OptionItem::oNull:
		for (int i = 0; i < OPTION_NUM; i++)
		{
			DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
			if (cursor == i)
			{
				UserData::DrawCoin({ 30.f, (float)124 + i * (SCREEN_HEIGHT / OPTION_NUM) }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
			}
		}
		break;
	}
}

void OptionScene::Finalize()
{

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
		//カーソルのリセット
		volume_cursor = -1;
		current_item = OptionItem::oNull;
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
			UserData::se_volume -= 100;
			break;
		case 1:
			UserData::bgm_volume -= 100;
			ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, false);
			break;
		}
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) ||
		InputPad::GetPressedButton(L_STICK_RIGHT))
	{
		switch (volume_cursor)
		{
		case 0:
			UserData::se_volume += 100;
			break;
		case 1:
			UserData::bgm_volume += 100;
			ResourceManager::rPlaySound(option_bgm, DX_PLAYTYPE_LOOP, false);
			break;
		}
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}
}

void OptionScene::VolumeDraw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	for (int i = 0; i < OPTION_NUM; i++)
	{
		DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawString(300, 100, "volume", 0x000000);

	if (volume_cursor != -1)
	{
		UserData::DrawCoin({ 300.f, (float)150 + (volume_cursor * 100) }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
	}
	DrawFormatString(500, 150, 0x000000, "SE:%d", UserData::se_volume);
	DrawFormatString(500, 250, 0x000000, "BGM:%d", UserData::bgm_volume);

}

void OptionScene::ControlUpdate()
{
	//Bボタンで項目の選択を解除
	if (InputPad::OnButton(XINPUT_BUTTON_B))
	{
		current_item = OptionItem::oNull;
	}
	//操作タイプの変更
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) ||
		InputPad::GetPressedButton(L_STICK_LEFT))
	{
		if (--UserData::control_type < 0)
		{
			UserData::control_type = 1;
		}
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) ||
		InputPad::GetPressedButton(L_STICK_RIGHT))
	{
		if (++UserData::control_type > 1)
		{
			UserData::control_type = 0;
		}
	}
}

void OptionScene::ControlDraw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	for (int i = 0; i < OPTION_NUM; i++)
	{
		DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawString(300, 100, "control", 0x000000);
	DrawFormatString(500, 150, 0x000000, "type:%d", UserData::control_type);
}

void OptionScene::EndUpdate()
{
	//左右入力で終了のカーソル移動
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_LEFT) ||
		InputPad::OnButton(L_STICK_LEFT))
	{
		if (--end_cursor < 0)
		{
			end_cursor = 1;
		}
	}
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_RIGHT) ||
		InputPad::OnButton(L_STICK_RIGHT))
	{
		if (++end_cursor > 1)
		{
			end_cursor = 0;
		}
	}
	//Bボタンで項目の選択を解除
	if (InputPad::OnButton(XINPUT_BUTTON_B))
	{
		current_item = OptionItem::oNull;
	}
	//Aボタンでカーソルが合っている場所ごとの処理
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		switch (end_cursor)
		{
		//項目の選択解除
		case 0:
			current_item = OptionItem::oNull;
			break;
		//オプション終了
		case 1:
			change_scene = eSceneType::eTitle;
			break;
		default:
			break;
		}
	}
}

void OptionScene::EndDraw()const
{
	for (int i = 0; i < OPTION_NUM; i++)
	{
		DrawFormatString(50, 100 + i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawString(300, 100, "タイトルに戻りますか？", 0x000000);
	DrawString(320, 150, "いいえ", 0x000000);
	DrawString(520, 150, "はい", 0x000000);
	UserData::DrawCoin({ (float)300 + (end_cursor*200), 175 }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
}

