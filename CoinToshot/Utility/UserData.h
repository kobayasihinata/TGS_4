#pragma once
#include <vector>

#include "Vector2D.h"

//オブジェクトの情報
struct ObjectData
{
	Vector2D loc;	//座標
	Vector2D size;	//大きさ
	int type;		//種類
	float radius = 0.f;	//半径(四角は使わない)
};
class UserData
{
private:
	static ObjectData old_data;			//ひとつ前のオブジェクト情報格納用
public:
	static int width_num[10];						//ステージの幅
	static int height_num[10];						//ステージの高さ
	static std::vector<ObjectData> stage_data[10];	//ステージのオブジェクト情報
	static int stage_num;						//ステージの数
	static int now_stage;									//現在のステージ数

	//UIに表示する必要があり、保持しておいた方が良い可能性があるプレイヤー情報はここで管理
	static Vector2D spawn_loc[10];	//リスポーンする位置
	static float player_hp;		//体力
	static int coin;			//スコア
	static int timer;			//残り時間
	static bool is_clear;		//false=ゲームオーバー true=ゲームクリア
	static int invincible;		//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す

public:
	//各種データを読み込む
	static void ReadData();

	//データを書き込む
	static void WriteData();

};

