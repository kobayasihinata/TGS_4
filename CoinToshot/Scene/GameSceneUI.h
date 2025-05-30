#pragma once
#include "../Utility/UserData.h"
#include <vector>
#include <string>
using namespace std;

#define STRING_SPAN 10		//項目同士の間隔

struct UIData
{
	Vector2D location;		//文字表示位置
	string text;		//表示文字
	int font_size;			//文字大きさ
	int text_color;			//文字色
	float move_speed;		//UI移動速度（上に進む）
	int life_span;			//寿命
	int life_count;			//時間測定

	bool operator == (const UIData _data)
	{
		if (this->location != _data.location)return false;
		if (this->text != _data.text)return false;
		if (this->text_color != _data.text_color)return false;
		if (this->move_speed != _data.move_speed)return false;
		if (this->life_span != _data.life_span)return false;
		if (this->life_count != _data.life_count)return false;
		return true;
	}
};
class GameSceneUI
{
private:
	int frame;			//フレーム計測
	int max_attraction;	//吸い寄せバー表示用
	int ui_font;		//フォントハンドル格納

	//ポップアップ表示用
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

	std::vector<std::vector<int>> button_image;
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw()const;

	//文字生成
	void SetUIData(Vector2D _location, string _text,int _font_size, int _text_color, float _move, int _life_span);
};
