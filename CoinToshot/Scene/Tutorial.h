#pragma once
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"
#include "TutoType.h"
#include "Camera/Camera.h"
#include "../Utility/InputPad.h"
#include "../Object/Base/ObjectBase.h"
#include "Dxlib.h"

#define FADE_TIME	 20	//チュートリアルテキストのフェードイン、アウトにかかるフレーム数
#define TEXT_BOX	 32		//ひとつずつの画像大きさ

class Tutorial
{
private:
	class Camera* camera;		//カメラポインタ格納(プレイヤーの情報を渡すためだけに取得)

	bool tutorial_flg;	//チュートリアル中か判断
	bool tuto_stop_flg;	//チュートリアルの為に、オブジェクトを止める必要があるか

	TutoType now_tuto;	//現在実行中のチュートリアルを格納
	ObjectBase* tuto_object;	//チュートリアルを呼び出したオブジェクトを保存

	int timer;			//チュートリアル実行時間測定
	float text_alpha;	//フェードイン、アウトの制御
	int stick_anim;		//スティックを回すアニメーション用
	int button_anim;	//ボタンアニメーション用
	int aim_timer;		//照準チュートリアル用　敵に照準が合っている時間測定
	int aim_success_flg;		//照準成功
	int aim_success_timer;		//照準成功時間測定
	int attack_sequence;		//攻撃チュートリアル手順
	Vector2D enemy_loc;			//敵座標保管
	int attack_success_timer;	//攻撃成功時間測定
	int tuto_end_timer;			//攻撃成功後テキスト時間測定

	int l_stick[4] = { L_STICK_UP,L_STICK_RIGHT,L_STICK_DOWN,L_STICK_LEFT };//表示順
	int r_stick[4] = { R_STICK_UP,R_STICK_RIGHT,R_STICK_DOWN,R_STICK_LEFT };//表示順
	int text_box[3];	//テキストボックス 辺、角、内側で３種類格納
	int generate_text_box;	//生成したテキストボックスを格納する場所
	Vector2D text_box_loc;		//テキストボックス表示位置
	Vector2D text_box_size;		//テキストボックス大きさ

	bool ex_anim_flg;			//アニメーションをするか
	int ex_anim_timer;			//アニメーション用
	int now_image;				//アニメーション用
	std::vector<int> ex_anim;	//爆発アニメーション
	int ex_se;					//爆発音

	bool tuto_executed_flg[TUTO_NUM];	//チュートリアルを既に行ったか判断
public:
	bool tuto_flg = false;			//既にチュートリアルを受けたことがあったら真
	float player_aim_rad = 0.f;		//現在の照準位置
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
	//現在のチュートリアルを取得
	TutoType GetNowTutorial()const { return now_tuto; }
	//オブジェクト停止状態か取得
	bool GetTutoStopFlg()const { return tuto_stop_flg; }
	//指定したチュートリアルが終わっているか取得
	bool GetIsEndTutorial(TutoType _type)const { return tuto_executed_flg[_type]; }
	//指定したチュートリアルが終わっている、または現在そのチュートリアル中か取得
	bool GetTutoNowEnd(TutoType _type)const {
		return GetIsEndTutorial(_type) || GetNowTutorial() == _type;
	}
	//基本チュートリアルが終わっているか取得
	bool GetBasicTuto()const
	{
		return (tuto_executed_flg[TutoType::tRule] &&
			tuto_executed_flg[TutoType::tMove] &&
			tuto_executed_flg[TutoType::tAim] &&
			tuto_executed_flg[TutoType::tAttack]);
	}

	//チュートリアル開始リクエストを送る _obj=チュートリアルを呼び出したオブジェクト
	bool StartTutoRequest(TutoType _type, ObjectBase* _obj = NULL);

	//チュートリアル毎の初期化
	void InitTuto(TutoType _type);

	//チュートリアル終了処理
	void TutoEnd();

	//照準が敵に合っているか判断
	bool CheckAim();

	//テキスト表示用箱生成
	void CreateTextBox()const;

	//パーツからテキストボックスを生成
	void GenerateTextBox(Vector2D _size)const;


	//時間経過で終了するタイプのチュートリアルの基礎更新
	void UpdateTimeTuto();

	//ルール説明描画
	void DrawRule()const;

	//移動説明更新
	void UpdateMove();
	//移動説明描画
	void DrawMove()const;

	//照準説明更新
	void UpdateAim();
	//照準説明描画
	void DrawAim()const;

	//攻撃説明更新
	void UpdateAttack();
	//攻撃説明描画
	void DrawAttack()const;

	//弾変更説明更新
	void UpdateBulletChange();
	//弾変更説明描画
	void DrawBulletChange()const;
};
