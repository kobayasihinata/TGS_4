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
	if (camera_location.x <= -STAGE_SIZE)
	{
		//�J������X���W�����[�Œ肷��
		camera_location.x = -STAGE_SIZE;
	}
	//�J����X���W����ʉE�[�ȏ�Ȃ�
	if (camera_location.x >= STAGE_SIZE - SCREEN_WIDTH)
	{
		//�J������X���W���E�[�Œ肷��
		camera_location.x = STAGE_SIZE - SCREEN_WIDTH;
	}
	//�J����Y���W����ʏ�[�ȏ�Ȃ�
	if (camera_location.y <= -STAGE_SIZE)
	{
		//�J������X���W����[�Œ肷��
		camera_location.y = -STAGE_SIZE;
	}
	//�J����Y���W����ʉ��[�ȏ�Ȃ�
	if (camera_location.y >= STAGE_SIZE - SCREEN_HEIGHT)
	{
		//�J������X���W�����[�Œ肷��
		camera_location.y = STAGE_SIZE - SCREEN_HEIGHT;
	}
}

void Camera::Update(Vector2D _loc)
{
	camera_location = _loc;
}