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
private:
	static int frame;			//�t���[������
	static int now_button;		//�{�^���A�j���[�V�����p
public:

	static RankingData ranking_data[RANKING_DATA];	//�����L���O�f�[�^�i�[
	static RankingData my_ranking_data;				//�O��v���C�̃f�[�^

	//UI�ɕ\������K�v������ or �ێ����Ă����������ǂ��\��������v���C���[���͂����ŊǗ�
	static float player_hp;			//�̗�
	static int coin;				//�X�R�A
	static int timer;				//�c�莞��
	static bool is_clear;			//false=�Q�[���I�[�o�[ true=�Q�[���N���A
	static bool is_dead;			//�v���C���[�����S���o����
	static bool is_gamestop;		//false=�Q�[���� true=�Q�[���I��
	static int invincible;			//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�
	static int bullet_type;			//�I�𒆂̒e�̎��
	static bool attraction_flg;		//�R�C���z����
	static int attraction_timer;	//�R�C���z���񂹎��ԑ���
	static bool can_bullet_change_flg;	//�e��ޕύX�\��
	static bool player_damage_flg;	//�v���C���[���_���[�W���󂯂ē����Ȃ���Ԃ����f
	static std::vector<std::vector<int>> button_image;//�{�^���摜�i�[

	//�f�o�b�O�p
	static int variable;			//�N�����ɍD���ɕύX�ł���l
	static bool variable_change;	//�l�ύX����
	static Vector2D variable_loc;	//�����\���p���W
public:
	//�{�^���摜��ǂݍ���
	static void LoadButtonImage();

	//�{�^���A�j���[�V�����p�X�V
	static void Update();

	//�����L���O�f�[�^��ǂݍ���
	static void ReadRankingData();

	//�f�[�^����������
	static void WriteRankingData();

	//�{�^���摜��`��
	static void DrawButtonImage(Vector2D _loc, int _button,int _size);

	//�{�^���ƕ�����`��
	static void DrawButtonAndString(Vector2D _loc, int _button, const char* _text, int _color);

	//�w�肵���ʒu�𒆐S�ɕ�����`�悷��
	static void DrawStringCenter(Vector2D _loc, const char* _text,int _color, int _font = 0);

	//�R�C���`��
	static void DrawCoin(Vector2D _loc, float _radius, int _r = 255, int _g = 255, int _b = 0);
		
	//�ς̕ϐ��`��
	static void DrawVariable();
};

