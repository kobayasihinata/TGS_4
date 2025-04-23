#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"


std::vector<RankingData> UserData::ranking_data{ 0 };	//ランキングデータ格納

Vector2D UserData::spawn_loc[10] = { 0,0 };		//リスポーンする位置
float UserData::player_hp = DEFAULT_HP;			//体力
int UserData::coin = 10;						//スコア
int UserData::timer = DEFAULT_TIMELIMIT;		//残り時間
bool UserData::is_clear = true;					//false=ゲームオーバー true=ゲームクリア
int UserData::invincible = 0;					//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す

void UserData::ReadRankingData()
{
	const char* a = "Resource/UserData/CTs_RankingData.txt";

	std::ifstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		RankingData data = { 0 };
		//ファイルの最後まで読み込む
		while(!file.eof()){
			file >> data.name;
			file >> data.coin;
			//格納
			ranking_data.push_back(data);
		}
	}
}

void UserData::WriteRankingData()
{
	const char* a = "Resource/UserData/CTs_RankingData.txt";

	std::ofstream file(a);
	//ファイルが読み込めていたなら
	if (file)
	{
		//ランキングデータ配分列データを書き込む
		for (const auto& ranking_data : ranking_data)
		{
			file << ranking_data.name;
			file << ranking_data.coin;
			file << "\n";
		}
	}
}
