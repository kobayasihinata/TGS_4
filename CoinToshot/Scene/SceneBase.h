#pragma once

enum eSceneType
{
	eTitle,
	eInGame,
	eRanking,
	eResult,
};

class SceneBase
{
protected:
	float frame=0;		//�t���[������

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
		if (++frame > 3600)
		{
			frame = 0;
		}
		return GetNowSceneType();
	}

	//�`�揈��
	virtual void Draw()const {}

	//�I��������
	virtual void Finalize() {}

	//���݂̃V�[���^�C�v(�I�[�o�[���C�h�K�{)
	virtual eSceneType GetNowSceneType()const = 0;
};
