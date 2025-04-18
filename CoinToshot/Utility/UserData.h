#pragma once
#include <vector>

#include "Vector2D.h"

//�I�u�W�F�N�g�̏��
struct ObjectData
{
	Vector2D loc;	//���W
	Vector2D size;	//�傫��
	int type;		//���
	float radius = 0.f;	//���a(�l�p�͎g��Ȃ�)
};
class UserData
{
private:
	static ObjectData old_data;			//�ЂƂO�̃I�u�W�F�N�g���i�[�p
public:
	static int width_num[10];						//�X�e�[�W�̕�
	static int height_num[10];						//�X�e�[�W�̍���
	static std::vector<ObjectData> stage_data[10];	//�X�e�[�W�̃I�u�W�F�N�g���
	static int stage_num;						//�X�e�[�W�̐�
	static int now_stage;									//���݂̃X�e�[�W��

	//UI�ɕ\������K�v������A�ێ����Ă����������ǂ��\��������v���C���[���͂����ŊǗ�
	static Vector2D spawn_loc[10];	//���X�|�[������ʒu
	static float player_hp;		//�̗�
	static int coin;			//�X�R�A
	static int timer;			//�c�莞��
	static bool is_clear;		//false=�Q�[���I�[�o�[ true=�Q�[���N���A
	static int invincible;		//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�

public:
	//�e��f�[�^��ǂݍ���
	static void ReadData();

	//�f�[�^����������
	static void WriteData();

};

