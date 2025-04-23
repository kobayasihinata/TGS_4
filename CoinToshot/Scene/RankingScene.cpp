#include "RankingScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
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

	//A�{�^���Ń^�C�g���ɖ߂�
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		return eSceneType::eTitle;
	}

#ifdef _DEBUG

	//���͋@�\�̎擾
	InputKey* input = InputKey::Get();

	//1�L�[�ŃC���Q�[����ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eInGame;
	}
#endif // _DEBUG
	return __super::Update(_delta);
}

void RankingScene::Draw()const
{
	DrawString(10, 10, "Ranking", GetColor(255, 255, 255));
	DrawString(10, 30, "Pad:A = Title", GetColor(255, 255, 255));

}

void RankingScene::Finalize()
{

}

eSceneType RankingScene::GetNowSceneType()const
{
	return eSceneType::eRanking;
}



