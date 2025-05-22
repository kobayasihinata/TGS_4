#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"

RankingData UserData::ranking_data[RANKING_DATA]{ 0 };	//ランキングデータ格納

float UserData::player_hp = DEFAULT_HP;			//体力
int UserData::coin = 10;						//スコア
int UserData::timer = DEFAULT_TIMELIMIT;		//残り時間
bool UserData::is_clear = true;					//false=ゲームオーバー true=ゲームクリア
bool UserData::is_gamestop = false;				//false=ゲーム中 true=ゲーム停止
int UserData::invincible = 0;					//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す
int UserData::bullet_type = 0;				//選択中の弾の種類
bool UserData::attraction_flg = false;
void UserData::ReadRankingData()
{
	const char* a = "Resource/Data/CTs_RankingData.txt";

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		RankingData data = { 0 };
		//ランキングデータ読み込み
		for (int i = 0; i < RANKING_DATA; i++)
		{
			file >> ranking_data[i].num;
			file >> ranking_data[i].name;
			file >> ranking_data[i].coin;
		}
	}
}

void UserData::WriteRankingData()
{
	const char* a = "Resource/Data/CTs_RankingData.txt";

	std::ofstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		//ランキングデータ読み込み
		for (int i = 0; i < RANKING_DATA; i++)
		{
			file << ranking_data[i].num << " ";
			file << ranking_data[i].name.c_str() << " ";
			file << ranking_data[i].coin << "\n";
		}
	}
}
