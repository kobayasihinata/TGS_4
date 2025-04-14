#include "Camera.h"
#include "../../Utility/DebugInformation.h"

Camera* Camera::Get()
{

	static Camera instance;
	//�������g�̃|�C���^��ԋp����
	return &instance;
}

void Camera::Update()
{
	DebugInfomation::Add("camera_location.x", camera_location.x);
	DebugInfomation::Add("camera_location.y", camera_location.y);
	//�J�������W�X�V
	camera_location.x = player_location.x - (SCREEN_WIDTH / 2);
	camera_location.y = player_location.y - (SCREEN_HEIGHT / 2);

	//�J����X���W����ʍ��[�ȉ��Ȃ�
	if (camera_location.x <= lock_pos[0].x)
	{
		//�J������X���W�����[�Œ肷��
		camera_location.x = lock_pos[0].x;
	}
	//�J����X���W����ʉE�[�ȏ�Ȃ�
	if (camera_location.x >= lock_pos[1].x)
	{
		//�J������X���W���E�[�Œ肷��
		camera_location.x = lock_pos[1].x;
	}
	//�J����Y���W����ʏ�[�ȏ�Ȃ�
	if (camera_location.y <= lock_pos[0].y)
	{
		//�J������X���W����[�Œ肷��
		camera_location.y = lock_pos[0].y;
	}
	//�J����Y���W����ʉ��[�ȏ�Ȃ�
	if (camera_location.y >= lock_pos[1].y)
	{
		//�J������X���W�����[�Œ肷��
		camera_location.y = lock_pos[1].y;
	}
}

void Camera::Update(Vector2D _loc)
{
	camera_location = _loc;
}

Vector2D Camera::GetCameraLocation()const
{
	return camera_location;
}

void Camera::SetStageSize(Vector2D _size)
{
	stage_size = _size;
	//�J�����Œ�ʒu�̍Đݒ�
	lock_pos[0].x = 0;
	lock_pos[0].y = 0;
	lock_pos[1].x = stage_size.x - SCREEN_WIDTH;
	lock_pos[1].y = stage_size.y - SCREEN_HEIGHT;
}

Vector2D Camera::GetLockPos(int _num)
{
	return lock_pos[_num];
}