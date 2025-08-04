#pragma once
#include "SceneBase.h"
#include "../Utility/Vector2D.h"
#include <string>
#include <vector>
using namespace std;

#define KEY_WIDTH 10	//名前入力キーボード横要素数
#define KEY_HEIGHT 4	//名前入力キーボード縦要素数

#define START_ANIM_TIME 180		//遷移演出時間	
#define RESULT_ANIM_TIME 300	//リザルト表示演出時間
#define BONUS_ANIM_TIME 300		//ボーナス点数加算演出時間

#define SKIP_TIME 30		//残り何フレームまでスキップ出来るか

#define GRAVITY 1	//一フレーム間に加算される重力量
#define DECELERATION 0.1f	//減速率

#define GRAPH_WIDTH 1500	//スランプグラフの幅
#define GRAPH_HEIGHT 700	//スランプグラフの高さ

struct StringData {
	char text;		//表示文字
	Vector2D location;	//座標
	Vector2D goal_location;	//ゴール座標
	Vector2D goal_size;	//ゴール大きさ
	Vector2D velocity;	//移動方向
	float speed;		//速度
	float angle;		//角度
	bool is_add;		//追加する文字か削除する文字か
	bool operator == (const StringData _data)
	{
		if (this->text != _data.text)return false;
		if (this->location != _data.location)return false;
		if (this->goal_location != _data.goal_location)return false;
		if (this->goal_size != _data.goal_size)return false;
		if (this->velocity != _data.velocity)return false;
		if (this->speed != _data.speed)return false;
		if (this->angle != _data.angle)return false;
		if (this->is_add != _data.is_add)return false;
		return true;
	}
};

//入力できる文字たちとその配置
static char key[KEY_HEIGHT][KEY_WIDTH]{
	{'0','1','2','3','4','5','6','7','8','9',},
	{'a','b','c','d','e','f','g','h','i','j',},
	{'k','l','m','n','o','p','q','r','s','t',},
	{'u','v','w','x','y','z',' ',' ',' ',' ',},
};

enum DispScene
{
	dIsClear = 0,
	dDispResult,
	dBonusPoint,
	dGraphDisp,
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//表示している画面


	int start_anim_timer;	//遷移演出時間測定
	int result_anim_timer;	//リザルト表示演出時間測定
	int bonus_anim_timer;	//ボーナス点数加算演出時間測定
	int graph_timer;		//グラフ表示時間

	int add_anim_num;		//加算アニメーション用1フレーム辺りの加算枚数
	int add_anim_coin;		//加算アニメーション用コイン枚数

	bool add_coin_once;		//ボーナス加算を一回だけ行う

	//スランプグラフ関連
	Vector2D graph_loc;		//グラフの左上座標

	//名前入力関連
	Vector2D name_string_loc;	//名前表示位置
	Vector2D key_box_loc;		//名前入力箱位置
	int current_x;		//カーソルX位置
	int current_y;		//カーソルY位置
	string name;		//名前格納
	std::vector<StringData> string_data;	//名前入力時のアニメーション用
	std::vector<StringData> delete_string_data;	//名前入力時のアニメーション用

	bool disp_se_once;	//項目表示SE再生
	int result_bgm;		//リザルトBGM
	int button_se;		//ボタンSE
	int erase_se;		//文字削除SE
	int cursor_se;		//カーソルSE
	int disp_se;		//項目表示SE
	int coin_se;		//コイン加算SE
public:
	ResultScene();
	virtual ~ResultScene();

	//初期化処理
	virtual void Initialize()override;

	//更新処理
	//引数：１フレーム当たりの時間
	//戻り値：次のシーンタイプ
	virtual eSceneType Update(float _delta)override;

	//描画処理
	virtual void Draw()const override;

	//終了時処理
	virtual void Finalize() override;

	//現在のシーンタイプ(オーバーライド必須)
	virtual eSceneType GetNowSceneType()const override;

	//名前入力
	eSceneType EnterName();

	//名前入力時のDraw
	void EnterNameDraw()const;

	//ランキング並べ替え
	void SortRanking();

	//文字移動処理
	StringData MoveString(StringData _data);

	//文字生成処理
	void CreateMoveString(const char _c, Vector2D _loc, Vector2D _goal_loc, Vector2D _goal_size, bool _is_add, Vector2D _velocity = 0.f);
};



