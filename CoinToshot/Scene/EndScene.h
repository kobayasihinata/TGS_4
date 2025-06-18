#pragma once

#include "SceneBase.h"
#define END_TIMER 1200	//���b�ŃQ�[�����I�����邩
#define IMAGE_LINE_NUM 11
#define SOUND_LINE_NUM 10

//�\������
static char image_cledit[IMAGE_LINE_NUM][256]
{
	"�摜�f��",
	"�v���C���[�A�G�摜�Fcraftpix.net"
	"Free Golem Chibi 2D Game Sprites",
	" ",
	"Free Chibi Skeleton Crusader Character Sprites",
	" ",
	"Free Chibi Dark Oracle Character Sprites",
	" ",
	"Free Simple Platformer Game Kit Pixel Art",
	" ",
	"�w�i�f�� : ichi.io(https://itch.io/)",
	"Basic Tileset and Asset Pack 32x32 Pixels",
};
static char sound_cledit[SOUND_LINE_NUM][256]
{
	"�����f��",
	"BGM : DOVA-SYNDROME  (https://dova-s.jp/)",
	" ",
	"�^�C�g�� & �N���W�b�gBGM  : �`�ŁuColorful Cheerful Jelly Beans�v",
	"�Q�[����BGM  : Tinymemory�uRail_train�v",
	"���U���g & �����L���OBGM : Tinymemory�uSweet smelling flower�v",
	" ",
	"���ʉ�: OtoLogic (https://otologic.jp/)",
	"�|�P�b�g�T�E���h (https://pocket-se.info/)",
	"���ʉ����{ (https://soundeffect-lab.info/)",
};
class EndScene : public SceneBase
{
private:

public:
	EndScene();
	virtual ~EndScene();

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

	//�N���W�b�g�`��
	void DrawCledit(float _loc_y)const;
};

