#include "TitleScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/common.h"

TitleScene::TitleScene()
{
	current_num = 0;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	__super::Initialize();

}

eSceneType TitleScene::Update(float _delta)
{
	//下入力で項目下移動
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::OnButton(L_STICK_DOWN))
	{
		if (++current_num >= ITEM_NUM)
		{
			current_num = 0;
		}
	}
	//上入力で項目上移動
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_UP) || InputPad::OnButton(L_STICK_UP))
	{
		if (--current_num < 0)
		{
			current_num = ITEM_NUM-1;
		}
	}
	//Aボタンで決定
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		switch (current_num)
		{
		case TitleItem::tGameMain:
			return eSceneType::eInGame;
			break;
		case TitleItem::tRanking:
			return eSceneType::eRanking;
			break;
		case TitleItem::tEnd:
			return eSceneType::eEnd;
			break;
		default:
			break;
		}
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

void TitleScene::Draw()const
{
	DrawString(10, 10, "Title", GetColor(255, 255, 255));

	int old = GetFontSize();

	SetFontSize(32);

	for (int i = 0; i < ITEM_NUM; i++)
	{
		//カーソルと項目の描画
		if (current_num == i)
		{
			DrawCircle(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + (i * 30)+15, 15, 0x00ff00, true);
			//項目の描画
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * 30), 0x00ff00, "%s", item_text[i]);
		}
		//項目のみの描画
		else
		{
			//項目の描画
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * 30), 0xffffff, "%s", item_text[i]);
		}
	}

	SetFontSize(old);
}

void TitleScene::Finalize()
{

}

eSceneType TitleScene::GetNowSceneType()const
{
	return eSceneType::eTitle;
}
