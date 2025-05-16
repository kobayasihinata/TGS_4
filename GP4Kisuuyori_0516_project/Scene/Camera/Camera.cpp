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
	if (camera_location.x <= -STAGE_SIZE)
	{
		//カメラのX座標を左端固定する
		camera_location.x = -STAGE_SIZE;
	}
	//カメラX座標が画面右端以上なら
	if (camera_location.x >= STAGE_SIZE - SCREEN_WIDTH)
	{
		//カメラのX座標を右端固定する
		camera_location.x = STAGE_SIZE - SCREEN_WIDTH;
	}
	//カメラY座標が画面上端以上なら
	if (camera_location.y <= -STAGE_SIZE)
	{
		//カメラのX座標を上端固定する
		camera_location.y = -STAGE_SIZE;
	}
	//カメラY座標が画面下端以上なら
	if (camera_location.y >= STAGE_SIZE - SCREEN_HEIGHT)
	{
		//カメラのX座標を下端固定する
		camera_location.y = STAGE_SIZE - SCREEN_HEIGHT;
	}
}

void Camera::Update(Vector2D _loc)
{
	camera_location = _loc;
}