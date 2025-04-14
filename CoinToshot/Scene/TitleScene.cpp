#include "TitleScene.h"
#include "../Utility/InputKey.h"
#include "DxLib.h"

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	__super::Initialize();

	//input_event->ActionKeyBind(KEY_INPUT_1, eInputState::Held, this, &TitleScene::ChangeInGameScene);

}

eSceneType TitleScene::Update(float _delta)
{
	//入力機能の取得
	InputKey* input = InputKey::Get();

	//1キーでインゲーム画面に遷移する
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eInGame;
	}

	return __super::Update(_delta);
}

void TitleScene::Draw()const
{
	DrawString(10, 10, "Title", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Ingame  2 = Help", GetColor(255, 255, 255));
}

void TitleScene::Finalize()
{

}

eSceneType TitleScene::GetNowSceneType()const
{
	return eSceneType::eTitle;
}
