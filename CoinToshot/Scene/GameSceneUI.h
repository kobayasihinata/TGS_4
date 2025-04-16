#pragma once
#include "../Utility/UserData.h"
#include <vector>

#define STRING_SPAN 10		//項目同士の間隔

struct UIData
{
	Vector2D local_location;	//文字表示位置
	const char* text;			//表示文字
	int text_color;				//文字色
	float move_speed;			//UI移動速度（上に進む）
	int life_span;		//寿命
	int life_count;		//時間測定
};
class GameSceneUI
{
private:
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

public:
	//更新
	void Update();
	//描画
	void Draw()const;

	//文字生成
	void SetUIData(Vector2D _location, const char* _text,int _text_color, float _move, int _life_span);
};
