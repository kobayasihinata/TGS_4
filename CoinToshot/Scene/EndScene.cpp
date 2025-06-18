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
	if (frame >= END_TIMER)
	{
		return eSceneType::eNull;
	}
	//A�{�^���Ń^�C�g���ɖ߂�
	if (InputPad::OnButton(XINPUT_BUTTON_B))
	{
		return eSceneType::eTitle;
	}
	//B�{�^���ŃJ�E���g�𑁂߂�
	if (InputPad::OnPressed(XINPUT_BUTTON_A))
	{
		frame += 6;
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
	int old = GetFontSize();
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x999900, TRUE);

	SetFontSize(32);
	//�N���W�b�g�`��
	DrawCledit(400 - frame);

	DrawBox(0,
		SCREEN_HEIGHT - 30,
		SCREEN_WIDTH -(((float)SCREEN_WIDTH/ (float)END_TIMER)*frame),
		SCREEN_HEIGHT,
		GetColor(255, 255 - ((255.f / (float)END_TIMER) * frame), 0),
		TRUE);
	UserData::DrawButtonAndString({ SCREEN_WIDTH / 2 +250,  SCREEN_HEIGHT-16}, XINPUT_BUTTON_B, ":�^�C�g���ɖ߂�", 0x444400);

	UserData::DrawButtonAndString({ SCREEN_WIDTH / 2 - 350,  SCREEN_HEIGHT-16}, XINPUT_BUTTON_A, ":�X�L�b�v", 0x444400);
	SetFontSize(old);
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

	//�摜�f�ޕ\�L�̕������`��
	SetFontSize(size * 1.5f);
	UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size}, image_cledit[0], 0xffffff);
	SetFontSize(size);
	for (int i = 1; i < IMAGE_LINE_NUM; i++)
	{
		UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size * i + 50 }, image_cledit[i], 0xffffff);
	}

	//�����f�ޕ\�L�̕������`��
	float shift = size * IMAGE_LINE_NUM + 200;
	SetFontSize(size * 1.5f);
	UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size + shift }, sound_cledit[0], 0xffffff);
	SetFontSize(size);
	for (int i = 1; i < SOUND_LINE_NUM; i++)
	{
		UserData::DrawStringCenter({ (float)SCREEN_WIDTH / 2,_loc_y + size * i + 50 + shift }, sound_cledit[i], 0xffffff);
	}
}