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
	tutorial = Tutorial::Get();

	start_anim_flg = true;
	start_anim_timer = 0;
	tuto_reset_flg = false;
	tuto_current_num = 0;
	current_num = 0;
	bg_image = CreateBackGround();

	//SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	enter_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	//BGM読み込み
	title_bgm = rm->GetSounds("Resource/Sounds/BGM/Colorful_Cheerful_Jelly_Beans.mp3");
	if (!CheckSoundMem(title_bgm))
	{
		ResourceManager::rPlaySound(title_bgm, DX_PLAYTYPE_LOOP);
	}
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
		if (!tuto_reset_flg)
		{
			//下入力で項目下移動
			if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN))
			{
				if (++current_num >= ITEM_NUM)
				{
					current_num = 0;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//上入力で項目上移動
			if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP))
			{
				if (--current_num < 0)
				{
					current_num = ITEM_NUM - 1;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//Aボタンで決定
			if (InputPad::OnButton(XINPUT_BUTTON_A))
			{
				ResourceManager::rPlaySound(enter_se, DX_PLAYTYPE_BACK);
				switch (current_num)
				{
				case TitleItem::tGameMain:
					StopSoundMem(title_bgm);
					//既プレイならチュートリアルをスキップできる
					if (tutorial->tuto_flg && tutorial->GetBasicTuto())
					{
						tuto_reset_flg = true;
					}
					else
					{
						tutorial->tuto_flg = false;
						tutorial->Initialize();
						return eSceneType::eInGame;
					}
					break;
				case TitleItem::tRanking:
					StopSoundMem(title_bgm);
					return eSceneType::eRanking;
					break;
				case TitleItem::tOption:
					StopSoundMem(title_bgm);
					return eSceneType::eOption;
					break;
				case TitleItem::tEnd:
					return eSceneType::eEnd;
					break;
				default:
					break;
				}

			}
		}
		//チュートリアルをリセットするか選ばせる所
		else
		{
			//右入力で項目右移動
			if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::GetPressedButton(L_STICK_RIGHT))
			{
				if (++tuto_current_num >= 2)
				{
					tuto_current_num = 0;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//左入力で項目左移動
			if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) || InputPad::GetPressedButton(L_STICK_LEFT))
			{
				if (--tuto_current_num < 0)
				{
					tuto_current_num = 1;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//Aボタンで決定
			if (InputPad::OnButton(XINPUT_BUTTON_A))
			{
				ResourceManager::rPlaySound(enter_se, DX_PLAYTYPE_BACK);
				switch (tuto_current_num)
				{
				case 0:
					tutorial->tuto_flg = true;
					return eSceneType::eInGame;
					break;
				case 1:
					tutorial->Initialize();
					tutorial->tuto_flg = false;
					return eSceneType::eInGame;
					break;
				default:
					break;
				}

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

#endif // _DEBUG
	return __super::Update(_delta);
}

void TitleScene::Draw()const
{
	int old = GetFontSize();

	//背景画像描画
	DrawGraph(0, 0, bg_image, TRUE);

	//タイトル文字
	SetFontSize(144);
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2+2,52 }, "CoinToshot", 0x000000);
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2 -2,48 }, "CoinToshot", 0xffffff);
	UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "CoinToshot", 0xddbb00);

	int size = 72;
	SetFontSize(size);
	
	for (int i = 0; i < ITEM_NUM; i++)
	{
		DrawFormatString(SCREEN_WIDTH / 2 + 2, SCREEN_HEIGHT / 2 + (i * size) + 2, 0xbbbb00, "%s", item_text[i]);
		//カーソルと項目の描画
		if (current_num == i)
		{
			UserData::DrawCoin({ (float)SCREEN_WIDTH / 2 - 30, (float)SCREEN_HEIGHT / 2 + (i * size) + size/2 }, 20,227+abs(((int)frame%56 - 28)),200 );
			//	DrawCircle(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + (i * 30)+15, 15, 0x00ff00, true);
			//項目の描画
			DrawFormatString(SCREEN_WIDTH / 2-1, SCREEN_HEIGHT / 2 + (i * size)-1, GetColor(227 + abs(((int)frame % 56 - 28)), 200,0), "%s", item_text[i]);
		}
		//項目のみの描画
		else
		{
			//項目の描画
			DrawFormatString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * size), 0xffffff, "%s", item_text[i]);
		}
	}

	SetFontSize(48);

	//操作説明の描画
	Vector2D _loc = { 700,900 };
	UserData::DrawButtonImage(_loc, L_STICK_UP, 75);
	UserData::DrawButtonImage({ _loc.x+60,_loc.y }, L_STICK_DOWN, 75);
	UserData::DrawButtonImage({ _loc.x+120,_loc.y }, XINPUT_BUTTON_DPAD_UP, 75);
	UserData::DrawButtonImage({ _loc.x+180,_loc.y }, XINPUT_BUTTON_DPAD_DOWN, 75);
	DrawStringF(_loc.x + 210, _loc.y - 30, ":カーソル移動", 0x222200);
	UserData::DrawButtonImage({ _loc.x + 180,_loc.y + 60 }, XINPUT_BUTTON_A, 75);
	DrawStringF(_loc.x + 210, _loc.y+30 , ":決定", 0x222200);

	//チュートリアル描画
	if (tuto_reset_flg)
	{
		SetFontSize(72);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		SetFontSize(108);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,300 }, "チュートリアルを飛ばしますか？", 0xffffff);
		SetFontSize(72);
		DrawString(SCREEN_WIDTH / 2 - 200, 400, "はい", tuto_current_num == 0 ? 0xff5555 : 0x999999);
		DrawString(SCREEN_WIDTH / 2 + 200, 400, "いいえ", tuto_current_num == 1 ? 0xff5555 : 0x999999);
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
	UserData::DrawCoin({ SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 }, 1350);
	SetDrawScreen(DX_SCREEN_BACK);

	return ret;
}