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
		//A�{�^�����������Ƃ�
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//�����L���O�ŉ��ʂ̃X�R�A�������Ă�����
			if (UserData::coin > UserData::ranking_data[9].coin)
			{
				//���O���͂ɑJ��
				now_disp = DispScene::dEnterName;
			}
			//�����N�O�Ȃ烉���L���O�\���V�[���ɔ��
			else
			{
				return eSceneType::eRanking;
			}
		}
		break;
	case DispScene::dEnterName:
		//���O����
		return EnterName();
		break;
	default:
		break;
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

void ResultScene::Draw()const
{
	DrawString(10, 10, "Result", GetColor(255, 255, 255));

	switch (now_disp)
	{
	case DispScene::dIsClear:
		DrawString(10, 30, "Pad:A = Next", GetColor(255, 255, 255));
		//�N���A���N���A����Ȃ����\��
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
		DrawString(10, 30, "Pad:A = End", GetColor(255, 255, 255));
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
	//���O���͂͌�ō��܂�

	//A�{�^���œ��͊����Ƃ��ă����L���O����
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		char a[11] = "aaa";
		UserData::ranking_data[9].coin = UserData::coin;
		for (int i = 0; i < 11; i++)
		{
			UserData::ranking_data[9].name[i] = a[i];
		}
		SortRanking();
		UserData::WriteRankingData();
		return eSceneType::eRanking;
	}

	return GetNowSceneType();
}

void ResultScene::SortRanking()
{
	RankingData work;	//���ёւ��p�̕ϐ�

	// �I��@�\�[�g
	for (int i = 0; i < RANKING_DATA - 1; i++) {
		for (int j = i + 1; j < RANKING_DATA; j++) {
			if (UserData::ranking_data[i].coin <= UserData::ranking_data[j].coin) {
				work = UserData::ranking_data[i];
				UserData::ranking_data[i] = UserData::ranking_data[j];
				UserData::ranking_data[j] = work;
			}
		}
	}

	// ���ʕt��
	for (int i = 0; i < RANKING_DATA; i++) {
		UserData::ranking_data[i].num = 1;
	}

	// ���_�������ꍇ�́A�������ʂƂ���
	// �����ʂ��������ꍇ�̎��̏��ʂ̓f�[�^�������Z���ꂽ���ʂƂ���
	for (int i = 0; i < RANKING_DATA - 1; i++) {
		for (int j = i + 1; j < RANKING_DATA; j++) {
			if (UserData::ranking_data[i].coin > UserData::ranking_data[j].coin) {
				UserData::ranking_data[j].num++;
			}
		}
	}
}
