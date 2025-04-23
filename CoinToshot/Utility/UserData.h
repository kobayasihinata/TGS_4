#pragma once
#include <vector>

#include "Vector2D.h"
#define RANKING_DATA_NUM 2		//�����L���O�f�[�^�̍��ڐ�
struct RankingData
{
	char* name;
	int coin;
};

struct ObjectData
{
	Vector2D loc;		//�ʒu
	Vector2D size;		//�傫��
	int type;			//�I�u�W�F�N�g�̎��
	float radius = 0.f;	//���a(�~��p)
};
class UserData
{

public:

	static std::vector<RankingData> ranking_data;	//�����L���O�f�[�^�i�[

	//UI�ɕ\������K�v������A�ێ����Ă����������ǂ��\��������v���C���[���͂����ŊǗ�
	static Vector2D spawn_loc[10];	//���X�|�[������ʒu
	static float player_hp;		//�̗�
	static int coin;			//�X�R�A
	static int timer;			//�c�莞��
	static bool is_clear;		//false=�Q�[���I�[�o�[ true=�Q�[���N���A
	static int invincible;		//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�

public:
	//�����L���O�f�[�^��ǂݍ���
	static void ReadRankingData();

	//�f�[�^����������
	static void WriteRankingData();

};

