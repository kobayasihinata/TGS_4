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
	graph_timer = 0;

	add_anim_num = ((int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)) / (BONUS_ANIM_TIME / 5);
	add_anim_coin = UserData::coin;
	add_coin_once = false;
	disp_se_once = true;
	graph_loc = { (SCREEN_WIDTH / 2) - (GRAPH_WIDTH / 2),(SCREEN_HEIGHT / 2) - (GRAPH_HEIGHT / 2) };
	name_string_loc = { 750,120 };
	key_box_loc = { 600,250 };
	current_x = 0;
	current_y = 0;

	//自身のデータをリセットしておく
	UserData::my_ranking_data = { -1,"ありえない",-1 };

	//BGM読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	result_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	button_se = rm->GetSounds("Resource/Sounds/enter.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	erase_se = rm->GetSounds("Resource/Sounds/explosion.wav");
	//クリアかゲームオーバーかでSEを変える
	disp_se = UserData::is_clear ? rm->GetSounds("Resource/Sounds/Coin/Get.mp3"): rm->GetSounds("Resource/Sounds/bishi.wav");
	coin_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	if (!CheckSoundMem(result_bgm))
	{
		ResourceManager::rPlaySound(result_bgm, DX_PLAYTYPE_LOOP);
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
		ResourceManager::rPlaySound(disp_se, DX_PLAYTYPE_BACK);
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
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
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
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
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
					ResourceManager::rPlaySound(coin_se, DX_PLAYTYPE_BACK);
				}
			}

		}
		//Aボタンを押したとき
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//演出が終了していたら遷移
			if (bonus_anim_timer > BONUS_ANIM_TIME)
			{
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
				now_disp = DispScene::dGraphDisp;
			}
			//終了していなければ演出スキップ
			else
			{
				//スキップは演出終了の一定フレーム前まで
				bonus_anim_timer = bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? bonus_anim_timer : BONUS_ANIM_TIME - SKIP_TIME;
			}
		}
		break;
	case DispScene::dGraphDisp:
		//測定
		graph_timer++;
		//Aボタンを押したとき
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
			//演出が終了していれば遷移
			if (graph_timer > 120)
			{
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
			else
			{
				graph_timer == 120;
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
	int bg_color, text_color1, text_color2, text_color3, g_count = 1, disp_time = 0;
	float graph_space = (float)GRAPH_WIDTH / (DEFAULT_TIMELIMIT / 60);
	float g_old = 0;
	DrawString(10, 10, "Result", GetColor(255, 255, 255));

	switch (now_disp)
	{
	case DispScene::dIsClear:
		if (UserData::is_clear)
		{
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, TRUE);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, start_anim_timer * (255 / (START_ANIM_TIME / 2)));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffbb, TRUE);
			SetFontSize(96);
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
			SetFontSize(96);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 - 1,SCREEN_HEIGHT / 2 - 51 }, "Nice try!", 0xffffff);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2 + 1,SCREEN_HEIGHT / 2 - 49 }, "Nice try!", 0xffffff);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2   ,SCREEN_HEIGHT / 2 - 50 },  "Nice try!", 0xbbbb00);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}

		if (start_anim_timer > START_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 60 }, XINPUT_BUTTON_A, 90);
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
		SetFontSize(96);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,75 }, "Result", result_anim_timer % 120 > 60 ? text_color1 : text_color2);
		SetFontSize(54);
		//タイマーで結果を順番に表示
		if (result_anim_timer > RESULT_ANIM_TIME / 5)
		{
			DrawString(SCREEN_WIDTH / 2 - 200, 250, "Time:", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 130 - GetDrawFormatStringWidth("%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)), 250, text_color1, "%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			if(DEFAULT_TIMELIMIT - UserData::timer == DEFAULT_TIMELIMIT)DrawString(SCREEN_WIDTH / 2 + 140, 250, "← MAX!!", result_anim_timer % 30 > 15 ? text_color1 : text_color3);
		}
		if (result_anim_timer > (RESULT_ANIM_TIME / 5) * 2)
		{
			DrawString(SCREEN_WIDTH / 2 - 140, 320, "HP:", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 130 - GetDrawFormatStringWidth("%d", (int)UserData::player_hp), 320, text_color1, "%d", (int)UserData::player_hp);
		}
		if (result_anim_timer > (RESULT_ANIM_TIME / 5) * 3)
		{
			UserData::DrawCoin({ SCREEN_WIDTH / 2 - 105, 425 }, 30);
			DrawString(SCREEN_WIDTH / 2 - 70, 390, ":", text_color1);
			DrawFormatString(SCREEN_WIDTH / 2 + 130 - GetDrawFormatStringWidth("%d", UserData::coin), 390, text_color1, "%d", UserData::coin);
		}

		if (result_anim_timer > RESULT_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 60 }, XINPUT_BUTTON_A, 90);
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
		SetFontSize(96);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,50 }, "Bonus!!", bonus_anim_timer % 30 > 15 ? text_color1 : text_color3);
		SetFontSize(54);			

		DrawString(SCREEN_WIDTH / 2 - 200, 250, "Time:", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 130 - GetDrawFormatStringWidth("%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60)), 250, text_color1, "%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));

		DrawString(SCREEN_WIDTH / 2 - 140, 320, "HP:", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 130 - GetDrawFormatStringWidth("%d", (int)UserData::player_hp), 320, text_color1, "%d", (int)UserData::player_hp);

		UserData::DrawCoin({ SCREEN_WIDTH / 2 - 105, 425 }, 30);
		DrawString(SCREEN_WIDTH / 2 - 70, 390, ":", text_color1);
		DrawFormatString(SCREEN_WIDTH / 2 + 130 - GetDrawFormatStringWidth("%d", bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? UserData::coin : add_anim_coin), 390, text_color1, "%d", bonus_anim_timer >= BONUS_ANIM_TIME - SKIP_TIME ? UserData::coin : add_anim_coin);

		//ボーナス加算描画
		if (bonus_anim_timer > BONUS_ANIM_TIME / 5)
		{
			DrawFormatString(SCREEN_WIDTH / 2 + 150, 250, 0xffbb00, "=    ×%d", (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 215, 280 }, 30);
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5)*2)
		{
			DrawFormatString(SCREEN_WIDTH / 2 + 150, 320, 0xffbb00, "=    ×%d", (int)UserData::player_hp * 10);
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 215, 350 }, 30);
		}
		if (bonus_anim_timer > (BONUS_ANIM_TIME / 5) * 3)
		{
			SetFontSize(62);
			DrawFormatString(SCREEN_WIDTH / 2 + 150, 385, bonus_anim_timer % 30 > 15 ? 0xffbb00 : 0xff0000, "+    ×%d", (int)UserData::player_hp * 10 + (int)((DEFAULT_TIMELIMIT - UserData::timer) / 60));
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 235, 430 }, 45);
		}
		if (bonus_anim_timer > BONUS_ANIM_TIME)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT -60 }, XINPUT_BUTTON_A, 90);
		}
		break;
	case DispScene::dGraphDisp:
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

		//背景
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_color, TRUE);
		//グラフ背景
		DrawBoxAA(graph_loc.x, 
			graph_loc.y, 
			graph_loc.x + GRAPH_WIDTH, 
			graph_loc.y + GRAPH_HEIGHT,
			0xffffff, TRUE);
		SetFontSize(27);
		//グリッド
		for (int x = 0; x <= GRAPH_WIDTH; x+= (graph_space*(15)))
		{
			//60秒区切りでグリッド色を変更＆数字を描画
			if (x % ((int)(graph_space * 15) * 4) == 0)
			{
				DrawLineAA(graph_loc.x + x,
					graph_loc.y,
					graph_loc.x + x,
					graph_loc.y + GRAPH_HEIGHT,
					0xffaaaa);
				DrawFormatStringF(graph_loc.x + x,
					graph_loc.y + GRAPH_HEIGHT,
					0xff0000,
					"%d秒", disp_time);
				disp_time += 60;
			}
			else
			{
				DrawLine(graph_loc.x + x,
					graph_loc.y,
					graph_loc.x + x,
					graph_loc.y + GRAPH_HEIGHT,
					0xaaaaaa);
			}
		}
		for (int y = 0; y <= GRAPH_HEIGHT; y += (GRAPH_HEIGHT/10))
		{
			//1000枚区切りでグリッド色を変更＆数字を描画
			if (y % (GRAPH_HEIGHT/5) == 0)
			{
				DrawLineAA(graph_loc.x,
					graph_loc.y + y,
					graph_loc.x + GRAPH_WIDTH,
					graph_loc.y + y,
					0xaaffaa);
				DrawFormatStringF(graph_loc.x - GetDrawFormatStringWidth("%d枚", 7000 - y * 10),
					graph_loc.y + y-12,
					0x00ff00,
					"%d枚", 7000 - y*10);
			}
			else
			{
				DrawLine(graph_loc.x,
					graph_loc.y + y,
					graph_loc.x + GRAPH_WIDTH,
					graph_loc.y + y,
					0xaaaaaa);
			}
		}
		for (auto& coin_graph : UserData::coin_graph)
		{
			DrawLineAA(graph_loc.x + (g_count - 1) * graph_space,
				graph_loc.y + GRAPH_HEIGHT - g_old/(7000/ GRAPH_HEIGHT),
				graph_loc.x + g_count * graph_space,
				graph_loc.y + GRAPH_HEIGHT - coin_graph / (7000 / GRAPH_HEIGHT),
				0x000000, TRUE);
			g_old = coin_graph;
			g_count++;
			//for (int i = 0; i < UserData::replay.size(); i++)
			//{
			//	//リプレイの秒数と一致しているなら描画
			//	if (UserData::replay[i].time == g_count)
			//	{
			//		DrawRotaGraph(graph_loc.x + g_count * graph_space,
			//			graph_loc.y + GRAPH_HEIGHT - coin_graph / (5000 / GRAPH_HEIGHT),
			//			0.3f,
			//			0.f,
			//			UserData::replay[i].image, TRUE);
			//	}
			//}
		}
		if (graph_timer > 120)
		{
			UserData::DrawButtonImage({ SCREEN_WIDTH / 2,SCREEN_HEIGHT - 60 }, XINPUT_BUTTON_A, 90);
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
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
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
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
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
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
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
		ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
	}

	//Aボタンを押して文字の追加(現在の入力と画面内の文字の合計が10文字未満なら)
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
		CreateMoveString(key[current_y][current_x], 
			{ key_box_loc.x + (current_x * 60),key_box_loc.y + (current_y * 60) }, 
			{ name_string_loc.x + 100,name_string_loc.y + 25 },
			{ 700,100 }, 
			true);
	}

	//Bボタンを押して一文字消す(現在の入力が0文字より大きいなら)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_B))
	{
		//現在のテキスト幅を計算
		int size = 0;
		char t;
		for (auto text : name)
		{
			size += GetDrawFormatStringWidth("%c", text);
			t = text;
		}
		CreateMoveString(t, { name_string_loc.x + 80 + size,name_string_loc.y }, { SCREEN_WIDTH/2,SCREEN_HEIGHT }, {SCREEN_WIDTH,100},false, { (float)GetRand(10) - 5,-5 });
		name.erase(name.end()-1);
		ResourceManager::rPlaySound(erase_se, DX_PLAYTYPE_BACK);
	}

	//STARTボタンで入力終了(一文字以上入力されているなら)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_START))
	{
		UserData::my_ranking_data.name = name.c_str();
		UserData::my_ranking_data.coin = UserData::coin;
		UserData::ranking_data[9] = UserData::my_ranking_data;
		SortRanking();
		UserData::WriteData();
		ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
		return eSceneType::eRanking;
	}


	for (auto& string_data : string_data)
	{
		string_data = MoveString(string_data);

		//目的地にたどり着いたら消す&名前入力
		if (string_data.velocity.y > 0.f &&
			string_data.goal_location.x - string_data.goal_size.x/2 <= string_data.location.x &&
			string_data.goal_location.x + string_data.goal_size.x / 2 >= string_data.location.x &&
			string_data.goal_location.y - string_data.goal_size.y / 2 <= string_data.location.y &&
			string_data.goal_location.y + string_data.goal_size.y / 2 >= string_data.location.y
			)
		{
			if(name.size() < 10)delete_string_data.push_back(string_data);
		
			if(string_data.is_add && name.size() < 10)name.push_back(string_data.text);
		}
		//画面外に出ても消す
		if (string_data.location.x <-50 ||
			string_data.location.x > SCREEN_WIDTH + 50 ||
			string_data.location.y > SCREEN_HEIGHT + 50)
		{
			delete_string_data.push_back(string_data);
		}
	}

	//オブジェクト配列から削除する処理
	for (const auto& delete_data : delete_string_data)
	{
		for (auto it = string_data.begin(); it != string_data.end();)
		{
			if (*it == delete_data)
			{
				it = string_data.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	delete_string_data.clear();


	if (++frame > 3600)
	{
		frame = 0;
	}
	return GetNowSceneType();
}

void ResultScene::EnterNameDraw()const 
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	SetFontSize(36);

	DrawString(SCREEN_WIDTH / 2+1, 601, ":入力",  0x000000);
	DrawString(SCREEN_WIDTH / 2,   600, ":入力",  0xffff00);
	DrawString(SCREEN_WIDTH / 2+1, 661, ":一字消す",0x000000);
	DrawString(SCREEN_WIDTH / 2, 660, ":一字消す",0xffff00);
	DrawString(SCREEN_WIDTH / 2+1, 721, ":確定",    0x000000);
	DrawString(SCREEN_WIDTH / 2, 720, ":確定",    0xffff00);

	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 620 }, XINPUT_BUTTON_A, 75);
	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 680 }, XINPUT_BUTTON_B, 75);
	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40, 740 }, XINPUT_BUTTON_START, 75);

	SetFontSize(32);
	//文字の描画
	for (int y = 0; y < KEY_HEIGHT; y++)
	{
		for (int x = 0; x < KEY_WIDTH; x++)
		{
			//選択されている項目の色を変える
			if (current_x == x && current_y == y)
			{
				DrawFormatString(key_box_loc.x+1 + x * 60, key_box_loc.y+1 + y * 60, 0x000000, "%c", key[y][x]);
				DrawFormatString(key_box_loc.x + x * 60, key_box_loc.y + y * 60, 0xff9900, "%c", key[y][x]);
			}
			else
			{
				DrawFormatString(key_box_loc.x+1 + x * 60, key_box_loc.y + 1 + y * 60, 0x000000, "%c", key[y][x]);
				DrawFormatString(key_box_loc.x + x * 60, key_box_loc.y + y * 60, 0xffff88, "%c", key[y][x]);
			}
		}
	}
	//現在の入力
	DrawFormatString(name_string_loc.x+1, name_string_loc.y+1, 0x000000, "name:%s", name.c_str());
	DrawFormatString(name_string_loc.x, name_string_loc.y, 0xffff00, "name:%s", name.c_str());
	if ((int)frame % 30 > 15)DrawLine(name_string_loc.x + 100, name_string_loc.y + GetFontSize(), name_string_loc.x + 400, name_string_loc.y + GetFontSize(), 0xffff00);

	SetFontSize(48);
	//ui_dataの描画
	for (const auto string_data : string_data)
	{
		DrawFormatStringF(string_data.location.x+1,
			string_data.location.y+1,
			0x000000,
			"%c",
			string_data.text
		);
		DrawFormatStringF(string_data.location.x,
			string_data.location.y,
			0xff9900,
			"%c",
			string_data.text
		);
	}
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
	for (int i = 0; i < RANKING_DATA; i++)
	{
		//自身の順位保存
		if (UserData::my_ranking_data.name == UserData::ranking_data[i].name &&
			UserData::my_ranking_data.coin == UserData::ranking_data[i].coin)
		{
			UserData::my_ranking_data.num = i+1;
			break;
		}
	}
}

StringData ResultScene::MoveString(StringData _data)
{
	StringData ret = _data;
	//X軸の減速
	if (ret.velocity.x > 0.f)ret.velocity.x -= DECELERATION;
	if (ret.velocity.x < 0.f)ret.velocity.x += DECELERATION;
	if (fabsf(ret.velocity.x) <= DECELERATION)ret.velocity.x = 0.f;

	//重力
	ret.velocity.y += GRAVITY;
	ret.location += ret.velocity;

	return ret;
}

void ResultScene::CreateMoveString(const char _c, Vector2D _loc, Vector2D _goal_loc, Vector2D _goal_size, bool _is_add, Vector2D _velocity)
{
	StringData data;
	data.text = _c;
	data.location = _loc;
	data.goal_location = _goal_loc;
	data.goal_size = _goal_size;
	data.velocity = _velocity == 0.f ? Vector2D{(data.goal_location.x - data.location.x) / 20,(data.goal_location.y - data.location.y) / 10 -5 } : _velocity;
	data.speed = 1.f;
	data.angle = 0.f;
	data.is_add = _is_add;
	string_data.push_back(data);
}