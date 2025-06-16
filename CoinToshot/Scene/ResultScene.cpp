#include "ResultScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

ResultScene::ResultScene()
{
	now_disp = DispScene::dIsClear;

	start_anim_timer= 0;
	result_anim_timer = 0;
	bonus_anim_timer = 0;
	add_anim_num = ((int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)) / (BONUS_ANIM_TIME / 5);
	add_anim_coin = UserData::coin;
	add_coin_once = false;
	disp_se_once = true;

	current_x = 0;
	current_y = 0;


	//BGM読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	result_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	button_se = rm->GetSounds("Resource/Sounds/enter.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	erase_se = rm->GetSounds("Resource/Sounds/explosion.wav");
	//クリアかゲームオーバーかでSEを変える
	disp_se = UserData::is_clear ? rm->GetSounds("Resource/Sounds/Coin/Get.mp3"): rm->GetSounds("Resource/Sounds/bishi.wav");
	coin_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	SetVolumeSoundMem(8000, result_bgm);
	if (!CheckSoundMem(result_bgm))
	{
		PlaySoundMem(result_bgm, DX_PLAYTYPE_BACK);
	}
}

ResultScene::~ResultScene()
{
	StopSoundMem(result_bgm);
}

void ResultScene::Initialize()
{
	__super::Initialize();
}

eSceneType ResultScene::Update(float _delta)
{
	if (!disp_se_once)
	{
		PlaySoundMem(disp_se, DX_PLAYTYPE_BACK);
		disp_se_once = true;
	}
	switch (now_disp)
	{
	case DispScene::dIsClear:	//ゲームクリアかオーバーか表示
		start_anim_timer++;
		//一定時間経過後、Aボタンを押したとき
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//演出が終了していたらボーナス加算演出開始
			if (start_anim_timer > START_ANIM_TIME)
			{
				now_disp = DispScene::dDispResult;
				PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
			}
			//終了していなければ演出スキップ
			else
			{
				//スキップは演出終了の15フレーム前まで
				start_anim_timer = start_anim_timer >= START_ANIM_TIME - SKIP_TIME ? start_anim_timer : START_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dDispResult://結果を表示
		result_anim_timer++;
		//タイマーで結果を順番に表示
		if (result_anim_timer == RESULT_ANIM_TIME / 5 ||
			result_anim_timer == (RESULT_ANIM_TIME / 5) * 2 ||
			result_anim_timer == (RESULT_ANIM_TIME / 5) * 3)
		{
			//SE再生
			disp_se_once = false;
		}
		//Aボタンを押したとき
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//演出が終了していたらボーナス加算演出開始
			if (result_anim_timer > RESULT_ANIM_TIME)
			{
				now_disp = DispScene::dBonusPoint;
				PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
			}
			//終了していなければ演出スキップ
			else
			{
				//スキップは演出終了の15フレーム前まで
				result_anim_timer = result_anim_timer >= RESULT_ANIM_TIME - SKIP_TIME ? result_anim_timer : RESULT_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dBonusPoint://点数加算を表示
		bonus_anim_timer++;
		if (bonus_anim_timer ==  BONUS_ANIM_TIME / 5 ||
			bonus_anim_timer == (BONUS_ANIM_TIME / 5) * 2 ||
			bonus_anim_timer == (BONUS_ANIM_TIME / 5) * 3)
		{
			//SE再生
			disp_se_once = false;
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5) * 4)
		{
			if (!add_coin_once)
			{
				UserData::coin += (int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60);
				add_coin_once = true;
			}
			//加算演出
			if (bonus_anim_timer < BONUS_ANIM_TIME - SKIP_TIME)
			{
				add_anim_coin += add_anim_num;
				if (bonus_anim_timer % 3 == 0)
				{
					PlaySoundMem(coin_se, DX_PLAYTYPE_BACK);
				}
			}

		}
		//Aボタンを押したとき
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//演出が終了していたら遷移
			if (bonus_anim_timer > BONUS_ANIM_TIME)
			{
				PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
				//ランキング最下位のスコアを上回っていたら
				if (UserData::coin > UserData::ranking_data[9].coin)
				{
					//名前入力に遷移
					now_disp = DispScene::dEnterName;
				}
				//ランク外ならランキング表示シーンに飛ぶ
				else
				{
					return eSceneType::eRanking;
				}

			}
			//終了していなければ演出スキップ
			else
			{
				//スキップは演出終了の一定フレーム前まで
				bonus_anim_timer = bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? bonus_anim_timer : BONUS_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dEnterName:
		//名前入力
		return EnterName();
		break;
	default:
		break;
	}

#ifdef _DEBUG
	//入力機能の取得
	InputKey* input = InputKey::Get();

	//1キーでタイトル画面に遷移する
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}
#endif // _DEBUG

	return __super::Update(_delta);
}

void ResultScene::Draw()const
{
	int bg_color, text_color1, text_color2, text_color3;
	DrawString(10, 10, "Result", GetColor(255, 255, 255));

	switch (now_disp)
	{
	case DispScene::dIsClear:
		if (UserData::is_clear)
		{
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, start_anim_timer * (255 / (START_ANIM_TIME / 2)));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffbb, TRUE);
			SetFontSize(64);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 -1,SCREEN_HEIGHT / 2 - 51 }, "Congratulations...!", 0x000000);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 +1,SCREEN_HEIGHT / 2 - 49 }, "Congratulations...!", 0x000000);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2   ,SCREEN_HEIGHT / 2 - 50 }, "Congratulations...!", 0xffff00);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		else
		{
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, start_anim_timer * (255 / (START_ANIM_TIME / 2)));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
			SetFontSize(64);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 - 1,SCREEN_HEIGHT / 2 - 51 }, "Nice try!", 0xffffff);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 + 1,SCREEN_HEIGHT / 2 - 49 }, "Nice try!", 0xffffff);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2   ,SCREEN_HEIGHT / 2 - 50 },  "Nice try!", 0xbbbb00);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}

		if (start_anim_timer > START_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, 60);
		}

		break;
	case DispScene::dDispResult:
		if (UserData::is_clear)
		{
			bg_color = 0xffffbb;
			text_color1 = 0x888800;
			text_color2 = 0xaaaa00;
			text_color3 = 0xffbb00;
		}
		else
		{
			bg_color = 0x555500;
			text_color1 = 0xaaaa00;
			text_color2 = 0xbbbb00;
			text_color3 = 0xaa6600;
		}

		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_color, TRUE);
		SetFontSize(64);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "Result", result_anim_timer % 120 > 60 ? text_color1 : text_color2);
		SetFontSize(36);
		//タイマーで結果を順番に表示
		if (result_anim_timer > RESULT_ANIM_TIME / 5)
		{
			DrawString(SCREEN_WIDTH / 2 - 120, 250, "Time:", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)), 250, text_color1, "%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			if(DEFAULT_TIMELIMIT - UserData::timer == DEFAULT_TIMELIMIT)DrawString(SCREEN_WIDTH / 2 + 130, 250, "← MAX!!", result_anim_timer % 30 > 15 ? text_color1 : text_color3);
		}
		if (result_anim_timer > (RESULT_ANIM_TIME / 5) * 2)
		{
			DrawString(SCREEN_WIDTH / 2 - 80, 310, "HP:", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)UserData::player_hp), 310, text_color1, "%d", (int)UserData::player_hp);
		}
		if (result_anim_timer > (RESULT_ANIM_TIME / 5) * 3)
		{
			UserData::DrawCoin({ SCREEN_WIDTH / 2 - 75, 395 }, 20);
			DrawString(SCREEN_WIDTH / 2 - 35, 370, ":", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", UserData::coin), 370, text_color1, "%d", UserData::coin);
		}

		if (result_anim_timer > RESULT_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, 60);
		}
		break;
	case DispScene::dBonusPoint:
		if (UserData::is_clear)
		{
			bg_color = 0xffffbb;
			text_color1 = 0x888800;
			text_color2 = 0xaaaa00;
			text_color3 = 0xffbb00;
		}
		else
		{
			bg_color = 0x555500;
			text_color1 = 0x999900;
			text_color2 = 0x666600;
			text_color3 = 0x995500;
		}

		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_color, TRUE);
		SetFontSize(64);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "Bonus!!", bonus_anim_timer % 30 > 15 ? text_color1 : text_color3);
		SetFontSize(36);			

		DrawString(SCREEN_WIDTH / 2 - 120, 250, "Time:", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)), 250, text_color1, "%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));

		DrawString(SCREEN_WIDTH / 2 - 80, 310, "HP:", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", (int)UserData::player_hp), 310, text_color1, "%d", (int)UserData::player_hp);

		UserData::DrawCoin({ SCREEN_WIDTH / 2 - 75, 395 }, 20);
		DrawString(SCREEN_WIDTH / 2 - 35, 370, ":", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 100 - GetDrawFormatStringWidth("%d", bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? UserData::coin : add_anim_coin), 370, text_color1, "%d", bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? UserData::coin : add_anim_coin);

		//ボーナス加算描画
		if (bonus_anim_timer > BONUS_ANIM_TIME / 5)
		{
			DrawFormatString(SCREEN_WIDTH / 2 + 130, 250, 0xffbb00, "=    ×%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 175, 270 }, 20);
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5)*2)
		{
			DrawFormatString(SCREEN_WIDTH / 2 + 130, 310, 0xffbb00, "=    ×%d", (int)UserData::player_hp * 10);
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 175, 330 }, 20);
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5) * 3)
		{
			SetFontSize(48);
			DrawFormatString(SCREEN_WIDTH / 2 + 130, 365, bonus_anim_timer % 30 > 15 ? 0xffbb00 : 0xff0000, "+    ×%d", (int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 200, 390 }, 30);
		}
		if (bonus_anim_timer > BONUS_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, 60);
		}
		break;
	case DispScene::dEnterName:
		EnterNameDraw();
		break;
	default:
		break;
	}

}

void ResultScene::Finalize()
{

}

eSceneType ResultScene::GetNowSceneType()const
{
	return eSceneType::eResult;
}

eSceneType ResultScene::EnterName()
{

	//十字キーか左スティックで項目の移動
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) || InputPad::GetPressedButton(L_STICK_LEFT))
	{
		//空白が選択されなくなるまで繰り返す(空白を飛ばす)
		do{
			//移動下限
			if (--current_x < 0)
			{
				current_x = KEY_WIDTH - 1;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::GetPressedButton(L_STICK_RIGHT))
	{
		//空白が選択されなくなるまで繰り返す(空白を飛ばす)
		do {
			//移動下限
			if (++current_x > KEY_WIDTH - 1)
			{
				current_x = 0;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP))
	{
		//空白が選択されなくなるまで繰り返す(空白を飛ばす)
		do {
			//移動下限
			if (--current_y < 0)
			{
				current_y = KEY_HEIGHT - 1;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}
	if (InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN))
	{
		//空白が選択されなくなるまで繰り返す(空白を飛ばす)
		do {
			//移動上限
			if (++current_y > KEY_HEIGHT - 1)
			{
				current_y = 0;
			}
		} while (key[current_y][current_x] == ' ');
		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
	}

	//Aボタンを押して文字の追加(現在の入力が10文字未満なら)
	if (name.size() < 10 && InputPad::OnButton(XINPUT_BUTTON_A))
	{
		name.push_back(key[current_y][current_x]);
		PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
	}

	//Bボタンを押して一文字消す(現在の入力が0文字より大きいなら)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_B))
	{
		name.erase(name.end()-1);
		PlaySoundMem(erase_se, DX_PLAYTYPE_BACK);
	}

	//STARTボタンで入力終了(一文字以上入力されているなら)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_START))
	{
		UserData::ranking_data[9].name = name.c_str();
		UserData::ranking_data[9].coin = UserData::coin;
		SortRanking();
		UserData::WriteRankingData();
		PlaySoundMem(button_se, DX_PLAYTYPE_BACK);
		return eSceneType::eRanking;
	}
	if (++frame > 3600)
	{
		frame = 0;
	}
	return GetNowSceneType();
}

void ResultScene::EnterNameDraw()const 
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	SetFontSize(24);

	DrawString(SCREEN_WIDTH / 2+1, 401, ":入力",  0x000000);
	DrawString(SCREEN_WIDTH / 2,   400, ":入力",  0xffff00);
	DrawString(SCREEN_WIDTH / 2+1, 441, ":一字消す",0x000000);
	DrawString(SCREEN_WIDTH / 2, 440, ":一字消す",0xffff00);
	DrawString(SCREEN_WIDTH / 2+1, 481, ":確定",    0x000000);
	DrawString(SCREEN_WIDTH / 2, 480, ":確定",    0xffff00);

	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 400 }, XINPUT_BUTTON_A, 50);
	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 440 }, XINPUT_BUTTON_B, 50);
	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 480 }, XINPUT_BUTTON_START, 50);

	SetFontSize(32);
	//文字の描画
	for (int y = 0; y < KEY_HEIGHT; y++)
	{
		for (int x = 0; x < KEY_WIDTH; x++)
		{
			//選択されている項目の色を変える
			if (current_x == x && current_y == y)
			{
				DrawFormatString(401 + x * 40, 201 + y * 40, 0x000000, "%c", key[y][x]);
				DrawFormatString(400 + x * 40, 200 + y * 40, 0xff9900, "%c", key[y][x]);
			}
			else
			{
				DrawFormatString(401 + x * 40, 201 + y * 40, 0x000000, "%c", key[y][x]);
				DrawFormatString(400 + x * 40, 200 + y * 40, 0xffff88, "%c", key[y][x]);
			}
		}
	}
	//現在の入力
	DrawFormatString(SCREEN_WIDTH / 2+1, 51, 0x000000, "name:%s", name.c_str());
	DrawFormatString(SCREEN_WIDTH / 2, 50, 0xffff00, "name:%s", name.c_str());
	if ((int)frame % 30 > 15)DrawLine(SCREEN_WIDTH / 2, 82, SCREEN_WIDTH / 2 + 300, 82, 0xffff00);
}

void ResultScene::SortRanking()
{
	RankingData work;	//並び替え用の変数

	// 選択法ソート
	for (int i = 0; i < RANKING_DATA - 1; i++) {
		for (int j = i + 1; j < RANKING_DATA; j++) {
			if (UserData::ranking_data[i].coin <= UserData::ranking_data[j].coin) {
				work = UserData::ranking_data[i];
				UserData::ranking_data[i] = UserData::ranking_data[j];
				UserData::ranking_data[j] = work;
			}
		}
	}

	// 順位付け
	for (int i = 0; i < RANKING_DATA; i++) {
		UserData::ranking_data[i].num = 1;
	}

	// 得点が同じ場合は、同じ順位とする
	// 同順位があった場合の次の順位はデータ個数が加算された順位とする
	for (int i = 0; i < RANKING_DATA - 1; i++) {
		for (int j = i + 1; j < RANKING_DATA; j++) {
			if (UserData::ranking_data[i].coin > UserData::ranking_data[j].coin) {
				UserData::ranking_data[j].num++;
			}
		}
	}
}
