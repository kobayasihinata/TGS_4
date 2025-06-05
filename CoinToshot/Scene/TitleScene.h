#pragma once

#include "SceneBase.h"

#define ITEM_NUM 3	//�^�C�g���̍��ڐ�
#define START_ANIM 180  //�A�j���[�V��������

enum TitleItem
{
	tGameMain = 0,
	tRanking,
	tEnd,
};

static char item_text[ITEM_NUM][256] =
{
	"Start",
	"Ranking",
	"End"
};
class TitleScene : public SceneBase
{
private:
	bool start_anim_flg;	//�^�C�g���J�ڎ��A�j���[�V����
	int start_anim_timer;	//�^�C�g���J�ڎ��A�j���[�V��������

	int current_num;		//�I������Ă��鍀��

	int bg_image;			//�w�i�摜�i�[

	int cursor_se;				//�J�[�\��SE
	int enter_se;				//����SE
public:
	TitleScene();
	virtual ~TitleScene();

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

	//�w�i�摜����
	int CreateBackGround();
};

