#pragma once
#include "SceneBase.h"

static int rank_color[10]
{
	0xffff44,
	0xdddd44,
	0xbbbb77,
	0x666644,
	0x666644,
	0x666644,
	0x666644,
	0x666644,
	0x666644,
	0x666644,
};
class RankingScene : public SceneBase
{
private:
	int ranking_bgm;//BGM
	int button_se;	//�߂�{�^�����ʉ�
public:
	RankingScene();
	virtual ~RankingScene();

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
};



