#include "DxLib.h"

#include "InGameScene.h"
#include "../Object/Player/Player.h"
#include "../Utility/InputKey.h"
#include "../Utility/UserData.h"

InGameScene::InGameScene() : objects(nullptr)
{
}

InGameScene::~InGameScene()
{

}

void InGameScene::Initialize()
{
	__super::Initialize();

	//各数字リセット
	UserData::timer = DEFAULT_TIMELIMIT;
	UserData::now_stage = 0;
	UserData::player_hp = DEFAULT_HP;
	UserData::life = DEFAULT_LIFE;
	UserData::score = 0;

	camera = Camera::Get();
	camera->SetStageSize({ (float)UserData::width_num[UserData::now_stage],(float)UserData::height_num[UserData::now_stage] });

	//オブジェクト管理クラス生成
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI生成
	ui = new GameSceneUI();

	//プレイヤー生成
	objects->CreateObject({ Vector2D{0,0},Vector2D{0,0},ePLAYER });
}

void InGameScene::Finalize()
{
	//オブジェクト管理クラス終了時処理
	objects->Finalize();

	//プレイヤー削除
	delete objects;
}

eSceneType InGameScene::Update(float _delta)
{

	change_scene = __super::Update(_delta);

	//制限時間減少
	UserData::timer--;

	//カメラ更新
	camera->Update();

	//オブジェクト更新
	objects->Update();

	//入力機能の取得
	InputKey* input = InputKey::Get();

	//1キーでタイトル画面に遷移する
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		change_scene = eSceneType::eTitle;
	}

	//2キーでリザルト画面に遷移する
	if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
	{
		change_scene = eSceneType::eResult;
	}

	return change_scene;
}

void InGameScene::Draw()const
{
	DrawString(10, 10, "InGame", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Title  2 = Result", 0xffffff);

	//UI描画
	ui->Draw();

	//プレイヤー描画
	objects->Draw();
	camera->GetCameraLocation().x;
	camera->GetCameraLocation().y;
	//グリッド表示
	for (int x = -STAGE_SIZE; x < STAGE_SIZE+SCREEN_WIDTH; x += 100)
	{
		DrawLine(x - camera->GetCameraLocation().x,
			-STAGE_SIZE - camera->GetCameraLocation().y,
			x - camera->GetCameraLocation().x,
			STAGE_SIZE + SCREEN_HEIGHT - camera->GetCameraLocation().y,
			0x00ff00);
	}
	for (int y = -STAGE_SIZE; y < STAGE_SIZE+SCREEN_HEIGHT; y += 100)
	{
		DrawLine(-STAGE_SIZE - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			STAGE_SIZE + SCREEN_WIDTH - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			0x00ff00);
	}
}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::ChangeNextStage()
{
	//現在のステージが最終ステージで遷移する場合はリザルトへ
	if (UserData::now_stage >= UserData::stage_num - 1)
	{
		//リザルト遷移
		ChangeResult();
	}
	//最終ステージ未満のステージなら次のステージへ
	else
	{
		//次のステージへ移動
		UserData::now_stage++;
		//既存のオブジェクトは全消し
		objects->DeleteAllObject();
		//読み込んだデータを基にステージを生成する
		std::vector<ObjectData> obj_data = UserData::stage_data[UserData::now_stage];
		for (const auto obj_data : obj_data)
		{
			objects->CreateObject(obj_data);
		}
	}

}
void InGameScene::ChangeResult()
{
	change_scene = eSceneType::eResult;
}


