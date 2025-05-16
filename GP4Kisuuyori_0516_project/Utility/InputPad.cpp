#include "InputPad.h"
#include "DebugInformation.h"

char InputPad::NowKey[BUTTONS];
char InputPad::OldKey[BUTTONS];
bool InputPad::NowStick[8] = { false };
bool InputPad::OldStick[8] = { false };
bool InputPad::NowTrigger[2];
bool InputPad::OldTrigger[2];
XINPUT_STATE InputPad::Input;
Stick InputPad::Rstick;
Stick InputPad::Lstick;

int InputPad::PressingTime[R_TRIGGER] = { 0 };

void InputPad::UpdateKey()
{
	// ���̓L�[�擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &Input);
	for (int i = 0; i < BUTTONS; i++)
	{
		OldKey[i] = NowKey[i];
		NowKey[i] = Input.Buttons[i];
	}

	//���X�e�B�b�N
	Lstick.ThumbX = Input.ThumbLX;
	Lstick.ThumbY = Input.ThumbLY;

	//�E�X�e�B�b�N
	Rstick.ThumbX = Input.ThumbRX;
	Rstick.ThumbY = Input.ThumbRY;

	//�g���K�[
	OldTrigger[0] = NowTrigger[0];
	NowTrigger[0] = Input.LeftTrigger > 127;

	OldTrigger[1] = NowTrigger[1];
	NowTrigger[1] = Input.RightTrigger > 127;

	//�X�e�B�b�N��bool�Ŕ��f���������X�V
	for (int i = 0; i < 8; i++)
	{
		OldStick[i] = NowStick[i];
		NowStick[i] = GetStick(i + L_STICK_UP);
	}

	//�e�{�^��������Ă�����^�C�}�[���Z�A������Ă��Ȃ����0��
	for (int i = 0; i < L_TRIGGER; i++)
	{
		if (OnPressed(i))PressingTime[i]++;
		else PressingTime[i] = 0;
	}
}

bool InputPad::OnButton(int button)
{
	//�g���K�[�͕ʏ���
	if (button >= L_TRIGGER)return (NowTrigger[button - L_TRIGGER] == true && OldTrigger[button - L_TRIGGER] == false);
	//�X�e�B�b�N�͕ʏ���
	if (button >= L_STICK_UP)return (NowStick[button - L_STICK_UP] == true && OldStick[button - L_STICK_UP] == false);
	//�{�^������
	bool ret = (NowKey[button] == true && OldKey[button] == false);
	return ret;
}

bool InputPad::OnPressed(int button)
{
	//�g���K�[�͕ʏ���
	if (button >= L_TRIGGER)return NowTrigger[button - L_TRIGGER] == true;
	//�X�e�B�b�N�͕ʏ���
	if (button >= L_STICK_UP)return NowStick[button - L_STICK_UP] == true;
	//�{�^������
	bool ret = (NowKey[button] == true);
	return ret;
}

bool InputPad::OnRelease(int button)
{
	//�g���K�[�͕ʏ���
	if (button >= L_TRIGGER)return (NowTrigger[button - L_TRIGGER] == false && OldTrigger[button - L_TRIGGER] == true);
	//�X�e�B�b�N�͕ʏ���
	if (button >= L_STICK_UP)return (NowStick[button - L_STICK_UP] == false && OldStick[button - L_STICK_UP] == true);
	//�{�^������
	bool ret = (NowKey[button] == false && OldKey[button] == true);
	return ret;
}

float InputPad::TipLStick(short StickL)
{
	if (StickL == STICKL_X)
	{
		float ratioL_X = Lstick.ThumbX / MAXL_X;

		//���X�e�B�b�N�̉������ő�l���P�Ƃ�������
		return ratioL_X;
	}
	else if (StickL == STICKL_Y)
	{
		float ratioL_Y = Lstick.ThumbY / MAXL_Y;

		//���X�e�B�b�N�̏c���l�̍ő�l���P�Ƃ�������
		return ratioL_Y;
	}

	return 0;
}

float InputPad::TipRStick(short StickR)
{
	if (StickR == STICKL_X)
	{
		float ratioR_X = Rstick.ThumbX / MAXL_X;
		//�E�X�e�B�b�N�̉������ő�l���P�Ƃ�������
		return ratioR_X;
	}
	else if (StickR == STICKL_Y)
	{
		float ratioR_Y = Rstick.ThumbY / MAXL_Y;
		//�E�X�e�B�b�N�̏c���l�̍ő�l���P�Ƃ�������
		return ratioR_Y;
	}

	return 0;
}

bool InputPad::GetStick(int _assign_key)
{
	//�m�肽���X�e�B�b�N�ɂ���ď�����ς���
	switch (_assign_key)
	{
		//���X�e�B�b�N��
	case L_STICK_UP:
		return InputPad::TipLStick(STICKL_Y) > 0.5f;
		break;
		//���X�e�B�b�N��
	case L_STICK_DOWN:
		return InputPad::TipLStick(STICKL_Y) < -0.5f;
		break;
		//���X�e�B�b�N�E
	case L_STICK_RIGHT:
		return InputPad::TipLStick(STICKL_X) > 0.5f;
		break;
		//���X�e�B�b�N��
	case L_STICK_LEFT:
		return InputPad::TipLStick(STICKL_X) < -0.5f;
		break;
		//�E�X�e�B�b�N��
	case R_STICK_UP:
		return InputPad::TipRStick(STICKL_Y) > 0.5f;
		break;
		//�E�X�e�B�b�N��
	case R_STICK_DOWN:
		return InputPad::TipRStick(STICKL_Y) < -0.5f;
		break;
		//�E�X�e�B�b�N�E
	case R_STICK_RIGHT:
		return InputPad::TipRStick(STICKL_X) > 0.5f;
		break;
		//�E�X�e�B�b�N��
	case R_STICK_LEFT:
		return InputPad::TipRStick(STICKL_X) < -0.5f;
		break;
	default:
		break;
	}
	return false;
}

bool InputPad::GetPressedButton(int _button, int _start_time, int _interval)
{
	//�w�肵���L�[������������Ă��鎞�Ԃ��A�J�n���Ԉȏォ���f
	if (PressingTime[_button] > _start_time)
	{
		//�J�n�����u�Ԃ�0�t���[���ڂƂ��āA����ȍ~�͎w�肵���Ԋu���ɐ^��Ԃ�
		return ((PressingTime[_button] - _start_time) % _interval == 0);
	}
	//�����ꂽ�u�Ԃ���񂾂��^
	if (PressingTime[_button] == 1)return true;
	return false;
}

int InputPad::GetNowInput()
{
	//�{�^���̓��͂𒲂ׂ�
	for (int i = 0; i < BUTTONS; i++)
	{
		if (NowKey[i] == 1)return i;
	}
	//�X�e�B�b�N�̓��͂𒲂ׂ�
	for (int j = 0; j < 8; j++)
	{
		if (NowStick[j] == 1)return j + L_STICK_UP;
	}
	//���E�g���K�[�̓��͂𒲂ׂ�
	for (int k = 0; k < 2; k++)
	{
		if (NowTrigger[k] == 1)return k + L_TRIGGER;
	}
	//�������͂���Ă��Ȃ�����Ԃ�
	return -1;
}