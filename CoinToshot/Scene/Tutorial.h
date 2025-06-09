#pragma once
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"
#include "TutoType.h"
#include "Dxlib.h"

#define FADE_TIME	 20	//チュートリアルテキストのフェードイン、アウトにかかるフレーム数
#define TEXT_BOX	 32		//ひとつずつの画像大きさ

class Tutorial
{
private:
	bool tutorial_flg;	//チュートリアル中か判断
	bool tuto_stop_flg;	//チュートリアルの為に、オブジェクトを止める必要があるか

	TutoType now_tuto;	//現在実行中のチュートリアルを格納

	int timer;			//チュートリアル実行時間測定
	float text_alpha;	//フェードイン、アウトの制御

	int text_box[3];	//テキストボックス 辺、角、内側で３種類格納
	int generate_text_box;	//生成したテキストボックスを格納する場所
public:


private:
	//コンストラクタをprivateにすることで、
//自クラスのメンバ関数でインスタンスを生成できないようにする
	Tutorial() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	Tutorial(Tutorial& v) = default;
	Tutorial& operator = (const Tutorial& v) = delete;
	~Tutorial() = default;
public:
	//インスタンスを取得する処理
	static Tutorial* Get();
public:
	//初期化(リセット)
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw()const;

	//チュートリアル中か取得
	bool GetTutorialFlg()const { return tutorial_flg; }

	//オブジェクト停止状態か取得
	bool GetTutoStopFlg()const { return tuto_stop_flg; }

	//チュートリアル開始リクエストを送る
	bool StartTutoRequest(TutoType _type);

	//チュートリアル毎の初期化
	void InitTuto(TutoType _type);


	//ルール説明描画
	void DrawRule()const;

	//テキスト表示用箱生成
	void CreateTextBox()const;

	//パーツからテキストボックスを生成
	void GenerateTextBox(Vector2D _loc, Vector2D _size)const;
};
