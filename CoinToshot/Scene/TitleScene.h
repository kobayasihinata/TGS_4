#pragma once

#include "SceneBase.h"

#define ITEM_NUM 3	//�^�C�g���̍��ڐ�

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
	int current_num;	//�I������Ă��鍀��

	int title_font;			//�^�C�g���̃n���h��
	int menu_font;			//���j���[���ڂ̃n���h��
	int bg_image;		//�w�i�摜�i�[

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

