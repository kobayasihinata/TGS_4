#include "RankingScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

RankingScene::RankingScene()
{
	//BGM、SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	button_se = rm->GetSounds("Resource/Sounds/pop.wav");
	ranking_bgm = rm->GetSounds("Resource/Sounds/BGM/Sweet_smelling_flower.mp3");
	if (!CheckSoundMem(ranking_bgm))
	{
		ResourceManager::rPlaySound(ranking_bgm,DX_PLAYTYPE_LOOP,TRUE);
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

	//Aボタンで決定
	if (UserData::CheckEnter())
	{
		ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
		return eSceneType::eTitle;
	}

	return __super::Update(_delta);
}

void RankingScene::Draw()const
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);
	UserData::DrawCoin({ SCREEN_WIDTH +100, SCREEN_HEIGHT / 2 }, 900);
	SetFontSize(48);

	UserData::DrawButtonAndString({ SCREEN_WIDTH / 2 - 50,  SCREEN_HEIGHT - 64 }, XINPUT_BUTTON_A, ":タイトルに戻る", 0xffff00);

	DrawString(161, 151,"No", 0xffffff);
	DrawString(160, 150,"No", 0x000000);
	DrawString(281, 151,"Name", 0xffffff);
	DrawString(280, 150,"Name", 0x000000);
	DrawString(661, 151,"Coin", 0xffffff);
	DrawString(660, 150,"Coin", 0x000000);

	for (int i = 0; i < RANKING_DATA; i++)
	{
		if (UserData::my_ranking_data.num == UserData::ranking_data[i].num &&
			UserData::my_ranking_data.name == UserData::ranking_data[i].name &&
			UserData::my_ranking_data.coin == UserData::ranking_data[i].coin
			)
		{
			UserData::DrawNum({ 170, 200 + i * 60 }, UserData::ranking_data[i].num, (int)frame % 30 > 15 ? 0xffffff : rank_color[i]);
			DrawFormatString(260, 200 + i * 60, (int)frame %30 >15 ? 0xffffff: rank_color[i], "%s", UserData::ranking_data[i].name.c_str());
			UserData::DrawNum({ 780 - UserData::DrawNum({-1000,0},UserData::ranking_data[i].coin,0xffffff), 200.f + i * 60}, UserData::ranking_data[i].coin, (int)frame % 30 > 15 ? 0xffffff : rank_color[i]);
		}									
		else								
		{									
			UserData::DrawNum({ 170, 200 + i * 60 }, UserData::ranking_data[i].num, rank_color[i]);
			DrawFormatString(260, 200 + i * 60, rank_color[i], "%s", UserData::ranking_data[i].name.c_str());
			UserData::DrawNum({ 780 - UserData::DrawNum({-1000,0},UserData::ranking_data[i].coin,0xffffff), 200 + i * 60 }, UserData::ranking_data[i].coin, rank_color[i]);
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



