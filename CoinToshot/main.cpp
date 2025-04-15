#include "DxLib.h"
#include "Scene/SceneManager.h"

#include "Utility/UserData.h"

//���C���֐�
int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	//���[�J���ϐ���`
	SceneManager* scene_manager = nullptr;
	int result = 0;

	//�K�v�f�[�^�ǂݍ���
	UserData::ReadData();

	try {
		//�I�u�W�F�N�g����
		scene_manager = new SceneManager();

		//����������
		scene_manager->Initialize();

		//�X�V����
		scene_manager->Update();

		//�I������������
		scene_manager->Finalize();
	}
	catch (const char* error_log)
	{
		//�G���[�����o��
		OutputDebugString(error_log);
		result = -1;
	}

	//�V�[���}�l�[�W���[�𐶐����Ă�����A�폜����
	if (scene_manager != nullptr)
	{
		delete scene_manager;
		scene_manager = nullptr;
	}

	//�I����Ԃ̒l��ԋp����
	return result;
}