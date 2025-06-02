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

int UserData::variable = 0;
bool UserData::variable_change = 0;
Vector2D UserData::variable_loc = 0;

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

void UserData::DrawStringCenter(Vector2D _loc, const char* _text, int _color, int _font)
{
	//フォント指定があればフォント用の描画
	if (_font != 0)
	{
		DrawStringFToHandle(_loc.x - GetDrawStringWidthToHandle(_text, strlen(_text),_font)/2, _loc.y, _text, _color, _font);
	}
	else
	{
		DrawStringF(_loc.x - GetDrawStringWidth(_text, strlen(_text))/2, _loc.y, _text, _color);
	}
}

void UserData::DrawCoin(Vector2D _loc, float _radius,int _r, int _g, int _b, int _a)
{
	//色の分割
	
	int main_color = GetColor(_r, _g, _b);
	int shadow_color = GetColor(_r > 80 ? _r - 80 : 0, _g > 80 ? _g - 80 : 0, _b);
	int sub_color = GetColor(_r, _g > 32 ? _g - 32 : 0, _b);

	if (_a <= 255)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _a);
	}
	DrawCircleAA(_loc.x, _loc.y, _radius, 20, main_color, true);
	DrawCircleAA(_loc.x - 1, _loc.y - 1, _radius / 1.5f, 20, shadow_color, true);
	DrawCircleAA(_loc.x, _loc.y, _radius / 1.5f, 20, sub_color, true);
	DrawBoxAA(_loc.x - (_radius / 6) + 1,
		_loc.y - (_radius / 2) + 1,
		_loc.x + (_radius / 6) + 1,
		_loc.y + (_radius / 2) + 1,
		shadow_color, true);
	DrawBoxAA(_loc.x - (_radius / 6),
		_loc.y - (_radius / 2),
		_loc.x + (_radius / 6),
		_loc.y + (_radius / 2),
		main_color, true);

	if (_a <= 255)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void UserData::DrawVariable()
{
	DrawBoxAA(variable_loc.x,
		variable_loc.y,
		variable_loc.x + 10,
		variable_loc.y - variable,
		0x00ff00, TRUE);
	DrawFormatStringF(variable_loc.x-10, variable_loc.y, 0x000000, "%d", variable);
	DrawFormatStringF(variable_loc.x-11, variable_loc.y-1, 0xffffff, "%d", variable);
}