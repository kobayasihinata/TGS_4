#include "OptionScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

OptionScene::OptionScene()
{
	cursor = 0;

	//BGM、SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	button_se = rm->GetSounds("Resource/Sounds/pop.wav");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	option_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	SetVolumeSoundMem(8000, option_bgm);
	if (!CheckSoundMem(option_bgm))
	{
		PlaySoundMem(option_bgm, DX_PLAYTYPE_BACK, TRUE);
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
	//下入力で項目下移動
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN))
	{
		if (++cursor >= OPTION_NUM)
		{
			cursor = 0;
		}
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	//上入力で項目上移動
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP))
	{
		if (--cursor < 0)
		{
			cursor = OPTION_NUM - 1;
		}
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}

	//Bボタンでタイトルに戻る
	if (InputPad::OnButton(XINPUT_BUTTON_B))
	{
		PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
		return eSceneType::eTitle;
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
	return __super::Update(_delta);
}

void OptionScene::Draw()const
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	UserData::DrawCoin({ SCREEN_WIDTH + 100, SCREEN_HEIGHT / 2 }, 900);
	int size = 48;
	SetFontSize(size);

	UserData::DrawButtonAndString({ SCREEN_WIDTH / 2 - 25,  SCREEN_HEIGHT - 64 }, XINPUT_BUTTON_B, ":タイトルに戻る", 0xffff00);

	for (int i = 0; i < OPTION_NUM; i++)
	{
		DrawFormatString(50, 100+ i * (SCREEN_HEIGHT / OPTION_NUM), 0x000000, "%s", option_text[i]);
		if (cursor == i)
		{
			UserData::DrawCoin({ 30.f, (float)124 + i * (SCREEN_HEIGHT / OPTION_NUM) }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
		}
	}
}

void OptionScene::Finalize()
{

}

eSceneType OptionScene::GetNowSceneType()const
{
	return eSceneType::eRanking;
}



