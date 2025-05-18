#pragma once

#define BULLET_NUM 4	//�e�̎��

//�v���C���[�̒e�ɕK�v�ȏ��
struct PlayerBullet
{
	char name[10];		//�e�̖���
	int cost;			//���˂ɂ�����R�C����
	float damage;		//�_���[�W��
	float speed;		//�e��
	float radius;		//�e�̔��a
	int h_count;		//�G�����̊ђʂ��邩
	int life_span;		//�e�̐�������

	int bullet_num;		//�e���������˂��邩
	float space;		//�ǂꂭ�炢�̊Ԋu���J���Č���
};

//�e�̃f�[�^
static PlayerBullet pBullet[BULLET_NUM]
{
	//name    cos   dam  spe   rad   h_c  life num space
	{"�ʏ�e",  1,	 1.f, 10.f, 20.f,   1, 120,1,    0},
	{"�����e", 10,   7.f, 15.f, 30.f,   5, 180,1,    0},
	{"�ŋ��e", 50,  50.f, 25.f, 40.f, 100, 360,1,    0},
	{"�g�U�e", 5,   2.f, 10.f, 20.f,   1,   30,5, 0.2f},
};
