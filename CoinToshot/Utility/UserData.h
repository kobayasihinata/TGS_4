#pragma once
#include <vector>

#include "Vector2D.h"
#include "common.h"
#include "InputPad.h"

#include <string>
using namespace std;

struct RankingData
{
	int num;		//順番
	string name;		//名前
	int coin;		//コインの数
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

	static RankingData ranking_data[RANKING_DATA];	//ランキングデータ格納

	//UIに表示する必要がある or 保持しておいた方が良い可能性があるプレイヤー情報はここで管理
	static float player_hp;			//体力
	static int coin;				//スコア
	static int timer;				//残り時間
	static bool is_clear;			//false=ゲームオーバー true=ゲームクリア
	static bool is_gamestop;		//false=ゲーム中 true=ゲーム終了
	static int invincible;			//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す
	static int bullet_type;			//選択中の弾の種類
	static bool attraction_flg;		//コイン吸い寄せ
	static int attraction_timer;	//コイン吸い寄せ時間測定
public:
	//ランキングデータを読み込む
	static void ReadRankingData();

	//データを書き込む
	static void WriteRankingData();

};

