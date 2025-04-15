#include "RankingScene.h"
#include "../Utility/InputKey.h"
#include "DxLib.h"
#include "../Utility/UserData.h"

RankingScene::RankingScene()
{

}

RankingScene::~RankingScene()
{

}

void RankingScene::Initialize()
{
	__super::Initialize();
}

eSceneType RankingScene::Update(float _delta)
{
	//入力機能の取得
	InputKey* input = InputKey::Get();

	//1キーでタイトル画面に遷移する
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}

	return __super::Update(_delta);
}

void RankingScene::Draw()const
{
	DrawString(10, 10, "Ranking", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Title", GetColor(255, 255, 255));

}

void RankingScene::Finalize()
{

}

eSceneType RankingScene::GetNowSceneType()const
{
	return eSceneType::eRanking;
}



