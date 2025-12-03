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

	for (int i = 0; i < LOGO_NUM; i++)
	{
		logo_shift[i] = GetRand(30)+60;
	}
	start_anim_flg = true;
	start_anim_timer = 0;
	tuto_reset_flg = false;
	tuto_loc = { SCREEN_WIDTH / 2 - 200, 400 };
	tuto_size = { 150,72 };
	tuto_current_num = 0;
	current_num = 0;
	old_current_num = 0;
	bg_image = CreateBackGround();

	menu_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	menu_size = { 300,72 };
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
			if (UserData::CheckCursorMove(DOWN))
			{
				if (++current_num >= ITEM_NUM)
				{
					current_num = 0;
				}
			}

			//上入力で項目上移動
			if (UserData::CheckCursorMove(UP))
			{
				if (--current_num < 0)
				{
					current_num = ITEM_NUM - 1;
				}
			}

			//マウスカーソルの位置に合わせて項目変更(キーマウ操作なら)
			if (UserData::control_type == 2)
			{
				for (int i = 0; i < ITEM_NUM; i++)
				{
					if (UserData::CheckCursor({ menu_loc.x,menu_loc.y + (i * menu_size.y) }, menu_size))
					{
						current_num = i;
					}
				}
			}
			//カーソルの位置が変わっていたらSE
			if (old_current_num != current_num)
			{
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
				old_current_num = current_num;
			}
			//決定ボタン
			if (UserData::CheckEnter())
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
			if (UserData::CheckCursorMove(RIGHT))
			{
				if (++tuto_current_num >= 2)
				{
					tuto_current_num = 0;
				}
			}
			//左入力で項目左移動
			if (UserData::CheckCursorMove(LEFT))
			{
				if (--tuto_current_num < 0)
				{
					tuto_current_num = 1;
				}
			}
			//マウスカーソルの位置に合わせて項目変更(キーマウ操作なら)
			if (UserData::control_type == 2)
			{
				for (int i = 0; i < 2; i++)
				{
					if (UserData::CheckCursor({ tuto_loc.x + (i * 400),tuto_loc.y}, tuto_size))
					{
						tuto_current_num = i;
					}
				}
			}
			//カーソルの位置が変わっていたらSE
			if (old_tuto_current_num != tuto_current_num)
			{
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
				old_tuto_current_num = tuto_current_num;
			}
			//決定ボタン
			if (UserData::CheckEnter())
			{
				ResourceManager::rPlaySound(enter_se, DX_PLAYTYPE_BACK);
				switch (tuto_current_num)
				{
				case 0:
					tutorial->tuto_flg = true;
					return eSceneType::eInGame;
					break;
				case 1:
					tutorial->tuto_flg = false;
					tutorial->Initialize();
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
		if (++start_anim_timer > START_ANIM || UserData::CheckEnter())
		{
			start_anim_flg = false;
		}
	}
	
	return __super::Update(_delta);
}

void TitleScene::Draw()const
{	
	//背景画像描画
	DrawGraph(0, 0, bg_image, TRUE);
	
	//色
	for (int i = 0; i < 255; i++)
	{
		for (int j = 0; j < 255; j++)
		{
			DrawPixel(100 + i, 200 + j, GetColor(i, j, (int)frame % 255));
		}
	}

	//タイトル文字
	SetFontSize(184);

	int logo_width = GetDrawStringWidth("CoinToShot", strlen("CoinToShot"));
	Vector2D logo_loc = { SCREEN_WIDTH / 2 - logo_width / 2,50 };	//描画開始位置
	string logo[LOGO_NUM] = { "C","o","i","n","T","o","S","h","o","t" };	//描画文字
	int draw_width = 0;
	int shift;				//ロゴの移動値を保存

	for (int i = 0; i < LOGO_NUM; i++)
	{
		shift = abs(((int)frame + (i*5)) % logo_shift[i] - logo_shift[i]/2) / 10;
		DrawStringF(logo_loc.x + draw_width + 2, logo_loc.y + shift + 2, logo[i].c_str(), 0x000000);
		DrawStringF(logo_loc.x + draw_width - 2, logo_loc.y + shift - 2, logo[i].c_str(), 0xffffff);
		DrawStringF(logo_loc.x + draw_width, logo_loc.y + shift, logo[i].c_str(), 0xddbb00);
		draw_width += GetDrawStringWidth(logo[i].c_str(), strlen(logo[i].c_str()));
	}

	SetFontSize(menu_size.y);
	
	for (int i = 0; i < ITEM_NUM; i++)
	{
		DrawFormatString(menu_loc.x + 2, menu_loc.y + (i * menu_size.y) + 2, 0xbbbb00, "%s", item_text[i]);
		//カーソルと項目の描画
		if (current_num == i)
		{
			UserData::DrawCoin({ menu_loc.x - 30, menu_loc.y + (i * menu_size.y) + menu_size.y /2 }, 20,227+abs(((int)frame%56 - 28)),200 );
			//	DrawCircle(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + (i * 30)+15, 15, 0x00ff00, true);
			//項目の描画
			DrawFormatString(menu_loc .x-5, menu_loc.y + (i * menu_size.y)-5, GetColor(227 + abs(((int)frame % 56 - 28)), 200,0), "%s", item_text[i]);
		}
		//項目のみの描画
		else
		{
			//項目の描画
			DrawFormatString(menu_loc.x, menu_loc.y + (i * menu_size.y), 0xffffff, "%s", item_text[i]);
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
		UserData::DrawStringCenter({ tuto_loc.x + 100, tuto_loc.y - 100 }, "チュートリアルを飛ばしますか？", 0xffffff);
		SetFontSize(72);
		DrawString(tuto_loc.x, tuto_loc.y, "はい", tuto_current_num == 0 ? 0xff5555 : 0x999999);
		DrawString(tuto_loc.x+400, tuto_loc.y, "いいえ", tuto_current_num == 1 ? 0xff5555 : 0x999999);
	}

	if (start_anim_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((255.f / START_ANIM) * start_anim_timer));
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
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