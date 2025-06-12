#pragma once
#include "SceneBase.h"
#include <string>
using namespace std;

#define KEY_WIDTH 10	//名前入力キーボード横要素数
#define KEY_HEIGHT 4	//名前入力キーボード縦要素数

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
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//表示している画面

	//名前入力関連
	int current_x;	//カーソルX位置
	int current_y;	//カーソルY位置
	string name;	//名前格納

	int result_bgm;				//リザルトBGM

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



