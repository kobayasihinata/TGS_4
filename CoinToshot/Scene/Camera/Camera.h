#pragma once
#include "../../Utility/common.h"
#include "../../Utility/Vector2D.h"
#include "Dxlib.h"

#define X_SHIFT_LIMIT 500.0f	//カメラのX可動域（半径）
#define Y_SHIFT_LIMIT 300.0f	//カメラのY可動域（半径）

class Camera
{
private:
	Vector2D camera_location = { 0,0 };			//カメラ座標
	Vector2D lock_pos[2];						//カメラの固定位置（四隅）
	Vector2D stage_size;						//ステージの大きさ

public:
	Vector2D player_location;					//プレイヤー座標格納用

private:
	//コンストラクタをprivateにすることで、
//自クラスのメンバ関数でインスタンスを生成できないようにする
	Camera() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	Camera(Camera& v) = default;
	Camera& operator = (const Camera& v) = delete;
	~Camera() = default;
public:
	//インスタンスを取得する処理
	static Camera* Get();
public:
	//カメラ更新
	void Update();

	//カメラ更新（カメラ座標指定）
	void Update(Vector2D _loc);

	//ステージの縦幅と横幅を格納する
	void SetStageSize(Vector2D _size);

	//カメラ座標取得
	Vector2D GetCameraLocation()const
	{
		return camera_location;
	}

	//カメラ固定位置取得 _num 0は左上の固定座標 1は右下の固定座標
	Vector2D GetLockPos(int _num)const
	{
		return lock_pos[_num];
	}
};
