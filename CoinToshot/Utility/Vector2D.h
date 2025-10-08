#pragma once
#include "DxLib.h"

class Vector2D
{
public:
	float x;	//x軸
	float y;	//y軸

public:
	Vector2D();			//コンストラクタ
	Vector2D(float scalar);			//コンストラクタ（オーバーロード）
	Vector2D(float mx, float my);	//コンストラクタ（オーバーロード）
	Vector2D(int mx, int my);	//コンストラクタ（オーバーロード）
	Vector2D(float mx, int my);	//コンストラクタ（オーバーロード）
	Vector2D(int mx, float my);	//コンストラクタ（オーバーロード）

	~Vector2D();					//デストラクタ

public:
	//代入オペレーター	
	Vector2D& operator = (const Vector2D& location);

	//比較演算オペレーター
	bool operator == (const Vector2D& location);
	bool operator != (const Vector2D& location);
	bool operator > (const Vector2D& location);
	bool operator >= (const Vector2D& location);
	bool operator < (const Vector2D& location);
	bool operator <= (const Vector2D& location);

	//加算オペレーター
	const Vector2D operator + (const Vector2D& location)const;
	//加算代入オペレーター
	Vector2D& operator += (const Vector2D& location);

	//減算オペレーター
	const Vector2D operator - (const Vector2D& location)const;
	//減算代入オペレーター
	Vector2D& operator -= (const Vector2D& location);

	//乗算オペレーター(scalar)
	const Vector2D operator * (const float& scalar)const;
	//乗算オペレーター(location)
	const Vector2D operator * (const Vector2D& location)const;
	//乗算代入オペレーター(scalar)
	Vector2D& operator *=(const float& scalar);
	//乗算代入オペレーター(location)
	Vector2D& operator *=(const Vector2D& location);

	//除算オペレーター(scalar)
	const Vector2D operator / (const float& scalar)const;
	//除算オペレーター(location)
	const Vector2D operator / (const Vector2D& location)const;
	//除算代入オペレーター(scalar)
	Vector2D& operator /= (const float& scalar);
	//除算代入オペレーター(location)
	Vector2D& operator /= (const Vector2D& location);

public:
	//座標をInt型にキャストする
	void ToInt(int* x, int* y)const;

	/// <summary>
	/// Vector2D型の引数でDrawBoxをする
	/// </summary>
	/// <param name="_loc1">描画座標左上</param>
	/// <param name="_loc2">描画座標右下</param>
	/// <param name="_color">描画色</param>
	/// <param name="_fill_flg">塗りつぶすかどうか</param>
	static void DrawBoxV2(Vector2D _loc1, Vector2D _loc2, unsigned int _color, bool _fill_flg)
	{
		DrawBoxAA(_loc1.x, _loc1.y, _loc2.x, _loc2.y, _color, _fill_flg);
	}
};







