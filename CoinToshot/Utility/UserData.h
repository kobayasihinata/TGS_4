#pragma once
#include <vector>

#include "Vector2D.h"
#include "common.h"
#include "InputPad.h"

#include <string>
using namespace std;

struct RankingData
{
	int num;		//����
	string name;		//���O
	int coin;		//�R�C���̐�
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

	static RankingData ranking_data[RANKING_DATA];	//�����L���O�f�[�^�i�[

	//UI�ɕ\������K�v������ or �ێ����Ă����������ǂ��\��������v���C���[���͂����ŊǗ�
	static float player_hp;			//�̗�
	static int coin;				//�X�R�A
	static int timer;				//�c�莞��
	static bool is_clear;			//false=�Q�[���I�[�o�[ true=�Q�[���N���A
	static bool is_gamestop;		//false=�Q�[���� true=�Q�[���I��
	static int invincible;			//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�
	static int bullet_type;			//�I�𒆂̒e�̎��
	static bool attraction_flg;		//�R�C���z����
	static int attraction_timer;	//�R�C���z���񂹎��ԑ���

	//�f�o�b�O�p
	static int variable;			//�N�����ɍD���ɕύX�ł���l
	static bool variable_change;	//�l�ύX����
	static Vector2D variable_loc;	//�����\���p���W
public:
	//�����L���O�f�[�^��ǂݍ���
	static void ReadRankingData();

	//�f�[�^����������
	static void WriteRankingData();

	//�w�肵���ʒu�𒆐S�ɕ�����`�悷��
	static void DrawStringCenter(Vector2D _loc, const char* _text,int _color, int _font = 0);

	//�R�C���`��
	static void DrawCoin(Vector2D _loc, float _radius, int _r = 255, int _g = 255, int _b = 0, int _a = 255);

	//�ς̕ϐ��`��
	static void DrawVariable();
};

