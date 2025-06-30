#include "DxLib.h"
#include "Scene/SceneManager.h"

#include "Utility/UserData.h"

//メイン関数
int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	//ローカル変数定義
	SceneManager* scene_manager = nullptr;
	int result = 0;

	//必要データ読み込み
	UserData::ReadRankingData();

	try {
		//オブジェクト生成
		scene_manager = new SceneManager();
		
		//初期化処理
		scene_manager->Initialize();

		//更新処理
		scene_manager->Update();

		//終了時処理処理
		scene_manager->Finalize();
	}
	catch (const char* error_log)
	{
		//エラー情報を出力
		OutputDebugString(error_log);
		result = -1;
	}

	//シーンマネージャーを生成していたら、削除する
	if (scene_manager != nullptr)
	{
		delete scene_manager;
		scene_manager = nullptr;
	}

	//終了状態の値を返却する
	return result;
}