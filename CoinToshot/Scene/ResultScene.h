#pragma once
#include "SceneBase.h"
#include <string>
using namespace std;

enum DispScene
{
	dIsClear = 0,
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//表示している画面
	char name[10];		//名前入力用

	//名前入力関連
	int current_x;	//カーソルX位置
	int current_y;	//カーソルY位置
	string name;	//名前格納

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

	//ランキング並べ替え
	void SortRanking();
};



