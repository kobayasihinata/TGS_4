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
	/// 次のステージに切り替える ないならクリアリザルト画面へ
	/// </summary>
	void ChangeNextStage();

	/// <summary>
	/// リザルト遷移
	/// </summary>
	/// <param name="_is_clear">false=ゲームオーバー true=ゲームクリア</param>
	void ChangeResult();

	/// <summary>
	/// ランダムでアイテムを発生させる
	/// </summary>
	void SpawnItem();
};



