#include "ResultScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"

ResultScene::ResultScene()
{
	now_disp = 0;

	current_x = 0;
	current_y = 0;
}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{
	__super::Initialize();
}

eSceneType ResultScene::Update(float _delta)
{
	switch (now_disp)
	{
	case DispScene::dIsClear:
		//Aボタンを押したとき
		if (InputPad::OnButton(XINPUT_BUTTON_A))
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
	DrawString(10, 10, "Result", GetColor(255, 255, 255));

	switch (now_disp)
	{
	case DispScene::dIsClear:
		DrawString(10, 30, "Pad:A = Next", GetColor(255, 255, 255));
		//クリアかクリアじゃないか表示
		if (UserData::is_clear)
		{
			DrawString(100, 10, "Clear", GetColor(255, 255, 255));
		}
		else
		{
			DrawString(100, 10, "GameOver", GetColor(255, 255, 255));

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
	}

	//Aボタンを押して文字の追加(現在の入力が10文字未満なら)
	if (name.size() < 10 && InputPad::OnButton(XINPUT_BUTTON_A))
	{
		name.push_back(key[current_y][current_x]);
	}

	//Bボタンを押して一文字消す(現在の入力が0文字より大きいなら)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_B))
	{
		name.erase(name.end()-1);
	}

	//STARTボタンで入力終了(一文字以上入力されているなら)
	if (name.size() > 0 && InputPad::OnButton(XINPUT_BUTTON_START))
	{
		UserData::ranking_data[9].name = name.c_str();
		UserData::ranking_data[9].coin = UserData::coin;
		SortRanking();
		UserData::WriteRankingData();
		return eSceneType::eRanking;
	}

	return GetNowSceneType();
}

void ResultScene::EnterNameDraw()const 
{
	DrawString(10, 30, "Pad:START = Enter",  GetColor(255, 255, 255));
	DrawString(10, 50, "Pad:A	  = Add",    GetColor(255, 255, 255));
	DrawString(10, 70, "Pad:B	  = Delete", GetColor(255, 255, 255));

	//文字の描画
	for (int y = 0; y < KEY_HEIGHT; y++)
	{
		for (int x = 0; x < KEY_WIDTH; x++)
		{
			//選択されている項目の色を変える
			if (current_x == x && current_y == y)
			{
				DrawFormatStringF(200 + x * 40, 200 + y * 40, 0xff0000, "%c", key[y][x]);
			}
			else
			{
				DrawFormatStringF(200 + x * 40, 200 + y * 40, 0x00ff00, "%c", key[y][x]);
			}
		}
	}
	//現在の入力
	DrawFormatString(SCREEN_WIDTH / 2, 50, 0x00ff00, "name:%s", name.c_str());
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
