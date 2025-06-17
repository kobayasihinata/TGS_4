#include "RankingScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

RankingScene::RankingScene()
{
	//BGM読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	ranking_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	SetVolumeSoundMem(8000, ranking_bgm);
	if (!CheckSoundMem(ranking_bgm))
	{
		PlaySoundMem(ranking_bgm,DX_PLAYTYPE_BACK,TRUE);
	}
}

RankingScene::~RankingScene()
{
	//リザルト→ランキング→タイトルと遷移した後、
	UserData::my_ranking_data = { -1,"ありえない",-1 };

	StopSoundMem(ranking_bgm);
}

void RankingScene::Initialize()
{
	__super::Initialize();
}

eSceneType RankingScene::Update(float _delta)
{

	//Aボタンでタイトルに戻る
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
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

void RankingScene::Draw()const
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	SetFontSize(32);

	DrawString(SCREEN_WIDTH / 2 + 1, SCREEN_HEIGHT-69, ":タイトル", 0x000000);
	DrawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT-70, ":タイトル", 0xffff00);

	UserData::DrawButtonImage({ SCREEN_WIDTH / 2 - 40,  SCREEN_HEIGHT - 40 }, XINPUT_BUTTON_A, 50);

	DrawString(181, 161,"No", 0xffffff);
	DrawString(180, 160,"No", 0x000000);
	DrawString(261, 161,"Name", 0xffffff);
	DrawString(260, 160,"Name", 0x000000);
	DrawString(651, 161,"coin", 0xffffff);
	DrawString(650, 160,"coin", 0x000000);
	for (int i = 0; i < RANKING_DATA; i++)
	{
		if (UserData::my_ranking_data.num == UserData::ranking_data[i].num &&
			UserData::my_ranking_data.name == UserData::ranking_data[i].name &&
			UserData::my_ranking_data.coin == UserData::ranking_data[i].coin
			)
		{
			DrawFormatString(180, 200 + i * 40, (int)frame %30 >15 ? 0xffffff:rank_color[i], "%d", UserData::ranking_data[i].num);
			DrawFormatString(260, 200 + i * 40, (int)frame %30 >15 ? 0xffffff: rank_color[i], "%s", UserData::ranking_data[i].name.c_str());
			DrawFormatString(650, 200 + i * 40, (int)frame %30 >15 ? 0xffffff: rank_color[i], "%d", UserData::ranking_data[i].coin);
		}
		else
		{
			DrawFormatString(180, 200 + i * 40, rank_color[i], "%d", UserData::ranking_data[i].num);
			DrawFormatString(260, 200 + i * 40, rank_color[i], "%s", UserData::ranking_data[i].name.c_str());
			DrawFormatString(650, 200 + i * 40, rank_color[i], "%d", UserData::ranking_data[i].coin);
		}
	}
}

void RankingScene::Finalize()
{

}

eSceneType RankingScene::GetNowSceneType()const
{
	return eSceneType::eRanking;
}



