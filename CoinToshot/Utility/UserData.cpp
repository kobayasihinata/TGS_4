#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"
#include "../Utility/ResourceManager.h"
//private
int UserData::frame = 0;			//フレーム測定
int UserData::now_button = 0;		//ボタンアニメーション用

//public
RankingData UserData::ranking_data[RANKING_DATA]{ 0 };	//ランキングデータ格納
RankingData UserData::my_ranking_data{ -1,"ありえない",-1 };	//ありえない値を入れておく

float UserData::player_hp = DEFAULT_HP;			
int UserData::coin = 10;						
int UserData::timer = DEFAULT_TIMELIMIT;		
bool UserData::is_clear = false;
bool UserData::is_dead = false;
bool UserData::is_gamestop = false;				
int UserData::invincible = 0;					
int UserData::bullet_type = 0;
bool UserData::attraction_flg = false;
int UserData::attraction_timer = 0;
bool UserData::can_bullet_change_flg = false;
std::vector<std::vector<int>> UserData::button_image;

int UserData::variable = 0;
bool UserData::variable_change = 0;
Vector2D UserData::variable_loc = 0;

void UserData::LoadButtonImage()
{
	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/UI/button1.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/UI/button2.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);

	//各変数初期化
	frame = 0;
	now_button = 0;
}

void UserData::Update()
{
	if (++frame > 30)
	{
		if (++now_button >= button_image.size())
		{
			now_button = 0;
		}
		frame = 0;
	}
}

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

void UserData::DrawButtonImage(Vector2D _loc, int _button,int _size)
{
	DrawRotaGraphF(_loc.x, _loc.y, (float)_size/50, 0, button_image[now_button][_button], TRUE);
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

void UserData::DrawCoin(Vector2D _loc, float _radius,int _r, int _g, int _b)
{
	//色の分割
	
	int main_color = GetColor(_r, _g, _b);
	int shadow_color = GetColor(_r > 80 ? _r - 80 : 0, _g > 80 ? _g - 80 : 0, _b);
	int sub_color = GetColor(_r, _g > 32 ? _g - 32 : 0, _b);

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