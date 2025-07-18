#pragma once
#include "SceneBase.h"
#include "../Object/ObjectManager.h"
#include "Camera/Camera.h"
#include "Tutorial.h"
#include "GameSceneUI.h"
#include "../Object/Base/ShapeCollider.h"
#include <vector>

//背景自動生成に使う情報
#define IMAGE_SIZE 64	//画像一つ当たりの大きさ
#define DEFAULT_BLOCK 11	//基本となるブロックの位置
#define ACCENT_DEFAULT 15	//アクセントになるブロックの位置

//遷移時アニメーション
#define G_START_ANIM_TIME 180
#define G_END_ANIM_TIME 300

//ボーナスアニメーション
#define TIME_BONUS 60	//文字表示時間
class InGameScene : public SceneBase
{
private:
	class Camera* camera;		//カメラ格納
	class Tutorial* tutorial;	//チュートリアル格納
	ObjectManager* objects;		//オブジェクト管理クラス
	GameSceneUI* ui;			//ゲームメインUI
	eSceneType change_scene;	//遷移先

	ShapeCollider* test;
	int change_result_delay;	//リザルト遷移までの遅延時間
	bool change_result;			//リザルト遷移処理を開始するか
	int tuto_coin_count;		//チュートリアル時に指定の枚数コインを発生させる
	bool tuto_heal_once;		//チュートリアル時回復アイテム生成
	int tuto_heal_timer;		//チュートリアル時回復アイテム生成タイミング
	int first_bonus_count;		//一定時間経過時に指定の枚数コインを発生させる
	int bonus_timer;			//ボーナスコインの表示時間測定
	int second_bonus_count;		//一定時間経過時に指定の枚数コインを発生させる
	int bg_image;				//完成した背景画像を格納する
	int flower_image;			//ガイド表示用の花を保存
	Vector2D guide_loc;			//ガイド表示の絶対座標
	Vector2D guide_local_loc;			//ガイド表示の描画座標
	Vector2D guide_size;			//ガイド表示の大きさ

	bool update_once;			//遷移時アニメーション用描画の為に一回だけ更新
	bool start_anim_flg;		//遷移時アニメーション
	int start_anim_timer;		//遷移時アニメーション測定

	bool pause_flg;				//一時停止判断
	bool coin_spawn_once;		//最低保証コインのスポーン

	int gamemain_bgm;			//BGM格納
	int game_clear_se;			//ゲームクリア効果音
	int game_over_se;			//ゲームオーバー効果音
	int clap_se;				//拍手効果音
	int coin_se;				//コインSE

public:
	InGameScene();
	virtual ~InGameScene();

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


	/// <summary>
	/// リザルト遷移
	/// </summary>
	/// <param name="_delay">遷移するまでの遅延</param>
	void ChangeResult(int _delay = 0);

	/// <summary>
	/// ランダムでアイテムを発生させる
	/// </summary>
	void SpawnItem();

	/// <summary>
	/// ランダムに敵を生成
	/// </summary>
	void SpawnEnemy();

	/// <summary>
	/// 画面上に出るUI生成
	/// </summary>
	/// <param name="_location">表示する座標</param>
	/// <param name="_text">表示する文字</param>
	/// <param name="_text_color">文字色</param>
	/// <param name="_move">移動量</param>
	void CreatePopUp(Vector2D _location, string _text, int _text_color = 0xffffff, float _move = 1.f, int _life_span = 60);

	/// <summary>
	/// 画面端のランダムな座標を受け取る
	/// </summary>
	/// <returns>座標</returns>
	Vector2D GetRandLoc();

	/// <summary>
	/// プレイヤーのコイン所持数に応じて敵の出現確率を変える
	/// </summary>
	/// <returns>スポーンさせる敵</returns>
	ObjectList GetRandEnemy();

	/// <summary>
	/// 指定したオブジェクトを指定した確率で返す
	/// 例：GetEnemy(eEnemy1,100)←確実にeEnemy1を返す
	/// 例：GetEnemy(eEnemy1,50,eEnemy2,50)←五分五分でeEnemy1かeEnemy2を返す
	/// 
	/// 確率は、全ての値を合計して、１００％になるように割った値で抽選される
	/// 例：GetEnemy(eEnemy1,100,eEnemy2,50)←合計が150なので、0~100でeEnemy1、101~150でeEnemy2　つまりeEnemy1は約66%の確率
	/// 例：GetEnemy(eEnemy1,25,eEnemy2,25)←Enemy1は50%の確率
	/// 
	/// </summary>
	ObjectList GetEnemy(ObjectList _list1,int _prob1,
		ObjectList _list2 = eNULL, int _prob2 = 0,
		ObjectList _list3 = eNULL, int _prob3 = 0,
		ObjectList _list4 = eNULL, int _prob4 = 0, 
		ObjectList _list5 = eNULL, int _prob5 = 0);

	/// <summary>
	/// 敵の生成に必要な情報を、現在の状態に応じて取得
	/// </summary>
	/// <returns>敵の生成に必要な情報</returns>
	ObjectData GetEnemyData();

	/// <summary>
	/// 背景の自動生成
	/// </summary>
	void CreateBackGround();

	/// <summary>
	/// ボーナスコイン更新
	/// </summary>
	void BonusCoinUpdate();

	/// <summary>
	/// チュートリアル用更新
	/// </summary>
	void TutorialUpdate();

	/// <summary>
	/// ガイド表示位置を更新
	/// </summary>
	void UpdateGuideLoc();
};



