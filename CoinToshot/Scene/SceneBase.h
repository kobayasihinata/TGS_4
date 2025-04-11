#pragma once

enum eSceneType
{
	eTitle,
	eInGame,
	eResult,
	eEdit
};

class SceneBase
{

public:
	SceneBase() {}
	virtual ~SceneBase() {}

	//初期化処理
	virtual void Initialize(){}

	//更新処理
	//引数：１フレーム当たりの時間
	//戻り値：次のシーンタイプ
	virtual eSceneType Update(float _delta)
	{
		return GetNowSceneType();
	}

	//描画処理
	virtual void Draw()const {}

	//終了時処理
	virtual void Finalize() {}

	//現在のシーンタイプ(オーバーライド必須)
	virtual eSceneType GetNowSceneType()const = 0;
};
