#pragma once
#include "SceneBase.h"

enum DispScene
{
	dIsClear = 0,
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//�\�����Ă�����
	char name[10];		//���O���͗p

public:
	ResultScene();
	virtual ~ResultScene();

	//����������
	virtual void Initialize()override;

	//�X�V����
	//�����F�P�t���[��������̎���
	//�߂�l�F���̃V�[���^�C�v
	virtual eSceneType Update(float _delta)override;

	//�`�揈��
	virtual void Draw()const override;

	//�I��������
	virtual void Finalize() override;

	//���݂̃V�[���^�C�v(�I�[�o�[���C�h�K�{)
	virtual eSceneType GetNowSceneType()const override;

	//���O����
	eSceneType EnterName();

	//�����L���O���בւ�
	void SortRanking();
};



