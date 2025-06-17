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
	UserData::timer = DEBUG_PLAYER ? 600 : DEFAULT_TIMELIMIT;
	UserData::player_hp = DEBUG_PLAYER ? 1 : DEFAULT_HP;
	UserData::is_gamestop = false;
	UserData::attraction_flg = false;
	UserData::attraction_timer = 0;
	UserData::bullet_type = 0;
	UserData::invincible = 0;
	UserData::is_clear = false;
	UserData::can_bullet_change_flg = false;
	UserData::is_dead = false;

	change_result_delay = -1;//0になったらリザルト遷移
	change_result = false;
	tuto_coin_count = 0;
	pause_flg = false;
	coin_spawn_once = false;
	update_once = false;
	start_anim_flg = true;
	start_anim_timer = 0;
	tuto_heal_once = false;
	tuto_heal_timer = 0;
	camera = Camera::Get();
	camera->player_location = 0;
	tutorial = Tutorial::Get();
	//チュートリアルを受けたことが無いなら初期化
	if (!tutorial->tuto_flg)
	{
		tutorial->Initialize();
	}

	//チュートリアルが完了していないなら初期コインは0枚、しているなら20枚
	UserData::coin = tutorial->GetBasicTuto() ? 20 : 0;

	//オブジェクト管理クラス生成
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI生成
	ui = new GameSceneUI();
	ui->Initialize();

	//プレイヤー生成
	objects->CreateObject({ Vector2D{48,32},Vector2D{40,40},ePLAYER });

	objects->CreateObject({ Vector2D{(float)(GetRand(1)* 2000 - 1000),(float)(GetRand(1)* 2000 - 1000)},Vector2D{100,100},eSLOT});
	objects->CreateObject({ Vector2D{ 150, 30 },Vector2D{40,40},eCOIN, 20.f });

	objects->CreateObject({ {1050,0}, Vector2D{ ENEMY1_WIDTH,ENEMY1_HEIGHT }, eENEMY1});

	//背景の自動生成
	CreateBackGround();

	ResourceManager* rm = ResourceManager::GetInstance();
	//BGM読み込み
	gamemain_bgm = rm->GetSounds("Resource/Sounds/BGM/Rail_train.mp3");
	game_clear_se = rm->GetSounds("Resource/Sounds/Direction/victory.mp3");
	game_over_se = rm->GetSounds("Resource/Sounds/Direction/deden.mp3");
	clap_se = rm->GetSounds("Resource/Sounds/Direction/大勢で拍手.mp3");

	//BGMを初めから再生するための処理
	PlaySoundMem(gamemain_bgm, DX_PLAYTYPE_LOOP, TRUE);
	StopSoundMem(gamemain_bgm);
}

void InGameScene::Finalize()
{
	//チュートリアルを受けた事があると判断
	tutorial->tuto_flg = true;
	//万が一チュートリアル中にゲームが終了した場合は、チュートリアル履歴をリセット
	if (tutorial->GetNowTutorial() == TutoType::tAim || tutorial->GetNowTutorial() == TutoType::tAttack)
	{
		tutorial->tuto_flg = false;
	}
	//カメラを初期位置に戻しておく
	camera->Update({ -SCREEN_WIDTH / 2 + 48, -SCREEN_HEIGHT / 2 + 32 });

	StopSoundMem(gamemain_bgm);
	//オブジェクト管理クラス終了時処理
	objects->Finalize();

	//プレイヤー削除
	delete objects;
}

eSceneType InGameScene::Update(float _delta)
{
	change_scene = __super::Update(_delta);

	//チュートリアル更新
	tutorial->Update();

	//一時停止フラグ切り替え
	if (InputPad::OnButton(XINPUT_BUTTON_START) && !tutorial->GetTutorialFlg())
	{
		pause_flg = !pause_flg;
	}

	//一時停止フラグか遷移時アニメーションフラグが立っていたら更新しない
	if ((!pause_flg && !start_anim_flg)|| !update_once)
	{
		if (!CheckSoundMem(gamemain_bgm) && update_once)
		{
			PlaySoundMem(gamemain_bgm, DX_PLAYTYPE_LOOP, false);
		}

		//いずれかの時間停止フラグが立っていたらオブジェクトの動きはすべて止める
		if (!UserData::is_gamestop && !tutorial->GetTutoStopFlg())
		{
			//一定時間立ったら移動チュートリアルをリクエスト
			if (frame > 120)tutorial->StartTutoRequest(TutoType::tMove);

			//カメラ更新
			camera->Update();

			//移動チュートリアルが終わっていたらコイン投げ入れ
			if (!tutorial->tuto_flg && tutorial->GetIsEndTutorial(TutoType::tMove) && (int)frame % 10 == 0 && tuto_coin_count++ < 20)
			{
				Vector2D rand = GetRandLoc();
				Vector2D camera_center = { camera->GetCameraLocation().x + (SCREEN_WIDTH / 2),camera->GetCameraLocation().y + (SCREEN_HEIGHT / 2) };
				Vector2D rand_velocity = { ((camera_center.x - rand.x) + (GetRand(SCREEN_WIDTH - 200) - (SCREEN_WIDTH - 200) / 2)) / 10,
										   ((camera_center.y - rand.y) + (GetRand(SCREEN_HEIGHT - 200) - (SCREEN_HEIGHT - 200) / 2)) / 10 };
				objects->CreateObject(eCOIN, rand, { 40,40 }, 20.f, rand_velocity);
			}

			//チュートリアルが終わっていないとタイマーが動かず、敵とコインが湧かないようにする
			if (tutorial->GetIsEndTutorial(TutoType::tAttack))
			{
				//アイテム生成
				SpawnItem();
				//制限時間減少
				if(!UserData::is_dead)UserData::timer--;
				//敵生成
				SpawnEnemy();
			}

			//攻撃チュートリアル中にコインが０枚になったら１枚生成
			if (tutorial->GetNowTutorial() == TutoType::tAttack && UserData::coin <= 0)
			{
				if (!coin_spawn_once)
				{
					objects->CreateObject({ Vector2D{ 140, 30 },Vector2D{40,40},eCOIN, 20.f });
					coin_spawn_once = true;
				}
			}
			else
			{
				coin_spawn_once = false;
			}

			//照準チュートリアルと攻撃チュートリアル中はHPが5以下になったら画面外から回復アイテムを投げつけられる
			if (tutorial->GetNowTutorial() == TutoType::tAim || tutorial->GetNowTutorial() == TutoType::tAttack)
			{
				if (UserData::player_hp <= 5)
				{
					if (!tuto_heal_once && ++tuto_heal_timer > 15)
					{
						//回復アイテム生成
						Vector2D rand = GetRandLoc();
						Vector2D rand_velocity = { (camera->player_location.x - rand.x)/7  ,
												   (camera->player_location.y - rand.y)/7 };
						objects->CreateObject(eHEAL, rand, { 40,40 }, 20.f, rand_velocity);
						tuto_heal_once = true;
						tuto_heal_timer = 0;
					}
				}
				//HPが回復したらリセット
				else
				{
					tuto_heal_once = false;
				}
				
			}

			//オブジェクト更新
			objects->Update();
		}

		//UI更新
		ui->Update();

		//時間切れで終了（勝利扱い）
		if (UserData::timer <= 0 && !change_result)
		{
			UserData::is_clear = true;
			ui->confetti_flg = true;
			ChangeResult(G_END_ANIM_TIME);
		}

		//リザルト遷移前の演出
		if (change_result && --change_result_delay <= 0)
		{
			change_scene = eSceneType::eResult;
		}


#ifdef _DEBUG

		//入力機能の取得
		InputKey* input = InputKey::Get();

		//デバッグ用
		if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
		{
		}
		if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
		{
		
		}
		if (input->GetKeyState(KEY_INPUT_3) == eInputState::Pressed)
		{
			tutorial->StartTutoRequest(TutoType::tAim);
		}

		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{40,40},eHEAL });
		}
#endif // _DEBUG

	}
	else
	{
		//BGMを止める
		if (CheckSoundMem(gamemain_bgm))
		{
			StopSoundMem(gamemain_bgm);
		}
		//遷移アニメーション早送り
		if (InputPad::OnPressed(XINPUT_BUTTON_A))
		{
			start_anim_timer++;
		}
		//遷移アニメーションフラグが立っているなら時間測定
		if (start_anim_flg)
		{
			if (++start_anim_timer > G_START_ANIM_TIME)
			{
				start_anim_flg = false;
				tutorial->StartTutoRequest(TutoType::tRule);
			}
		}

	}

	//遷移時一回だけ更新
	update_once = true;
	return change_scene;
}

void InGameScene::Draw()const
{
	int old = GetFontSize();
	DrawString(10, 10, "InGame", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Title  2 = Result", 0xffffff);

	//背景画像描画
	DrawGraphF(-STAGE_SIZE - camera->GetCameraLocation().x,- STAGE_SIZE -camera->GetCameraLocation().y, bg_image, true);

	//オブジェクト描画
	objects->Draw();

	//UI描画
	ui->Draw();

	//ゲームオーバーかクリアか表示
	if (UserData::is_gamestop)
	{
		if (UserData::is_clear)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, change_result_delay < 25 ? 255 - (change_result_delay * 10) : 0);
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			SetFontSize(48);
			DrawBox((SCREEN_WIDTH/2) - 200, (SCREEN_HEIGHT/2) - 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2) + 50, 0x000000, true);
			DrawBox((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2) - 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2) + 50, 0xffffff, false);
			DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 30, "GameClear!", 0xffffff);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((255 / G_END_ANIM_TIME) * change_result_delay));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			SetFontSize(48);
			DrawBox((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2) - 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2) + 50, 0x000000, true);
			DrawBox((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2 )- 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2 )+ 50, 0xffffff, false);
			DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 30, "GameOver...", 0xaaaaaa);
		}
	}

	//一時停止フラグが立っていたら、ポーズ画面の描画
	if (pause_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }, "ポーズ中", 0xffffff);
	}
	//チュートリアルフラグが立っていたら、チュートリアル描画
	if (tutorial->GetTutorialFlg())
	{
		tutorial->Draw();
	}
	//遷移時アニメーションフラグが立っていたら、アニメーション処理
	if (start_anim_flg)
	{
		int coin_size = 900 - start_anim_timer * (900 / G_START_ANIM_TIME);
		if (coin_size > 20)
		{
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 }, coin_size);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, coin_size * 10);
			UserData::DrawCoin({ SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 }, 80 - coin_size*2,255,255,255);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}

	//最低保証コインが出た時のメッセージ
	if (coin_spawn_once)
	{
		DrawString((SCREEN_WIDTH / 2) + 100, (SCREEN_HEIGHT / 2) + 20, "あきらめないで！", (int)frame % 30 > 15 ? 0xcccc00 : 0xffff00);
	}
	SetFontSize(old);
}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::ChangeResult(int _delay)
{
	//遅延無し
	if (_delay <= 0)
	{
		change_scene = eSceneType::eResult;
	}
	//遅延あり
	else
	{
		//時止め
		UserData::is_gamestop = true;
		//リザルト遷移処理開始
		change_result = true;
		//リザルト遷移までの時間設定
		change_result_delay = _delay;
		//ゲームクリアかゲームオーバーのSEを再生
		if (UserData::is_clear)
		{
			if (!CheckSoundMem(game_clear_se))
			{
				PlaySoundMem(game_clear_se, DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			if (!CheckSoundMem(game_over_se))
			{
				PlaySoundMem(game_over_se, DX_PLAYTYPE_BACK);
			}
		}
	}
}

void InGameScene::SpawnItem()
{
	DebugInfomation::Add("frame", frame);

	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(GetRand(100)),(float)(GetRand(100))},Vector2D{40,40},eCOIN, 20.f });
		//objects->CreateObject({ Vector2D{-100,-100},Vector2D{30,30},eENEMY1,/* 20.f */ });
	}
	if ((int)frame % 10 == 0)
	{
		objects->CreateEffect(elShine, { (float)(GetRand(100)),(float)(GetRand(100)) }, TRUE, 0xffff00);
	}
}
void InGameScene::SpawnEnemy()
{
	//画面外からランダムに一定周期でスポーン
	if ((int)frame % (120 -((DEFAULT_TIMELIMIT - UserData::timer)/300)) == 0)
	{
		objects->CreateObject(GetEnemyData());
	}
}

void InGameScene::CreatePopUp(Vector2D _location, string _text, int _text_color, float _move, int _life_span, int _font_size)
{
	ui->SetUIData(_location, _text, _text_color, _move, _life_span, _font_size);
}

Vector2D InGameScene::GetRandLoc()
{
	Vector2D ret;

	//左右の端か上下の端かランダムで決定
	if ((bool)GetRand(1))
	{
		//左右の端
		ret.x = (float)(camera->GetCameraLocation().x - (SCREEN_WIDTH/2) + (SCREEN_WIDTH * 2 * GetRand(1)));
		ret.y = (float)(camera->GetCameraLocation().y - (SCREEN_HEIGHT/2) + GetRand(SCREEN_HEIGHT*2));
	}
	else
	{
		//上下の端
		ret.x = (float)(camera->GetCameraLocation().x - (SCREEN_WIDTH/2) + GetRand(SCREEN_WIDTH*2));
		ret.y = (float)(camera->GetCameraLocation().y - (SCREEN_HEIGHT/2) + (SCREEN_HEIGHT * 2 * GetRand(1)));
	}
	return ret;
}

ObjectList InGameScene::GetRandEnemy()
{
	int coin = UserData::coin;
	//コインが0～49ならenemy1をスポーン
	if (coin < 50)
	{
		return GetEnemy(eENEMY1, 100);
	}
	//コインが50～100ならenemy1と2をランダムでスポーン
	if (coin < 100)
	{
		return GetEnemy(eENEMY1, 50, eENEMY2, 50);
	}
	//コインが100～199ならenemy2をスポーン
	if (coin < 200)
	{
		return GetEnemy(eENEMY2, 100);
	}
	//コインが200～299ならenemy2と3をスポーン
	if (coin < 300)
	{
		return GetEnemy(eENEMY2, 50, eENEMY3, 50);
	}
	//コインが300～399ならenemy2と4をスポーン
	if (coin < 400)
	{
		return GetEnemy(eENEMY2, 90, eENEMY4, 10);
	}
	//コインが400～499ならenemy2と5をスポーン
	if (coin < 500)
	{
		return GetEnemy(eENEMY2, 80, eENEMY5, 20);
	}
	//それ以降は全ての敵を均等に
	else
	{
		return GetEnemy(eENEMY1, 30, eENEMY2, 30, eENEMY3, 15, eENEMY4, 10, eENEMY5, 15);
	}

	//どれにも当てはまらないなら、敵1をスポーンさせる
	return eENEMY1;
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

ObjectData InGameScene::GetEnemyData()
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

void InGameScene::CreateBackGround()
{
	std::vector<int> background_image;	//背景画像格納
	std::vector<std::vector<int>> bg_arran;	//背景画像配置

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	background_image = rm->GetImages("Resource/Images/BackGround/terrain_tiles_v2.png", 160, 10, 16, 64, 64);

	//草を敷き詰める
	std::vector<int> test;

	for (int i = 0; i < STAGE_SIZE * 2; i += IMAGE_SIZE)
	{
		for (int j = 0; j < STAGE_SIZE * 2; j += IMAGE_SIZE)
		{
			if (GetRand(5) == 0)
			{
				test.push_back(background_image[ACCENT_DEFAULT]);
			}
			else
			{
				test.push_back(background_image[DEFAULT_BLOCK]);
			}
		}
		bg_arran.push_back(test);
		test.clear();
	
	}

	//生成した背景を一つの画像として保存、それ以外の情報は削除
	bg_image = MakeScreen(STAGE_SIZE * 2, STAGE_SIZE * 2);
	SetDrawScreen(bg_image);
	ClearDrawScreen();
	int y = 0;
	for (const auto bg1 : bg_arran)
	{
		int x = 0;
		for (const auto bg2 : bg1)
		{
			DrawGraphF(x * 64, y * 64, bg2, true);
			x++;
		}
		y++;
	}
	//コインが生成されるエリアを緑地にする
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			DrawGraph(1950 + x * 64, 1950 + y * 64, background_image[40 + x + (y * 10)], TRUE);
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);

}