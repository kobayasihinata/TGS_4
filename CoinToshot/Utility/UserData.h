#pragma once
#include <vector>

#include "Vector2D.h"
#include "common.h"
#include "InputPad.h"

#include <string>
using namespace std;

struct RankingData
{
	int num;		//順番
	string name;		//名前
	int coin;		//コインの数
};

struct ObjectData
{
	Vector2D loc;		//位置
	Vector2D size;		//大きさ
	int type;			//オブジェクトの種類
	float radius = 0.f;	//半径(円専用)
};
class UserData
{
private:
	static int frame;			//フレーム測定
	static int now_button;		//ボタンアニメーション用
public:

	static RankingData ranking_data[RANKING_DATA];	//ランキングデータ格納
	static RankingData my_ranking_data;				//前回プレイのデータ

	//UIに表示する必要がある or 保持しておいた方が良い可能性があるプレイヤー情報はここで管理
	static float player_hp;			//体力
	static int coin;				//スコア
	static int timer;				//残り時間
	static bool is_clear;			//false=ゲームオーバー true=ゲームクリア
	static bool is_dead;			//プレイヤーが死亡演出中か
	static bool is_gamestop;		//false=ゲーム中 true=ゲーム終了
	static int invincible;			//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す
	static int bullet_type;			//選択中の弾の種類
	static bool attraction_flg;		//コイン吸い寄せ
	static int attraction_timer;	//コイン吸い寄せ時間測定
	static bool can_bullet_change_flg;	//弾種類変更可能か
	static bool player_damage_flg;	//プレイヤーがダメージを受けて動けない状態か判断
	static std::vector<std::vector<int>> button_image;//ボタン画像格納

	//デバッグ用
	static int variable;			//起動中に好きに変更できる値
	static bool variable_change;	//値変更中か
	static Vector2D variable_loc;	//調整表示用座標
public:
	//ボタン画像を読み込む
	static void LoadButtonImage();

	//ボタンアニメーション用更新
	static void Update();

	//ランキングデータを読み込む
	static void ReadRankingData();

	//データを書き込む
	static void WriteRankingData();

	//ボタン画像を描画
	static void DrawButtonImage(Vector2D _loc, int _button,int _size);

	//ボタンと文字を描画
	static void DrawButtonAndString(Vector2D _loc, int _button, const char* _text, int _color);

	//指定した位置を中心に文字を描画する
	static void DrawStringCenter(Vector2D _loc, const char* _text,int _color, int _font = 0);

	//コイン描画
	static void DrawCoin(Vector2D _loc, float _radius, int _r = 255, int _g = 255, int _b = 0);
		
	//可変の変数描画
	static void DrawVariable();
};

