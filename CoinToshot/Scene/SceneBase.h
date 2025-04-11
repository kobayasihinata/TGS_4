#pragma once

enum eSceneType
{
	eTitle,
	eInGame,
	eResult,
	eEdit
};

class SceneBase
{

public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//����������
	virtual void Initialize(){}

	//�X�V����
	//�����F�P�t���[��������̎���
	//�߂�l�F���̃V�[���^�C�v
	virtual eSceneType Update(float _delta)
	{
		return GetNowSceneType();
	}

	//�`�揈��
	virtual void Draw()const {}

	//�I��������
	virtual void Finalize() {}

	//���݂̃V�[���^�C�v(�I�[�o�[���C�h�K�{)
	virtual eSceneType GetNowSceneType()const = 0;
};
