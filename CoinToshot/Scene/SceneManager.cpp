#include "SceneManager.h"
#include "../Utility/common.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "Factory/SceneFactory.h"
#include "../Utility/DebugInformation.h"
#include "../Utility/FpsController.h"

SceneManager::SceneManager() : current_scene(nullptr), is_finalize(false)
{}

SceneManager::~SceneManager()
{
	//����Y��h�~
	Finalize();
}

/// <summary>
/// ����������
/// </summary>
void SceneManager::Initialize()
{
	//�E�B���h�E���[�h�ŋO��
	ChangeWindowMode(TRUE);

	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//DX���C�u�����̏�����
	if (DxLib_Init() == -1)
	{
		throw("DX���C�u�������������o���܂���ł���\n");
	}

	//����ʂ���`����n�߂�
	SetDrawScreen(DX_SCREEN_BACK);

	//�^�C�g����ʂ���V�[�����J�n����
	ChangeScene(eSceneType::eTitle);

	// �E�B���h�E�YPC�Ɉꎞ�I�Ƀt�H���g�f�[�^��ǂݍ���(�V�X�e���I���܂�)
	// NOTE: .ttf���t�H���g�f�[�^�i�g�D���[�^�C�v�t�H���g�̗��j
	AddFontResourceExA("Resource/Font/GenJyuuGothic-Medium.ttf", FR_PRIVATE, NULL);
}

/// <summary>
/// ���s����
/// </summary>
void SceneManager::Update()
{
	//�J�n���Ԃ��擾
	LONGLONG start_time = GetNowHiPerformanceCount();

	//���͋@�\�̃C���X�^���X�擾���鏈��
	InputKey* input = InputKey::Get();

	//fps����
	FpsController* FPSC = new FpsController(60, 800);

	//���C�����[�v�i�E�B���h�E�������� or ESC�L�[�����͂��ꂽ�@�Ń��[�v���I������j
	while (ProcessMessage() != -1)
	{
		//1�t���[��������̎��Ԃ��v�Z����
		float delta_second = static_cast<float>(GetNowHiPerformanceCount() - start_time) * 0.000001f;

		//���͋@�\�̍X�V
		input->Update();
		InputPad::UpdateKey();

		//�f�o�b�O�\���̍X�V
		DebugInfomation::Update();

		FPSC->All();
#ifdef _DEBUG
		FPSC->Disp();
#endif

		//�V�[���̍X�V����
		eSceneType next_scene_type = current_scene->Update(delta_second);

		//eNull�Ȃ�I��
		if (next_scene_type == eSceneType::eNull)
		{
			break;
		}

		//�V�[���̕`�揈��
		Draw();

		//�V�[���؂�ւ�
		if (next_scene_type != current_scene->GetNowSceneType())
		{
			ChangeScene(next_scene_type);
		}

		//�����I���{�^��
		if (input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::Pressed)
		{
			break;
		}
	}
}

/// <summary>
/// �I��������
/// </summary>
void SceneManager::Finalize()
{
	if (is_finalize)
	{
		return;
	}

	//DX���C�u�����̏I������
	DxLib_End();

	//�I����������
	is_finalize = true;

	// �E�B���h�E�Y�Ɉꎞ�I�ɕێ����Ă����t�H���g�f�[�^���폜
	RemoveFontResourceExA("", FR_PRIVATE, NULL);
}

void SceneManager::Draw()const
{
	//��ʂ̏�����
	ClearDrawScreen();

	//�V�[���̕`�揈��
	current_scene->Draw();

	//�f�o�b�O�\���̕`��
	DebugInfomation::Draw();

	//����ʂ̓��e��\��ʂɔ��f����
	ScreenFlip();
}


void SceneManager::ChangeScene(eSceneType type)
{
	//�����œn���ꂽ��񂩂�V�����V�[�����쐬����
	SceneBase* new_scene = SceneFactory::CreateScene(type);

	//�G���[�`�F�b�N
	if (new_scene == nullptr)
	{
		throw("�V�[���������ł��܂���ł���");
	}

	//���݃V�[���̏I������
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	//�V�����V�[���̏�����
	new_scene->Initialize();
	current_scene = new_scene;
}