#include "SceneManager.h"
#include "../Utility/common.h"
#include "../Utility/InputKey.h"
#include "../Utility/InputPad.h"
#include "DxLib.h"
#include "Factory/SceneFactory.h"
#include "../Utility/DebugInformation.h"
#include "../Utility/FpsController.h"

SceneManager::SceneManager() : current_scene(nullptr), is_finalize(false)
{}

SceneManager::~SceneManager()
{
	//解放忘れ防止
	Finalize();
}

/// <summary>
/// 初期化処理
/// </summary>
void SceneManager::Initialize()
{
	//ウィンドウモードで軌道
	ChangeWindowMode(TRUE);

	//画面サイズの設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//DXライブラリの初期化
	if (DxLib_Init() == -1)
	{
		throw("DXライブラリが初期化出来ませんでした\n");
	}

	//裏画面から描画を始める
	SetDrawScreen(DX_SCREEN_BACK);

	//タイトル画面からシーンを開始する
	ChangeScene(eSceneType::eTitle);
}

/// <summary>
/// 実行処理
/// </summary>
void SceneManager::Update()
{
	//開始時間を取得
	LONGLONG start_time = GetNowHiPerformanceCount();

	//入力機能のインスタンス取得する処理
	InputKey* input = InputKey::Get();

	//fps制御
	FpsController* FPSC = new FpsController(60, 800);

	//メインループ（ウィンドウが閉じられる or ESCキーが入力された　でループを終了する）
	while (ProcessMessage() != -1)
	{
		//1フレーム当たりの時間を計算する
		float delta_second = static_cast<float>(GetNowHiPerformanceCount() - start_time) * 0.000001f;

		//入力機能の更新
		input->Update();
		InputPad::UpdateKey();

		//デバッグ表示の更新
		DebugInfomation::Update();

		FPSC->All();
#ifdef _DEBUG
		FPSC->Disp();
#endif

		//シーンの更新処理
		eSceneType next_scene_type = current_scene->Update(delta_second);

		//シーンの描画処理
		Draw();

		//シーン切り替え
		if (next_scene_type != current_scene->GetNowSceneType())
		{
			ChangeScene(next_scene_type);
		}

		//強制終了ボタン
		if (input->GetKeyState(KEY_INPUT_ESCAPE) == eInputState::Pressed)
		{
			break;
		}
	}
}

/// <summary>
/// 終了時処理
/// </summary>
void SceneManager::Finalize()
{
	if (is_finalize)
	{
		return;
	}

	//DXライブラリの終了処理
	DxLib_End();

	//終了処理成功
	is_finalize = true;
}

void SceneManager::Draw()const
{
	//画面の初期化
	ClearDrawScreen();

	//シーンの描画処理
	current_scene->Draw();

	//デバッグ表示の描画
	DebugInfomation::Draw();

	//裏画面の内容を表画面に反映する
	ScreenFlip();
}


void SceneManager::ChangeScene(eSceneType type)
{
	//引数で渡された情報から新しいシーンを作成する
	SceneBase* new_scene = SceneFactory::CreateScene(type);

	//エラーチェック
	if (new_scene == nullptr)
	{
		throw("シーンが生成できませんでした");
	}

	//現在シーンの終了処理
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	//新しいシーンの初期化
	new_scene->Initialize();
	current_scene = new_scene;
}