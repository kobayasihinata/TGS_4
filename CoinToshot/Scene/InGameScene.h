#pragma once
#include "SceneBase.h"
#include "../Object/ObjectManager.h"
#include "Camera/Camera.h"
#include "GameSceneUI.h"
#include "../Object/Base/ShapeCollider.h"

class InGameScene : public SceneBase
{
private:
	class Camera* camera;		//カメラ格納
	ObjectManager* objects;		//オブジェクト管理クラス
	GameSceneUI* ui;			//ゲームメインUI
	eSceneType change_scene;	//遷移先

	ShapeCollider* test;
	int change_result_delay;	//リザルト遷移までの遅延時間
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
	/// <param name="_life_span">生存期間</param>
	void CreatePopUp(Vector2D _location, string _text, int _font_size, int _text_color, float _move, int _life_span);

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
	ObjectData EnemyRandSpawn();
};



