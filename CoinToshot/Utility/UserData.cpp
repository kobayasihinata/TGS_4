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
InputKey* UserData::input = InputKey::Get();

//public
RankingData UserData::ranking_data[RANKING_DATA]{ 0 };	//ランキングデータ格納
RankingData UserData::my_ranking_data{ -1,"ありえない",-1 };	//ありえない値を入れておく

float UserData::player_hp = DEFAULT_HP;			
int UserData::coin = 0;						
int UserData::timer = DEFAULT_TIMELIMIT;		
bool UserData::is_clear = false;
bool UserData::is_dead = false;
bool UserData::is_gamestop = false;				
int UserData::invincible = 0;					
int UserData::now_bullet = 0;
std::vector<int> UserData::get_bullet = { 0 };
std::vector<int> UserData::get_bullet_cd = { 0 };
bool UserData::attraction_flg = false;
int UserData::attraction_timer = 0;
bool UserData::can_bullet_change_flg = false;
bool UserData::player_damage_flg = false;
std::vector<int> UserData::coin_graph;
std::vector<ReplayData> UserData::replay;
std::vector<std::vector<int>> UserData::button_image;
int UserData::coin_image = 0;

int UserData::se_volume = 0;
int UserData::se_volume_num = 7;
int UserData::bgm_volume = 0;
int UserData::bgm_volume_num = 7;
int UserData::control_type = 0;
SceneBase* UserData::old_scene = NULL;

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

	coin_image = MakeScreen(40, 40, TRUE);
	SetDrawScreen(coin_image);
	ClearDrawScreen();
	UserData::DrawCoin({ 19,19 }, 20);
	SetDrawScreen(DX_SCREEN_BACK);
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
	for (auto& get_bullet_cd : get_bullet_cd)
	{
		if (get_bullet_cd > 0)
		{
			get_bullet_cd--;
		}
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
		file >> se_volume;
		file >> se_volume_num;
		file >> bgm_volume;
		file >> bgm_volume_num;
		file >> control_type;
	}
}

void UserData::WriteData()
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
		file << se_volume << "\n";
		file << se_volume_num << "\n";
		file << bgm_volume << "\n";
		file << bgm_volume_num << "\n";
		file << control_type << "\n";
	}
}

void UserData::DrawButtonImage(Vector2D _loc, int _button,int _size)
{
	DrawRotaGraphF(_loc.x, _loc.y, (float)_size/50, 0, button_image[now_button][_button], TRUE);
}

void UserData::DrawButtonAndString(Vector2D _loc, int _button ,const char* _text, int _color)
{
	int size = (GetFontSize() * 1.5f);
	DrawButtonImage(_loc, _button, size);
	DrawStringF(_loc.x + (size/2), _loc.y - (size/2), _text, 0x000000);
	DrawStringF(_loc.x + (size/2), _loc.y - (size/2), _text, _color);
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

	DrawCircleAA(_loc.x, _loc.y, _radius, 40, main_color, true);
	DrawCircleAA(_loc.x - 1, _loc.y - 1, _radius / 1.5f, 40, shadow_color, true);
	DrawCircleAA(_loc.x, _loc.y, _radius / 1.5f, 40, sub_color, true);
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

void UserData::DrawDefaultCoin(Vector2D _loc, float _radius)
{
	DrawRotaGraphF(_loc.x, _loc.y, _radius / 20.f, 0.f, coin_image, TRUE);
}

bool UserData::CheckCursorMove(int _dir)
{
	switch (_dir)
	{
	case UP:
		switch (control_type)
		{
			//上ボタン
		case 0:
		case 1:
			return InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_UP) || InputPad::GetPressedButton(L_STICK_UP);
			break;

			//Wキー
		case 2:
			return input->GetKeyState(KEY_INPUT_W) == eInputState::Pressed;
			break;
		}
		break;
	case DOWN:
		switch (control_type)
		{
			//下ボタン
		case 0:
		case 1:
			return InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_DOWN) || InputPad::GetPressedButton(L_STICK_DOWN);
			break;

			//Sキー
		case 2:
			return input->GetKeyState(KEY_INPUT_S) == eInputState::Pressed;
			break;
		}
		break;
	case RIGHT:
		switch (control_type)
		{
			//右ボタン
		case 0:
		case 1:
			return InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::GetPressedButton(L_STICK_RIGHT);
			break;

			//Dキー
		case 2:
			return input->GetKeyState(KEY_INPUT_D) == eInputState::Pressed;
			break;
		}
		break;
	case LEFT:
		switch (control_type)
		{
			//左ボタン
		case 0:
		case 1:
			return InputPad::GetPressedButton(XINPUT_BUTTON_DPAD_LEFT) || InputPad::GetPressedButton(L_STICK_LEFT);
			break;

			//Aキー
		case 2:
			return input->GetKeyState(KEY_INPUT_A) == eInputState::Pressed;
			break;
		}
		break;
	default:
		break;
	}
	return false;
}

bool UserData::CheckPlayerMove(int _dir)
{
	switch (_dir)
	{
	case UP:
		switch (control_type)
		{
			//上ボタン
		case 0:
		case 1:
			return InputPad::OnPressed(XINPUT_BUTTON_DPAD_UP) || InputPad::OnPressed(L_STICK_UP);
			break;

			//Wキー
		case 2:
			return input->GetKeyState(KEY_INPUT_W) == eInputState::Held;
			break;
		}
		break;
	case DOWN:
		switch (control_type)
		{
			//下ボタン
		case 0:
		case 1:
			return InputPad::OnPressed(XINPUT_BUTTON_DPAD_DOWN) || InputPad::OnPressed(L_STICK_DOWN);
			break;

			//Sキー
		case 2:
			return input->GetKeyState(KEY_INPUT_S) == eInputState::Held;
			break;
		}
		break;
	case RIGHT:
		switch (control_type)
		{
			//右ボタン
		case 0:
		case 1:
			return InputPad::OnPressed(XINPUT_BUTTON_DPAD_RIGHT) || InputPad::OnPressed(L_STICK_RIGHT);
			break;

			//Dキー
		case 2:
			return input->GetKeyState(KEY_INPUT_D) == eInputState::Held;
			break;
		}
		break;
	case LEFT:
		switch (control_type)
		{
			//左ボタン
		case 0:
		case 1:
			return InputPad::OnPressed(XINPUT_BUTTON_DPAD_LEFT) || InputPad::OnPressed(L_STICK_LEFT);
			break;

			//Aキー
		case 2:
			return input->GetKeyState(KEY_INPUT_A) == eInputState::Held;
			break;
		}
		break;
	default:
		break;
	}
	return false;
}

bool UserData::CheckEnter()
{
	switch (control_type)
	{
		//Aボタン
	case 0:
	case 1:
		return InputPad::OnButton(XINPUT_BUTTON_A);
		break;

		//スペースキー or 左クリック
	case 2:
		return input->GetKeyState(KEY_INPUT_SPACE) == eInputState::Pressed ||
			   input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::Pressed;
		break;
	}
	return false;
}

bool UserData::CheckCancel()
{
	switch (control_type)
	{
		//Bボタン
	case 0:
	case 1:
		return InputPad::OnButton(XINPUT_BUTTON_B);
		break;

		//エスケープキー or 右クリック
	case 2:
		return input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::Pressed ||
			input->GetMouseState(MOUSE_INPUT_RIGHT) == eInputState::Pressed;
		break;
	}
	return false;
}

bool UserData::CheckPause()
{
	switch (control_type)
	{
		//STARTボタン
	case 0:
	case 1:
		return InputPad::OnButton(XINPUT_BUTTON_START);
		break;

		//エスケープキー
	case 2:
		return input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::Pressed;
		break;
	}
	return false;
}

bool UserData::CheckBulletButton()
{
	switch (control_type)
	{
		//LBとRB
	case 0:
		return InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER) || InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER);
		break;

		//Bボタン
	case 1:
		return InputPad::OnButton(XINPUT_BUTTON_B);
		break;
		//左クリックor右クリックしながら左クリック
	case 2:
		return (input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::Pressed ||
			input->GetMouseState(0x0003) == eInputState::Pressed);
		break;
	}
	return false;
}

bool UserData::CheckBulletChangeButtonRight()
{
	switch (control_type)
	{
		//右トリガー
	case 0:
		return InputPad::OnButton(R_TRIGGER);
		break;

		//RB
	case 1:
		return InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER);
		break;

		//Eキー（仮）
	case 2:
		return input->GetKeyState(KEY_INPUT_E) == eInputState::Pressed;
		break;
	}
}

bool UserData::CheckBulletChangeButtonLeft()
{
	switch (control_type)
	{
		//左トリガー
	case 0:
		return InputPad::OnButton(L_TRIGGER);
		break;

		//LB
	case 1:
		return InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER);
		break;

		//Qキー（仮）
	case 2:
		return input->GetKeyState(KEY_INPUT_Q) == eInputState::Pressed;
		break;
	}
}
bool UserData::CheckCursor(Vector2D _loc, Vector2D _size)
{
	Vector2D cursor = { input->GetMouseCursor().x ,input->GetMouseCursor().y };
	return cursor >= _loc && cursor <= _loc + _size;
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

int UserData::DrawNum(Vector2D _loc, int _num ,int _color)
{
	std::string a = to_string(_num);
	for (int i = 0; i < a.size();i++)
	{
		if (a[i] != '1')
		{
			DrawFormatString(_loc.x + (i * (GetFontSize()*0.7f)), _loc.y, _color, "%c", a[i]);
		}
		else
		{
			DrawFormatString(_loc.x + (i * (GetFontSize() * 0.7f))+ (GetFontSize() * 0.15f), _loc.y, _color, "%c", a[i]);
		}
	}
	return a.size() * (GetFontSize() * 0.7f);
}

float UserData::Distance(Vector2D _loc1, Vector2D _loc2)
{
	return std::sqrt(std::pow(_loc2.x - _loc1.x, 2) + std::pow(_loc2.y - _loc1.y, 2));
}