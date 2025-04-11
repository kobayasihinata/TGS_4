#pragma once
#include"DxLib.h"

#define BUTTONS 16
#define STICKL_X 1		
#define STICKL_Y 2		
#define MAXL_X 32767.f  //���X�e�B�b�NX���̍ő�l(float�^)
#define MAXL_Y 32767.f  //���X�e�B�b�NY���̍ő�l(float�^)

//�L�[���蓖�ėp
#define L_STICK_UP (16)			//���X�e�B�b�N�̏�
#define L_STICK_DOWN (17)		//���X�e�B�b�N�̉�
#define L_STICK_LEFT (18)		//���X�e�B�b�N�̍�
#define L_STICK_RIGHT (19)		//���X�e�B�b�N�̉E

#define R_STICK_UP (20)			//�E�X�e�B�b�N�̏�
#define R_STICK_DOWN (21)		//�E�X�e�B�b�N�̉�
#define R_STICK_LEFT (22)		//�E�X�e�B�b�N�̍�
#define R_STICK_RIGHT (23)		//�E�X�e�B�b�N�̉E

#define L_TRIGGER (24)			//���g���K�[
#define R_TRIGGER (25)			//�E�g���K�[

//�X�e�B�b�N
struct Stick
{
	short ThumbX;	//�����l
	short ThumbY;	//�c���l
};

class InputPad
{
private:
	static XINPUT_STATE Input; //�p�b�h
	static Stick Rstick; //�E�X�e�B�b�N
	static Stick Lstick; //���X�e�B�b�N
	static char NowKey[BUTTONS]; //����̓��̓L�[
	static char OldKey[BUTTONS]; //�P�t���[���O�̓��̓L�[
	static bool NowStick[8];	 //����̓��̓X�e�B�b�N
	static bool OldStick[8];	 //�P�t���[���O�̓��̓X�e�B�b�N
	static bool NowTrigger[2];	 //����̃g���K�[
	static bool OldTrigger[2];	 //�P�t���[���O�̃g���K�[

	//�e�{�^�����������ԑ���
	static int PressingTime[R_TRIGGER];
public:
	//�p�b�h���̍X�V
	static void UpdateKey();

	//�{�^���������ꂽ�u��
	static bool OnButton(int button);

	//�{�^���������Ă��
	static bool OnPressed(int button);

	//�{�^���𗣂����u��
	static bool OnRelease(int button);

	//�X�e�B�b�N�̌X�������i���j
	static float TipLStick(short StickL);

	//�X�e�B�b�N�̌X�������i�E�j
	static float TipRStick(short StickR);

	//�X�e�B�b�N�̌X����bool�ŕԂ�
	static bool GetStick(int _assign_key);

	//�w�肵���L�[���w��̎��Ԓ���������Ă�����A�w��̊Ԋu���ɐ^��Ԃ�(�������u�Ԃ���x�����^��Ԃ�)
	static bool GetPressedButton(int _button, int start_time = 10, int interval = 5);

	// ���݉�����Ă���{�^���A�X�e�B�b�N�̔ԍ���Ԃ�
	// �����ɓ��͂���Ă���ꍇ�͊e��{�^���A���X�e�B�b�N�㉺���E�A
	// �E�X�e�B�b�N�㉺���E�̏��ԂŎႢ�����̕����Ԃ����
	static int GetNowInput();
};