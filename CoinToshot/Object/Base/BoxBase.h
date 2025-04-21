#pragma once
#include "../../Utility/Vector2D.h"

#define PUSH_SHIFT 0.6f		  //Push関数でオブジェクトを移動させた時に上乗せする数値
#define SURFACE_THICKNESS 15  //表面の厚さ

enum hitPosition
{
	top,
	bottom,
	left,
	right
};

class ShapeCollider
{
protected:
	Vector2D location;			//中心座標	
	Vector2D box_size;			//判定の大きさ
	Vector2D local_location;	//相対座標

public:

	void Initialize(Vector2D init_location = 0.0f, Vector2D init_size = 40.0f);		//初期化

	bool CheckHit(ShapeCollider* object2);			//当たり判定の確認(相手のポインタを渡すバージョン)
	bool CheckHit(Vector2D _loc2, Vector2D _size2);	//当たり判定の確認(相手の座標とサイズを渡すバージョン)

	void Push(ShapeCollider* hit_object);			//２つのオブジェクトの座標を受けとり、適切な移動処理を行う

	//座標の取得
	Vector2D GetLocation()const { return location; }
	//ローカル座標の取得
	Vector2D GetLocalLocation()const { return local_location; }
	//判定のサイズ取得
	Vector2D GetSize()const { return box_size; }

	//ローカル座標の設定
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
};


