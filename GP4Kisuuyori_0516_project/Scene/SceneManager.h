#pragma once

#include "SceneBase.h"

class SceneManager
{
private:
	SceneBase* current_scene;	//���݃V�[�����̃|�C���^
	bool is_finalize;

public:
	SceneManager();
	~SceneManager();

	//����������
	void Initialize();

	//�X�V����
	void Update();

	//�I��������
	void Finalize();

private:
	//�`�揈��
	void Draw()const;

	//�V�[���؂�ւ�����
	void ChangeScene(eSceneType type);
};



