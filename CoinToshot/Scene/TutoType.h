#pragma once

static enum TutoType
{
	tRule=0,		//���[������
	tMove,			//�ړ����@����
	tAim,			//�Ə��J�[�\������
	tAttack,		//�U������
	tBulletChange,	//�e�ύX����
	tEnemyBullet,	//�G�̍U������
	tSlot,			//�X���b�g����
	tNone			//�`���[�g���A�������Ă��Ȃ����p
};

#define TUTO_NUM 8