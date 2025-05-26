#include "TitleScene.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "../Utility/common.h"

TitleScene::TitleScene()
{
	current_num = 0;

	font = CreateFontToHandle("���_�S�V�b�N", 40, 3, DX_FONTTYPE_ANTIALIASING);
}

TitleScene::~TitleScene()
{
	// �t�H���g�n���h�����폜
	DeleteFontToHandle(font);

}

void TitleScene::Initialize()
{
	__super::Initialize();

}

eSceneType TitleScene::Update(float _delta)
{
	//�����͂ō��ډ��ړ�
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::OnButton(L_STICK_DOWN))
	{
		if (++current_num >= ITEM_NUM)
		{
			current_num = 0;
		}
	}
	//����͂ō��ڏ�ړ�
	if (InputPad::OnButton(XINPUT_BUTTON_DPAD_UP) || InputPad::OnButton(L_STICK_UP))
	{
		if (--current_num < 0)
		{
			current_num = ITEM_NUM-1;
		}
	}
	//A�{�^���Ō���
	if (InputPad::OnButton(XINPUT_BUTTON_A))
	{
		switch (current_num)
		{
		case TitleItem::tGameMain:
			return eSceneType::eInGame;
			break;
		case TitleItem::tRanking:
			return eSceneType::eRanking;
			break;
		case TitleItem::tEnd:
			return eSceneType::eEnd;
			break;
		default:
			break;
		}
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

void TitleScene::Draw()const
{
	DrawString(10, 30, "Title�^�C�g��", 0xffffff);
	// �t�H���g�𗘗p���ĕ�����`��
	DrawStringToHandle(10, 10, "Title�^�C�g��", GetColor(255, 255, 255), font);
	int old = GetFontSize();

	SetFontSize(32);
	
	for (int i = 0; i < ITEM_NUM; i++)
	{
		//�J�[�\���ƍ��ڂ̕`��
		if (current_num == i)
		{
			DrawCircle(SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 + (i * 30)+15, 15, 0x00ff00, true);
			//���ڂ̕`��
			DrawFormatStringToHandle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * 30), 0x00ff00, font, "%s", item_text[i]);
		}
		//���ڂ݂̂̕`��
		else
		{
			//���ڂ̕`��
			DrawFormatStringToHandle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + (i * 30), 0xffffff, font, "%s", item_text[i]);
		}
	}

	SetFontSize(old);
}

void TitleScene::Finalize()
{

}

eSceneType TitleScene::GetNowSceneType()const
{
	return eSceneType::eTitle;
}
