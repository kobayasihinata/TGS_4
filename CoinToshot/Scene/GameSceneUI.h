#pragma once
#include "../Utility/UserData.h"
#include "Tutorial.h"
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
	int frame;				//フレーム計測
	int max_attraction;		//吸い寄せバー表示用

	class Tutorial* tutorial;	//チュートリアル格納

	int bullet_image;		//弾種類表示
	int old_bullet_type;	//弾種類変更用
	int bullet_change_timer;//弾変更アニメーション時間測定
	float change_anim_move;	//アニメーションの移動量格納

	Vector2D player_ui_loc;	//プレイヤー情報位置

	//ポップアップ表示用
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

	//ボタン画像格納
	std::vector<std::vector<int>> button_image;

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw()const;

	//文字生成
	void SetUIData(Vector2D _location, string _text, int _text_color, float _move, int _life_span, int _font_size);

	//弾種類表示
	void CreateBulletTypeImage()const;

	//弾情報描画
	void DrawBullet(Vector2D _loc, int _type)const;

	//プレイヤー情報描画
	void DrawPlayerUI()const;

	//弾種類の移動が右か左か判断 true=右
	bool CheckMoveDirection(int _now, int _old)const;
};
