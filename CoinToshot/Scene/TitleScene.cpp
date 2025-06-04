#include "TitleScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/common.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"
#include <vector>

TitleScene::TitleScene()
{
	current_num = 0;
	bg_image = CreateBackGround();

	//SE“Ç‚İ‚İ
	ResourceManager* rm = ResourceManager::GetInstance();
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	enter_se = rm->GetSounds("Resource/Sounds/enter.mp3");
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
	//‰º“ü—Í‚Å€–Ú‰ºˆÚ“®
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::OnButton(L_STICK_DOWN))
	{
		if (++current_num >= ITEM_NUM)
		{
			current_num = 0;
		}
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	//ã“ü—Í‚Å€–ÚãˆÚ“®
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_UP) || InputPad::OnButton(L_STICK_UP))
	{
		if (--current_num < 0)
		{
			current_num = ITEM_NUM-1;
		}
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	//Aƒ{ƒ^ƒ“‚ÅŒˆ’è
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		PlaySoundMem(enter_se, DX_PLAYTYPE_BACK);
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

	//“ü—Í‹@”\‚Ìæ“¾
	InputKey* input = InputKey::Get();

	//1ƒL[‚ÅƒCƒ“ƒQ[ƒ€‰æ–Ê‚É‘JˆÚ‚·‚é
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eInGame;
	}

	//2ƒL[‚Å”wŒi¶¬‚µ’¼‚µ
	if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
	{
		bg_image = CreateBackGround();
	}
#endif // _DEBUG
	return __super::Update(_delta);
}

void TitleScene::Draw()const
{
	int old = GetFontSize();

	//”wŒi‰æ‘œ•`‰æ
	DrawGraph(0, 0, bg_image, TRUE);

	SetFontSize(96);
	// ƒtƒHƒ“ƒg‚ğ—˜—p‚µ‚Ä•¶š‚ğ•`‰æ
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2 +1,51 }, "CoinToshot", 0x000000);
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "CoinToshot", 0xffff00);

	int size = 48;
	SetFontSize(32);
	
	for (int i = 0; i < ITEM_NUM; i++)
	{
		//ƒJ[ƒ\ƒ‹‚Æ€–Ú‚Ì•`‰æ
		if (current_num == i)
		{
			UserData::DrawCoin({ (float)SCREEN_WIDTH / 2 - 30, (float)SCREEN_HEIGHT / 2 + (i * size) + 15 }, 15);
			//	DrawCircle(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + (i * 30)+15, 15, 0x00ff00, true);
			//€–Ú‚Ì•`‰æ
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * size), 0x00ff00, "%s", item_text[i]);
		}
		//€–Ú‚Ì‚İ‚Ì•`‰æ
		else
		{
			//€–Ú‚Ì•`‰æ
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * size), 0xffffff, "%s", item_text[i]);
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

int TitleScene::CreateBackGround()
{
	int ret = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);	//‰æ‘œ‰¼Ši”[

	//¶¬‚µ‚½”wŒi‚ğˆê‚Â‚Ì‰æ‘œ‚Æ‚µ‚Ä•Û‘¶A‚»‚êˆÈŠO‚Ìî•ñ‚Ííœ
	SetDrawScreen(ret);
	ClearDrawScreen();

	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xbbbb00, true);
	//for (int i = 0; i < 50; i++)
	//{
	//	DrawQuadrangle(GetRand(SCREEN_WIDTH * 2) - SCREEN_WIDTH / 2, GetRand(SCREEN_HEIGHT * 2) - SCREEN_HEIGHT / 2,
	//				   GetRand(SCREEN_WIDTH * 2) - SCREEN_WIDTH / 2, GetRand(SCREEN_HEIGHT * 2) - SCREEN_HEIGHT / 2,
	//				   GetRand(SCREEN_WIDTH * 2) - SCREEN_WIDTH / 2, GetRand(SCREEN_HEIGHT * 2) - SCREEN_HEIGHT / 2,
	//				   GetRand(SCREEN_WIDTH * 2) - SCREEN_WIDTH / 2, GetRand(SCREEN_HEIGHT * 2) - SCREEN_HEIGHT / 2,
	//				   GetColor(GetRand(55) + 200, GetRand(55) + 200, GetRand(100)),
	//				   true);
	//}
	SetDrawScreen(DX_SCREEN_BACK);

	return ret;
}