#pragma once
#include "../../Utility/common.h"
#include "../../Utility/Vector2D.h"
#include "../Tutorial.h"
#include "Dxlib.h"


class Camera
{
private:
	Vector2D camera_location;			//�J�������W
	Vector2D lock_pos[2];						//�J�����̌Œ�ʒu�i�l���j
	Vector2D stage_size;						//�X�e�[�W�̑傫��

public:
	Vector2D player_location;					//�v���C���[���W�i�[�p
	class Tutorial* tutorial;		//�`���[�g���A���i�[

private:
	//�R���X�g���N�^��private�ɂ��邱�ƂŁA
//���N���X�̃����o�֐��ŃC���X�^���X�𐶐��ł��Ȃ��悤�ɂ���
	Camera() = default;

	//�R�s�[�K�[�h
	//�N���X�O�ŃC���X�^���X�𐶐����ēn�����Ƃ��ł��Ȃ��悤�ɂ���
	Camera(Camera& v) = default;
	Camera& operator = (const Camera& v) = delete;
	~Camera() = default;
public:
	//�C���X�^���X���擾���鏈��
	static Camera* Get();
public:
	//�J�����X�V
	void Update();

	//�J�����X�V�i�J�������W�w��j
	void Update(Vector2D _loc);

	//�J�������W�擾
	Vector2D GetCameraLocation()const
	{
		return camera_location;
	}

	//�J�����Œ�ʒu�擾 _num 0�͍���̌Œ���W 1�͉E���̌Œ���W
	Vector2D GetLockPos(int _num)const
	{
		return lock_pos[_num];
	}
};
