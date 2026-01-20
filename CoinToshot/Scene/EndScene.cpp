#include "EndScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"
#include "../Utility/ResourceManager.h"

EndScene::EndScene()
{
	release_button = false;

	//SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	button_se = rm->GetSounds("Resource/Sounds/pop.wav");
}

EndScene::~EndScene()
{

}

void EndScene::Initialize()
{
	__super::Initialize();
}

eSceneType EndScene::Update(float _delta)
{
	InputKey* input = InputKey::Get();

	//一定時間たったらゲーム終了
	if (frame >= END_TIMER)
	{
		return eSceneType::eNull;
	}
	//キャンセルボタンでタイトルに戻る
	if (UserData::CheckCancel())
	{
		ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
		return eSceneType::eTitle;
	}
	//Aボタンでカウントを早める
	if (release_button && UserData::CheckEnter(eInputState::Held))
	{
		frame += 6;
	}
	//↑タイトルからの遷移後すぐにカウントを早めないようにする
	if (UserData::CheckEnter(eInputState::Released))
	{
		release_button = true;
	}
#ifdef _DEBUG
#endif // _DEBUG
	return __super::Update(_delta);
}

void EndScene::Draw()const
{
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);

	SetFontSize(48);
	//クレジット描画
	DrawCledit(600 - frame);

	DrawBox(0,
		SCREEN_HEIGHT - 30,
		SCREEN_WIDTH -(((float)SCREEN_WIDTH/ (float)END_TIMER)*frame),
		SCREEN_HEIGHT,
		GetColor(255, 255 - ((255.f / (float)END_TIMER) * frame), 0),
		TRUE);
	UserData::DrawButtonAndString({ SCREEN_WIDTH / 2 +250,  SCREEN_HEIGHT-26}, XINPUT_BUTTON_B, ":タイトルに戻る", 0x444400);

	UserData::DrawButtonAndString({ SCREEN_WIDTH / 2 - 350,  SCREEN_HEIGHT-26}, XINPUT_BUTTON_A, ":早送り", 0x444400);
}

void EndScene::Finalize()
{

}

eSceneType EndScene::GetNowSceneType()const
{
	return eSceneType::eEnd;
}

void EndScene::DrawCledit(float _loc_y)const
{
	int size = GetFontSize();

	//画像素材表記の文字列を描画
	SetFontSize(size * 1.5f);
	UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size}, image_cledit[0], 0xffffff);
	SetFontSize(size);
	for (int i = 1; i < IMAGE_LINE_NUM; i++)
	{
		UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size * i + 80 }, image_cledit[i], 0xffffff);
	}

	//音声素材表記の文字列を描画
	float shift = size * IMAGE_LINE_NUM + 200;
	SetFontSize(size * 1.5f);
	UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size + shift }, sound_cledit[0], 0xffffff);
	SetFontSize(size);
	for (int i = 1; i < SOUND_LINE_NUM; i++)
	{
		UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size * i + 80 + shift }, sound_cledit[i], 0xffffff);
	}
}