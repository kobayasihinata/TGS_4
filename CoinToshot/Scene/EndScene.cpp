#include "EndScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/UserData.h"

EndScene::EndScene()
{

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

	//3�b��������Q�[���I��
	if (frame >= 180)
	{
		return eSceneType::eNull;
	}
	//A�{�^���Ń^�C�g���ɖ߂�
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		return eSceneType::eTitle;
	}
#ifdef _DEBUG
	//���͋@�\�̎擾
	InputKey* input = InputKey::Get();

	//1�L�[�Ń^�C�g����ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		return eSceneType::eTitle;
	}
#endif // _DEBUG
	return __super::Update(_delta);
}

void EndScene::Draw()const
{
	DrawString(10, 10, "End", GetColor(255, 255, 255));
	DrawString(10, 30, "Key:1 or Pad:A = Title", GetColor(255, 255, 255));
}

void EndScene::Finalize()
{

}

eSceneType EndScene::GetNowSceneType()const
{
	return eSceneType::eEnd;
}



