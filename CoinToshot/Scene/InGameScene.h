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

//ボス出現タイミング
#define BOSS1_SPAWN 200
#define BOSS2_SPAWN 140
#define BOSS3_SPAWN  60

#define BOSS_BGM_FADE 120	//ボスBGMフェード時間

struct BossHp
{
	char* name;		//ボス名
	float hp;		//表示HP
	float max_hp;	//HP最大値
	float hp_move;	//減少アニメーション用
};

class InGameScene : public SceneBase
{
private:
	class Camera* camera;		//カメラ格納
	class Tutorial* tutorial;	//チュートリアル格納
	ObjectManager* objects;		//オブジェクト管理クラス
	GameSceneUI* ui;			//ゲームメインUI
	eSceneType change_scene;	//遷移先

	ObjectBase* update_shop;	//ショップ画面用
	bool shop_flg;				//ショップ画面を表示するか

	int change_result_delay;	//リザルト遷移までの遅延時間
	bool change_result;			//リザルト遷移処理を開始するか
	int tuto_coin_count;		//チュートリアル時に指定の枚数コインを発生させる
	bool tuto_heal_once;		//チュートリアル時回復アイテム生成
	int tuto_heal_timer;		//チュートリアル時回復アイテム生成タイミング
	int first_bonus_count;		//一定時間経過時に指定の枚数コインを発生させる
	int bonus_timer;			//ボーナスコインの表示時間測定
	int second_bonus_count;		//一定時間経過時に指定の枚数コインを発生させる

	int gamemain_image;			//描画全てを一枚の画像として保存
	int bg_image;				//完成した背景画像を格納する
	int flower_image;			//ガイド表示用の花を保存

	Vector2D guide_loc;			//ガイド表示の絶対座標
	Vector2D guide_local_loc;	//ガイド表示の描画座標
	Vector2D guide_size;		//ガイド表示の大きさ

	bool update_once;			//遷移時アニメーション用描画の為に一回だけ更新
	bool start_anim_flg;		//遷移時アニメーション
	int start_anim_timer;		//遷移時アニメーション測定

	bool pause_flg;				//一時停止判断
	bool pause_timer;			//一時停止の時間測定
	int pause_cursor;			//ポーズ画面のカーソル
	bool back_title_flg;		//タイトル画面に戻るか確認する画面を表示するか判断
	int back_title_cursor;		//タイトル画面に戻るか確認
	bool coin_spawn_once;		//最低保証コインのスポーン
	bool boss_spawn_once[3];	//ボスを一体だけ生成

	Vector2D now_gm_loc;		//ゲームメイン画像の現在位置
	Vector2D goal_gm_loc;		//ゲームメイン画像のゴール位置
	float now_gm_size;			//ゲームメイン画像の現在倍率
	float goal_gm_size;			//ゲームメイン画像のゴール倍率
	int zoom_time;				//何秒間ズームしたままでいるか
	int zoom_time_count;		//何秒間ズームしたままでいるか測定
	float zoom_speed;			//ズームの速度

	std::vector<BossHp> boss_hp;//ボスの体力表示用
	bool boss_timer_stop;		//ボスによってタイマーが止まっているか判断	

	bool boss_anim_flg;			//ボス生成アニメーション
	int boss_anim_count;		//ボスのアニメーション時間
	int boss_anim_timer;		//ボスのアニメーション測定
	ObjectBase* now_anim_boss;	//現在アニメーションしているボス

	int first_bonus_image;		//3/1ボーナス画像
	int second_bonus_image;		//3/2ボーナス画像

	bool crossfade_flg;			//BGMのクロスフェード
	int crossfade_num;			//クロスフェードの数値
	int now_bgm;				//クロスフェード開始時のBGM
	int crossfade_bgm;			//クロスフェード終了後のBGM

	int gamemain_bgm;			//BGM格納
	int boss_bgm;				//BGM格納
	int game_clear_se;			//ゲームクリアSE
	int game_over_se;			//ゲームオーバーSE
	int clap_se;				//拍手SE
	int coin_se;				//コインSE
	int cursor_se;				//カーソルSE
	int enter_se;				//決定SE
	int back_se;				//戻るボタンSE
	int throw_se;				//投げるSE
	int cheers_se;				//歓声SE

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
	/// ゲームメインのオブジェクトが使う画像を読み込んでおく
	/// </summary>
	void LoadGameMainResource();

	/// <summary>
	/// 描画する画像を作る
	/// </summary>
	void MakeGameMainDraw();

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
	/// ボーナス描画作成
	/// </summary>
	void CreateBonusDraw();

	/// <summary>
	/// チュートリアル用更新
	/// </summary>
	void TutorialUpdate();

	/// <summary>
	/// ガイド表示位置を更新
	/// </summary>
	void UpdateGuideLoc();

	/// <summary>
	/// リザルトに表示するシーン判定
	/// </summary>
	void SaveReplay();

	/// <summary>
	/// ショップ画面を表示するか判断
	/// </summary>
	/// <param name="_flg">表示するかしないか</param>
	/// <param name="_shop">表示するショップのポインタ</param>
	void SetShopFlg(bool _flg, ObjectBase* _shop = nullptr);

	/// <summary>
	/// ショップ画面表示中か取得
	/// </summary>
	/// <returns></returns>
	bool GetShopFlg() { return shop_flg; }

	/// <summary>
	/// ゲームメイン画像のズーム処理
	/// </summary>
	void UpdateZoom();

	/// <summary>
	/// ズームの値を設定
	/// </summary>
	/// <param name="_loc">位置 画面中心座標が標準</param>
	/// <param name="_size">大きさ 1が標準</param>
	/// <param name="_time">拡大している時間</param>
	/// <param name="_speed">拡大する速度</param>
	void SetZoom(Vector2D _loc, float _size, int _time, float _speed);

	/// <summary>
	/// 表示するボスの情報を格納する
	/// </summary>
	/// <param name="_name">ボス名</param>
	/// <param name="_hp">体力</param>
	/// <param name="_hp_max">体力最大値</param>
	/// <param name="_hp_move">ゲージ減少アニメーション</param>
	void SetBossHp(const char* _name, float _hp, float _hp_max, float _hp_move);

	/// <summary>
	/// ボス登場演出
	/// </summary>
	/// <param name="_spawn_loc">ボスの座標（相対座標）</param>
	/// <param name="_anim_time">アニメーション時間</param>
	void SetBossSpawnAnim(ObjectBase* _boss,Vector2D _spawn_loc, int _anim_time);

	/// <summary>
	/// BGM更新
	/// </summary>
	void UpdateSound();

	/// <summary>
	/// クロスフェードを開始する
	/// </summary>
	/// <param name="now_bgm">クロスフェードする前のBGM</param>
	/// <param name="_change_bgm">クロスフェードした後のBGM</param>
	void SetCrossFade(int _change_bgm);

	/// <summary>
	/// 通常のゲームメインBGMに戻す
	/// </summary>
	void SetCrossFadeDefault() { SetCrossFade(gamemain_bgm); }

	/// <summary>
	/// BGMのクロスフェード処理
	/// </summary>
	void UpdateCrossFade();

	/// <summary>
	/// ボスによってタイマーが停止しているか取得
	/// </summary>
	/// <returns></returns>
	bool GetBossTimerStop()const { return boss_timer_stop; }
};



