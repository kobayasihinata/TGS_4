#include "ResultScene.h"
#include "../Utility/InputKey.h"
#include "DxLib.h"
#include "../Utility/UserData.h"

ResultScene::ResultScene()
{

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
	//入力機能の取得
	InputKey* input = InputKey::Get();

	//1キーでタイトル画面に遷移する
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}

	return __super::Update(_delta);
}

void ResultScene::Draw()const
{
	DrawString(10, 10, "Result", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Title", GetColor(255, 255, 255));

	if (UserData::is_clear)
	{
		DrawString(100, 10, "Clear", GetColor(255, 255, 255));
	}
	else
	{
		DrawString(100, 10, "GameOver", GetColor(255, 255, 255));

	}
}

void ResultScene::Finalize()
{

}

eSceneType ResultScene::GetNowSceneType()const
{
	return eSceneType::eResult;
}



