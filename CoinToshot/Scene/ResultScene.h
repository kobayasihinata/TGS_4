#pragma once
#include "SceneBase.h"
#include <string>
using namespace std;

#define KEY_WIDTH 10	//���O���̓L�[�{�[�h���v�f��
#define KEY_HEIGHT 4	//���O���̓L�[�{�[�h�c�v�f��

//���͂ł��镶�������Ƃ��̔z�u
static char key[KEY_HEIGHT][KEY_WIDTH]{
	{'0','1','2','3','4','5','6','7','8','9',},
	{'a','b','c','d','e','f','g','h','i','j',},
	{'k','l','m','n','o','p','q','r','s','t',},
	{'u','v','w','x','y','z',' ',' ',' ',' ',},
};

enum DispScene
{
	dIsClear = 0,
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//�\�����Ă�����

	//���O���͊֘A
	int current_x;	//�J�[�\��X�ʒu
	int current_y;	//�J�[�\��Y�ʒu
	string name;	//���O�i�[

	int result_bgm;				//���U���gBGM

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

	//���O���͎���Draw
	void EnterNameDraw()const;

	//�����L���O���בւ�
	void SortRanking();
};



