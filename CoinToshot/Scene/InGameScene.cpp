#include "DxLib.h"

#include "InGameScene.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy1.h"
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
	UserData::player_hp = DEFAULT_HP;
	UserData::coin = 20;

	camera = Camera::Get();

	//オブジェクト管理クラス生成
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI生成
	ui = new GameSceneUI();

	//プレイヤー生成
	objects->CreateObject({ Vector2D{0,0},Vector2D{40,40},ePLAYER });

	objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{30,30},eENEMY1/*, 20.f*/ });
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

	//UI更新
	ui->Update();

	//アイテム生成
	SpawnItem();

	//敵生成
	SpawnEnemy();

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

	//プレイヤー描画
	objects->Draw();
	camera->GetCameraLocation().x;
	camera->GetCameraLocation().y;

	//グリッド表示
	for (int x = -STAGE_SIZE; x < STAGE_SIZE; x += 100)
	{
		DrawLineAA(x - camera->GetCameraLocation().x,
			-STAGE_SIZE - camera->GetCameraLocation().y,
			x - camera->GetCameraLocation().x,
			STAGE_SIZE - camera->GetCameraLocation().y,
			0x00ff00);
	}
	for (int y = -STAGE_SIZE; y < STAGE_SIZE; y += 100)
	{
		DrawLineAA(-STAGE_SIZE - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			STAGE_SIZE - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			0x00ff00);
	}

	//UI描画
	ui->Draw();
}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::ChangeResult()
{
	change_scene = eSceneType::eResult;
}

void InGameScene::SpawnItem()
{
	DebugInfomation::Add("frame", frame);

	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(GetRand(100)),(float)(GetRand(100))},Vector2D{40,40},eCOIN, 20.f});
		//objects->CreateObject({ Vector2D{-100,-100},Vector2D{30,30},eENEMY1,/* 20.f */ });
	}
}

void InGameScene::SpawnEnemy()
{
	//画面外からランダムに一定周期でスポーン
	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(camera->player_location.x - (SCREEN_WIDTH/2) + (SCREEN_WIDTH * GetRand(1))),(float)(camera->player_location.y - (SCREEN_HEIGHT / 2) + GetRand(SCREEN_HEIGHT))},Vector2D{40,40},eENEMY1, /*20.f */});

	}
}

void InGameScene::CreatePopUp(Vector2D _location, const char* _text, int _font_size, int _text_color, float _move, int _life_span)
{
	ui->SetUIData(_location, _text, _font_size,_text_color, _move, _life_span);
}