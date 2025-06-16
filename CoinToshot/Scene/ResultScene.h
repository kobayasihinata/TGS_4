#pragma once
#include "SceneBase.h"
#include <string>
using namespace std;

#define KEY_WIDTH 10	//���O���̓L�[�{�[�h���v�f��
#define KEY_HEIGHT 4	//���O���̓L�[�{�[�h�c�v�f��

#define START_ANIM_TIME 180		//�J�ډ��o����	
#define RESULT_ANIM_TIME 300	//���U���g�\�����o����
#define BONUS_ANIM_TIME 300		//�{�[�i�X�_�����Z���o����

#define SKIP_TIME 30		//�c�艽�t���[���܂ŃX�L�b�v�o���邩

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
	dDispResult,
	dBonusPoint,
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//�\�����Ă�����


	int start_anim_timer;	//�J�ډ��o���ԑ���
	int result_anim_timer;	//���U���g�\�����o���ԑ���
	int bonus_anim_timer;	//�{�[�i�X�_�����Z���o���ԑ���

	int add_anim_num;		//���Z�A�j���[�V�����p1�t���[���ӂ�̉��Z����
	int add_anim_coin;		//���Z�A�j���[�V�����p�R�C������

	bool add_coin_once;		//�{�[�i�X���Z����񂾂��s��

	//���O���͊֘A
	int current_x;		//�J�[�\��X�ʒu
	int current_y;		//�J�[�\��Y�ʒu
	string name;		//���O�i�[

	bool disp_se_once;	//���ڕ\��SE�Đ�

	int result_bgm;		//���U���gBGM
	int button_se;		//�{�^��SE
	int erase_se;		//�����폜SE
	int cursor_se;		//�J�[�\��SE
	int disp_se;		//���ڕ\��SE
	int coin_se;		//�R�C�����ZSE
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



