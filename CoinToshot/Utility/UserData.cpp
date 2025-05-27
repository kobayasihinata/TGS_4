#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"

RankingData UserData::ranking_data[RANKING_DATA]{ 0 };	//ランキングデータ格納

float UserData::player_hp = DEFAULT_HP;			
int UserData::coin = 10;						
int UserData::timer = DEFAULT_TIMELIMIT;		
bool UserData::is_clear = true;					
bool UserData::is_gamestop = false;				
int UserData::invincible = 0;					
int UserData::bullet_type = 0;
bool UserData::attraction_flg = false;
int UserData::attraction_timer = 0;

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
