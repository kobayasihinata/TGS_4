#pragma once

#include "Vector2D.h"

#define D_KEYCODE_MAX (256)		//キーコード最大数
#define MOUSE_MAX (256)			//マウス最大数
//キー入力状態
enum class eInputState : unsigned char
{
	None,
	Pressed,
	Released,
	Held,
};

//マウスカーソル
struct Cursor
{
	int x;	//横軸値
	int y;	//縦軸値
};

class InputKey
{
private:

	//キーボード入力値
	char now_key[D_KEYCODE_MAX] = {};		//現在の入力格納
	char old_key[D_KEYCODE_MAX] = {};		//前回の入力格納
	int now_mouse[MOUSE_MAX] = {}; //今回の入力マウスボタン
	int old_mouse[MOUSE_MAX] = {}; //前回の入力マウスボタン

	Cursor cursor; //カーソル
private:
	//コンストラクタをprivateにすることで、
	//自クラスのメンバ関数でインスタンスを生成できないようにする
	InputKey() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	InputKey(InputKey& v) = default;
	InputKey& operator = (const InputKey& v) = delete;
	~InputKey() = default;

public:
	//インスタンスを取得する処理
	static InputKey* Get();
public:
	//入力更新
	void Update();
public:
	//入力状態を取得する(キーボード)
	eInputState GetKeyState(int keycode)const;

	//入力状態を取得する(マウス)
	eInputState GetMouseState(int mouse)const;

	//マウスカーソルの座標を取得する
	Cursor GetMouseCursor()
	{
		return cursor;
	}

private:
	//指定した数字がキー配列の範囲内か確認
	bool CheckKeyCodeRange(int key_code)const
	{
		return (0 <= key_code && key_code < D_KEYCODE_MAX);
	}
	//指定した数字がキー配列の範囲内か確認
	bool CheckMouseRange(int mouse)const
	{
		return (0 <= mouse && mouse < MOUSE_MAX);
	}

};




