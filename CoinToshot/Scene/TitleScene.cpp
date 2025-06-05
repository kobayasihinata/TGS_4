#include "TitleScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/common.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/DebugInformation.h"

#include <vector>

TitleScene::TitleScene()
{
	start_anim_flg = true;
	start_anim_timer = 0;

	current_num = 0;
	bg_image = CreateBackGround();

	//SE読み込み
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
	
	//アニメーション中なら操作不可
	if (!start_anim_flg)
	{
		//下入力で項目下移動
		if (InputPad::OnButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::OnButton(L_STICK_DOWN))
		{
			if (++current_num >= ITEM_NUM)
			{
				current_num = 0;
			}
			PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
		}
		//上入力で項目上移動
		if (InputPad::OnButton(XINPUT_BUTTON_DPAD_UP) || InputPad::OnButton(L_STICK_UP))
		{
			if (--current_num < 0)
			{
				current_num = ITEM_NUM - 1;
			}
			PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
		}
		//Aボタンで決定
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
	}
	//アニメーション中の処理
	else
	{
		//一定時間経ったらアニメーション終了
		if (++start_anim_timer > START_ANIM || InputPad::OnButton(XINPUT_BUTTON_A))
		{
			start_anim_flg = false;
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

	//2キーで背景生成し直し
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

	//背景画像描画
	DrawGraph(0, 0, bg_image, TRUE);

	SetFontSize(96);
	// フォントを利用して文字を描画
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2 +1,51 }, "CoinToshot", 0x000000);
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "CoinToshot", 0xffff00);

	int size = 48;
	SetFontSize(32);
	
	for (int i = 0; i < ITEM_NUM; i++)
	{
		//カーソルと項目の描画
		if (current_num == i)
		{
			UserData::DrawCoin({ (float)SCREEN_WIDTH / 2 - 30, (float)SCREEN_HEIGHT / 2 + (i * size) + 15 }, 15);
			//	DrawCircle(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + (i * 30)+15, 15, 0x00ff00, true);
			//項目の描画
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * size), 0x00ff00, "%s", item_text[i]);
		}
		//項目のみの描画
		else
		{
			//項目の描画
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * size), 0xffffff, "%s", item_text[i]);
		}
	}

	if (start_anim_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((255.f / START_ANIM) * start_anim_timer));
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
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
	int ret = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);	//画像仮格納

	//生成した背景を一つの画像として保存、それ以外の情報は削除
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