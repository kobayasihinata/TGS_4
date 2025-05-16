#pragma once

#include "SceneBase.h"

class SceneManager
{
private:
	SceneBase* current_scene;	//現在シーン情報のポインタ
	bool is_finalize;

public:
	SceneManager();
	~SceneManager();

	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//終了時処理
	void Finalize();

private:
	//描画処理
	void Draw()const;

	//シーン切り替え処理
	void ChangeScene(eSceneType type);
};



