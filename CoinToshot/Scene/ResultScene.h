#pragma once
#include "SceneBase.h"
#include <string>
using namespace std;

#define KEY_WIDTH 10	//名前入力キーボード横要素数
#define KEY_HEIGHT 4	//名前入力キーボード縦要素数

#define START_ANIM_TIME 180		//遷移演出時間	
#define RESULT_ANIM_TIME 300	//リザルト表示演出時間
#define BONUS_ANIM_TIME 300		//ボーナス点数加算演出時間

#define SKIP_TIME 30		//残り何フレームまでスキップ出来るか

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
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//表示している画面


	int start_anim_timer;	//遷移演出時間測定
	int result_anim_timer;	//リザルト表示演出時間測定
	int bonus_anim_timer;	//ボーナス点数加算演出時間測定

	int add_anim_num;		//加算アニメーション用1フレーム辺りの加算枚数
	int add_anim_coin;		//加算アニメーション用コイン枚数

	bool add_coin_once;		//ボーナス加算を一回だけ行う

	//名前入力関連
	int current_x;		//カーソルX位置
	int current_y;		//カーソルY位置
	string name;		//名前格納

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
};



