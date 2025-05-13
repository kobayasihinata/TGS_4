#include "DxLib.h"

#include "InGameScene.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy1.h"
#include "../Utility/InputKey.h"
#include "../Utility/UserData.h"
#define _OPEN_SYS_ITOA_EXT
#include <string.h>

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

	change_result_delay = 0;

	camera = Camera::Get();

	//オブジェクト管理クラス生成
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI生成
	ui = new GameSceneUI();

	//プレイヤー生成
	objects->CreateObject({ Vector2D{0,0},Vector2D{40,40},ePLAYER });

	objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{40,40},eENEMY1/*, 20.f*/ });
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

void InGameScene::ChangeResult(int _delay)
{
	//遅延無し
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
		objects->CreateObject(EnemyRandSpawn());
	}
}

void InGameScene::CreatePopUp(Vector2D _location, string _text, int _font_size, int _text_color, float _move, int _life_span)
{
	ui->SetUIData(_location, _text, _font_size,_text_color, _move, _life_span);
}

Vector2D InGameScene::GetRandLoc()
{
	Vector2D ret;

	//左右の端か上下の端かランダムで決定
	if ((bool)GetRand(1))
	{
		//左右の端
		ret.x = (float)(camera->player_location.x - SCREEN_WIDTH + (SCREEN_WIDTH * 2 * GetRand(1)));
		ret.y = (float)(camera->player_location.y - SCREEN_HEIGHT + GetRand(SCREEN_HEIGHT*2));
	}
	else
	{
		//上下の端
		ret.x = (float)(camera->player_location.x - SCREEN_WIDTH + GetRand(SCREEN_WIDTH*2));
		ret.y = (float)(camera->player_location.y - SCREEN_HEIGHT + (SCREEN_HEIGHT * 2 * GetRand(1)));
	}
	return ret;
}

ObjectList InGameScene::GetRandEnemy()
{
	int coin = UserData::coin;
	//コインが0～99ならenemy1をスポーン
	if (coin < 100)
	{
		return GetEnemy(eENEMY3, 100);
	}
	//コインが100～199ならenemy1と2をランダムでスポーン
	if (coin < 200)
	{
		return GetEnemy(eENEMY1, 50, eENEMY2, 50);
	}
	//コインが200～299ならenemy2をスポーン
	if (coin < 300)
	{
		return GetEnemy(eENEMY2, 100);
	}
	//コインが300～399ならenemy2と3をスポーン
	if (coin < 400)
	{
		return GetEnemy(eENEMY2, 50, eENEMY3, 50);
	}
	//コインが400～499ならenemy2と4をスポーン
	if (coin < 500)
	{
		return GetEnemy(eENEMY2, 90, eENEMY4, 10);
	}
	//コインが500～599ならenemy2と5をスポーン
	if (coin < 600)
	{
		return GetEnemy(eENEMY2, 80, eENEMY5, 20);
	}
	//それ以降は全ての敵を均等に
	else
	{
		return GetEnemy(eENEMY1, 20, eENEMY2, 20, eENEMY3, 20, eENEMY4, 20, eENEMY5, 20);
	}

	//何もなければ敵1をスポーンさせる
	return eENEMY2;
}

ObjectList InGameScene::GetEnemy(ObjectList _list1, int _prob1,
	ObjectList _list2, int _prob2,
	ObjectList _list3, int _prob3,
	ObjectList _list4, int _prob4,
	ObjectList _list5, int _prob5)
{
	int rand = GetRand(_prob1 + _prob2 + _prob3 + _prob4 + _prob5);

	//ランダムな値がオブジェクト１の確率の値を下回っていたら
	if (rand < _prob1)
	{
		//オブジェクト１を返す
		return _list1;
	}
	//ランダムな値がオブジェクト１、２の確率の合計を下回っていたら
	else if (rand < _prob1 + _prob2)
	{
		//オブジェクト２
		return _list2;
	}
	//ランダムな(略)
	else if (rand < _prob1 + _prob2 + _prob3)
	{
		//オブジェクト３
		return _list3;
	}
	//ランダムな(略)
	else if (rand < _prob1 + _prob2 + _prob3 + _prob4)
	{
		//オブジェクト４
		return _list4;
	}
	//ランダムな(略)
	else if (rand < _prob1 + _prob2 + _prob3 + _prob4 + _prob5)
	{
		//オブジェクト５
		return _list5;
	}

	//何かのエラーで失敗したらコインをドロップする
	return eCOIN;
}

ObjectData InGameScene::EnemyRandSpawn()
{
	ObjectData ret;
	ObjectList spawn = GetRandEnemy();
	//スポーンする敵に応じた大きさを初期値に代入する
	switch (spawn)
	{
	case ObjectList::eENEMY1:
		ret = { GetRandLoc(), Vector2D{ ENEMY1_WIDTH,ENEMY1_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY2:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY2_WIDTH,ENEMY2_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY3:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY3_WIDTH,ENEMY3_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY4:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY4_WIDTH,ENEMY4_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY5:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY5_WIDTH,ENEMY5_HEIGHT }, spawn};
		break;
	default:
		break;
	}
	return ret;
}