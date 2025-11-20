#pragma once

#include "SceneBase.h"
#include "Tutorial.h"

#define ITEM_NUM 4	//タイトルの項目数
#define START_ANIM 180  //アニメーション時間

enum TitleItem
{
	tGameMain = 0,
	tRanking,
	tOption,
	tEnd,
};

static char item_text[ITEM_NUM][256] =
{
	"Start",
	"Ranking",
	"Option",
	"End"
};
class TitleScene : public SceneBase
{
private:
	class Tutorial* tutorial;	//チュートリアル格納

	bool start_anim_flg;	//タイトル遷移時アニメーション
	int start_anim_timer;	//タイトル遷移時アニメーション測定
	bool tuto_reset_flg;	//チュートリアルをスキップするか選ばせる
	Vector2D tuto_loc;		//チュートリアルスキップ時の項目の座標
	Vector2D tuto_size;		//チュートリアルスキップ時の項目の大きさ
	int tuto_current_num;	//チュートリアルスキップ時の項目
	int old_tuto_current_num;//チュートリアルスキップ時のひとつ前の項目

	int current_num;		//選択されている項目
	int old_current_num;	//ひとつ前に選択されていた項目
	Vector2D menu_loc;		//項目の座標
	Vector2D menu_size;		//項目一つずつの大きさ
	int bg_image;			//背景画像格納

	int title_bgm;				//タイトルBGM
	int cursor_se;				//カーソルSE
	int enter_se;				//決定SE
public:
	TitleScene();
	virtual ~TitleScene();

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

	//背景画像生成
	int CreateBackGround();
};

