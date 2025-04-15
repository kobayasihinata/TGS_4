#pragma once
#include "../../Utility/Vector2D.h"

#define PUSH_SHIFT 0.6f		  //Push関数でオブジェクトを移動させた時に上乗せする数値
#define SURFACE_THICKNESS 15  //表面の厚さ

class ObjectBase;
class ActorBase;

enum hitPosition
{
	top,
	bottom,
	left,
	right
};

class ShapeCollider
{
private:
	Vector2D location;			//絶対座標	
	Vector2D box_size;			//判定の大きさ
	
public:
	Vector2D local_location;	//相対座標

public:

	void Initialize();		//初期化

	bool CheckHit(ShapeCollider* object1, ShapeCollider* object2);			//当たり判定の確認
	bool CheckHit(Vector2D _loc, Vector2D _size, ShapeCollider* object2);	//当たり判定の確認(自身の座標とサイズを渡すバージョン)
	bool CheckHit(Vector2D _loc1, Vector2D _size1, Vector2D _loc2, Vector2D _size2);	//当たり判定の確認(自身と相手の座標とサイズを渡すバージョン)

	//座標の取得
	Vector2D GetLocation()const { return location; }
	//ローカル座標の取得
	Vector2D GetLocalLocation()const { return local_location; }
	//判定のサイズ取得
	Vector2D GetSize()const { return box_size; }
};


