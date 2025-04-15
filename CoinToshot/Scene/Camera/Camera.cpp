#include "Camera.h"
#include "../../Utility/DebugInformation.h"

Camera* Camera::Get()
{

	static Camera instance;
	//自分自身のポインタを返却する
	return &instance;
}

void Camera::Update()
{
	DebugInfomation::Add("camera_location.x", camera_location.x);
	DebugInfomation::Add("camera_location.y", camera_location.y);
	//カメラ座標更新
	camera_location.x = player_location.x - (SCREEN_WIDTH / 2);
	camera_location.y = player_location.y - (SCREEN_HEIGHT / 2);

	//カメラX座標が画面左端以下なら
	if (camera_location.x <= lock_pos[0].x)
	{
		//カメラのX座標を左端固定する
		camera_location.x = lock_pos[0].x;
	}
	//カメラX座標が画面右端以上なら
	if (camera_location.x >= lock_pos[1].x)
	{
		//カメラのX座標を右端固定する
		camera_location.x = lock_pos[1].x;
	}
	//カメラY座標が画面上端以上なら
	if (camera_location.y <= lock_pos[0].y)
	{
		//カメラのX座標を上端固定する
		camera_location.y = lock_pos[0].y;
	}
	//カメラY座標が画面下端以上なら
	if (camera_location.y >= lock_pos[1].y)
	{
		//カメラのX座標を下端固定する
		camera_location.y = lock_pos[1].y;
	}
}

void Camera::Update(Vector2D _loc)
{
	camera_location = _loc;
}

void Camera::SetStageSize(Vector2D _size)
{
	stage_size = _size;
	//カメラ固定位置の再設定
	lock_pos[0].x = -STAGE_SIZE;
	lock_pos[0].y = -STAGE_SIZE;
	lock_pos[1].x = STAGE_SIZE;
	lock_pos[1].y = STAGE_SIZE;
}
