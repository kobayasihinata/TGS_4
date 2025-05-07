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
	//名前入力は後で作ります

	//十字キーか左スティックで項目の移動
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_LEFT) || InputPad::OnButton(L_STICK_LEFT))
	{
		current_x--;
	}
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::OnButton(L_STICK_RIGHT))
	{
		current_x++;
	}
	//Aボタンを押したときの選択されている項目で文字を決める
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		name.push_back(97 + current_x);
	}

	//STARTボタンで入力終了
	if (InputPad::OnButton(XINPUT_BUTTON_START))
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
	DrawString(10, 30, "Pad:START = End", GetColor(255, 255, 255));

	//選択されている項目のカーソル
	DrawCircle(current_x * 40 + 100, current_y * 40 + 100, 40,0x00ff00, false);
	DrawFormatString(current_x * 40 + 100, current_y * 40 + 100, 0x00ff00, "%d", current_x);
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
