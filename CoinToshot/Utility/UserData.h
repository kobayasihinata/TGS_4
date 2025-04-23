#pragma once
#include <vector>

#include "Vector2D.h"
#define RANKING_DATA_NUM 2		//ランキングデータの項目数
struct RankingData
{
	char* name;
	int coin;
};

struct ObjectData
{
	Vector2D loc;		//位置
	Vector2D size;		//大きさ
	int type;			//オブジェクトの種類
	float radius = 0.f;	//半径(円専用)
};
class UserData
{

public:

	static std::vector<RankingData> ranking_data;	//ランキングデータ格納

	//UIに表示する必要があり、保持しておいた方が良い可能性があるプレイヤー情報はここで管理
	static Vector2D spawn_loc[10];	//リスポーンする位置
	static float player_hp;		//体力
	static int coin;			//スコア
	static int timer;			//残り時間
	static bool is_clear;		//false=ゲームオーバー true=ゲームクリア
	static int invincible;		//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す

public:
	//ランキングデータを読み込む
	static void ReadRankingData();

	//データを書き込む
	static void WriteRankingData();

};

