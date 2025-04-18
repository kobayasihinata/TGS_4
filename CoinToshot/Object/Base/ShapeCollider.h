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
	//四角形用の変数
	Vector2D box_size;			//判定の大きさ

	////三角形用の変数
	//Vector2D vert_location[3];	//頂点座標
	//Vector2D vert_local_location[3];	//頂点座標

	//円用の変数
	float radius;				//半径

protected:
	Vector2D location;			//絶対中心座標	
	Vector2D local_location;	//相対中心座標

public:

	void Initialize(Vector2D init_location = 0.0f, Vector2D init_size = 40.0f,float init_radius = 0.f);		//初期化(init_radiusが0なら四角形として扱う)

	bool CheckHit(ShapeCollider* object);			//当たり判定の確認(相手のポインタを渡すバージョン)

	//bool CheckHit(Vector2D _loc2, float _radius);	//当たり判定の確認(相手の座標と半径を渡すバージョン)

	bool CollisionCheckBtoB(Vector2D _loc, Vector2D _size)const;	//箱同士の当たり判定 _loc,_size = 相手の座標と大きさ
	bool CollisionCheckBtoC(Vector2D _loc1, Vector2D _size1,Vector2D _loc2, float _radius2)const;	//箱と円の当たり判定 _loc,_radius = 相手の座標と半径
	bool CollisionCheckCtoC(Vector2D _loc, float _radius)const;	//円同士の当たり判定 _loc,_radius = 相手の座標と半径

	float DistanceSqrf(const float t_x1, const float t_y1, const float t_x2, const float t_y2)const;		//二点の距離を求める

	void Push(ShapeCollider* hit_object);			//２つのオブジェクトの座標を受けとり、適切な移動処理を行う

	//座標の取得
	Vector2D GetLocation()const { return location; }
	//ローカル座標の取得
	Vector2D GetLocalLocation()const { return local_location; }

	//判定のサイズ取得(四角形)
	Vector2D GetSize()const { return box_size; }
	//半径取得(円)
	float GetRadius()const { return radius; }

	//ローカル座標の設定
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
};


