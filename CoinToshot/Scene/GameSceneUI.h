#pragma once
#include "../Utility/UserData.h"
#include "Tutorial.h"
#include <vector>
#include <string>
using namespace std;

#define STRING_SPAN 10		//項目同士の間隔
#define BONUS_TEXT_TIMER 300	//時間経過ボーナス表示時間

struct UIData
{
	Vector2D location;		//文字表示位置
	string text;			//表示文字
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

struct ConfettiData
{
	Vector2D location;	//表示座標
	Vector2D velocity;	//移動量
	int image;			//画像格納
	float radian;		//描画角度
	float move_radian;	//角度変更後の値
	float speed;		//移動速度

	bool operator == (const ConfettiData _data)
	{
		if (this->location != _data.location)return false;
		if (this->velocity != _data.velocity)return false;
		if (this->image != _data.image)return false;
		if (this->radian != _data.radian)return false;
		if (this->move_radian != _data.move_radian)return false;
		if (this->speed != _data.speed)return false;
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
	int now_coin_num;		//現在のコイン枚数
	int old_coin_num;		//１フレーム前のコイン枚数

	int first_bonus_timer;	//時間経過ボーナスの表示時間
	int second_bonus_timer;	//時間経過ボーナスの表示時間

	int damage_timer;		//ダメージ時間測定
	int con_spawn;			//紙吹雪生成までの間隔

	int lock_image;			//"ロックされている"用画像
	int confetti_image[8];	//8色の紙吹雪格納

	bool ex_anim_once;			//一回だけアニメーション
	bool ex_se_once;			//一回だけ再生
	int ex_anim_timer;			//アニメーション用
	int now_image;				//アニメーション用
	std::vector<int> ex_anim;	//爆発アニメーション
	int ex_se;					//爆発音
	int lock_se;				//使用不可SE
	int count_se;				//カウントダウンSE

	//ポップアップ表示用
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

	//コインログ用
	std::vector<UIData> coin_data;
	std::vector<UIData> delete_coin_data;

	//紙吹雪用
	std::vector<ConfettiData> confetti_data;
	std::vector<ConfettiData> delete_confetti_data;

public:
	bool confetti_flg;	//紙吹雪を生成するか判断
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw()const;

	//文字生成
	void SetUIData(Vector2D _location, string _text, int _text_color, float _move, int _life_span);

	//紙吹雪データ生成
	ConfettiData GetConfettiData()const;

	//弾種類表示
	void CreateBulletTypeImage()const;

	//ロック画像生成
	void CreateLockImage()const;

	//紙吹雪８種類生成
	void CreateConfettiImage();

	//弾情報描画
	void DrawBullet(Vector2D _loc, int _type)const;

	//プレイヤー情報描画
	void DrawPlayerUI()const;

	//弾種類の移動が右か左か判断 true=右
	bool CheckMoveDirection(int _now, int _old)const;
};
