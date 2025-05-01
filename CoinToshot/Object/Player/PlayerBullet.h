#pragma once

#define BULLET_NUM 3	//�e�̎��

//�v���C���[�̒e�ɕK�v�ȏ��
struct PlayerBullet
{
	int cost;			//���˂ɂ�����R�C����
	float damage;		//�_���[�W��
	float speed;		//�e��
	float radius;		//�e�̔��a
	int h_count;		//�G�����̊ђʂ��邩
	int life_span;		//�e�̐�������
};

//�e�̃f�[�^
static PlayerBullet pBullet[BULLET_NUM]
{
	//cos   dam  spe   rad   h_c  life
	{  1,	1.f, 10.f, 10.f,   1, 120},
	{ 10,   7.f, 15.f, 15.f,   1, 180},
	{100, 100.f, 20.f, 25.f, 100, 360}
};
