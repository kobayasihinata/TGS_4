#pragma once

#define FRAME_RATE 60			//�t���[�����[�g

#define SCREEN_WIDTH  1280		//��ʉ���
#define SCREEN_HEIGHT 720		//��ʍ���

#define STAGE_SIZE 2000			//�X�e�[�W�̑傫���i���S����㉺���E��2000px����)

#define DEFAULT_TIMELIMIT 18000	//�������ԏ����l(�t���[���P��)

#define F_PI (float)(3.1415)				//�~����

#define DEFAULT_ANIM_SPAN 10	//�A�j���[�V��������

#define MOVE_LOWER_LIMIT 0.3f	//��~�Ƃ݂Ȃ�velocity�̉���

#define RANKING_DATA 10			//�o�^���郉���L���O�f�[�^�̐�

//�v���C���[�̃p�����[�^�֘A
#define DEFAULT_HP 2			//�v���C���[����HP
#define DEFAULT_LIFE 0			//�v���C���[�����c�@
#define PLAYER_WIDTH 40			//�v���C���[��
#define PLAYER_HEIGHT 40		//�v���C���[����
#define PLAYER_SPEED 4.f		//�v���C���[�ړ����x
#define PLAYER_DAMAGE_CD 120	//�v���C���[��e�㖳�G���ԁi�t���[���P�ʁj
#define PLAYER_DAMAGE_STOP 30	//�v���C���[��e��ړ��s�\���ԁi�t���[���P�ʁj

//�G�̃p�����[�^�֘A
#define ENEMY1_HP 2				//�G�PHP
#define ENEMY1_SPEED 1			//�G�P���x
#define ENEMY1_DAMAGE 1			//�G�P�^�_��
#define ENEMY1_DROPCOIN 5		//�G�P�R�C���h���b�v��

#define ENEMY2_HP 5				//�G�QHP
#define ENEMY2_SPEED 2			//�G�Q���x
#define ENEMY2_DAMAGE 3			//�G�Q�^�_��
#define ENEMY2_DROPCOIN 10		//�G�Q�R�C���h���b�v��

#define ENEMY3_HP 3				//�G�RHP
#define ENEMY3_SPEED 0			//�G�R���x
#define ENEMY3_DAMAGE 3			//�G�R�^�_��
#define ENEMY3_DROPCOIN 7		//�G�R�R�C���h���b�v��

#define ENEMY4_HP 3				//�G�SHP
#define ENEMY4_SPEED 5			//�G�S���x
#define ENEMY4_DAMAGE 0			//�G�S�^�_��
#define ENEMY4_DROPCOIN 5		//�G�S�R�C���h���b�v��

#define ENEMY5_HP 10			//�G�THP
#define ENEMY5_SPEED 1			//�G�T���x
#define ENEMY5_DAMAGE 2			//�G�T�^�_��
#define ENEMY5_DROPCOIN 20		//�G�T�R�C���h���b�v��