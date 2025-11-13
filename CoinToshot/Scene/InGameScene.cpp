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
	UserData::now_bullet = 0;
	UserData::invincible = 0;
	UserData::is_clear = false;
	UserData::can_bullet_change_flg = false;
	UserData::is_dead = false;
	UserData::coin_graph.clear();
	UserData::get_bullet = { 0,1 };
	UserData::get_bullet_cd = { 0,0 };
	UserData::now_bullet = 0;

	update_shop = nullptr;
	shop_flg = false;
	change_result_delay = -1;//0になったらリザルト遷移
	change_result = false;
	tuto_coin_count = 0;
	pause_flg = false;
	pause_timer = 0;
	pause_cursor = 0;
	back_title_flg = false;
	back_title_cursor = 0;
	coin_spawn_once = false;
	for (int i = 0; i < 3; i++)
	{
		boss_spawn_once[i] = false;
	}
	first_bonus_count = 0;
	bonus_timer = 0;
	second_bonus_count = 0;
	update_once = false;
	start_anim_flg = true;
	start_anim_timer = 0;
	tuto_heal_once = false;
	tuto_heal_timer = 0;
	camera = Camera::Get();
	camera->player_location = 0;
	tutorial = Tutorial::Get();
	tutorial->GetInGame(this);
	guide_loc = 0;
	guide_local_loc = 0;
	guide_size = 95;
	goal_gm_loc = now_gm_loc = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };
	goal_gm_size = now_gm_size = 1.f;
	zoom_time = 0;
	zoom_time_count = 0; 
	zoom_speed = 0;

	boss_anim_flg = false;
	boss_anim_count = 0;
	boss_anim_timer = 0;
	now_anim_boss = nullptr;

	//チュートリアルが完了していないなら初期コインは0枚、しているなら20枚
	UserData::coin = tutorial->GetBasicTuto() ? 20 : 0;

	//オブジェクト管理クラス生成
	objects = new ObjectManager();
	objects->Initialize(this);
	
	//プレイヤー生成
	objects->CreateObject({ Vector2D{48,32},Vector2D{40,40},ePLAYER });

	//UI生成
	ui = new GameSceneUI();
	ui->Initialize(this);


	objects->CreateObject({ {-1000,-1000},{100,100},eSLOT});
	objects->CreateObject({ {1000,1000},{100,100},eSLOT});
	objects->CreateObject({ {-1000,1000},{160,120},eSHOP});
	objects->CreateObject({ {1000,-1000},{160,120},eSHOP});
	objects->CreateObject({ Vector2D{ 200, 30},Vector2D{40,40},eCOIN, 20.f});
	objects->CreateObject({ {1050,0}, Vector2D{ ENEMY1_WIDTH,ENEMY1_HEIGHT }, eENEMY1});

	gamemain_image = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
	flower_image = MakeScreen(191, 191);

	//背景の自動生成
	CreateBackGround();
	//ボーナス描画の生成
	CreateBonusDraw();

	//ゲームメインで使う画像、音源の事前読み込み
	LoadGameMainResource();

	//BGMを初めから再生するための処理
	ResourceManager::rPlaySound(gamemain_bgm,DX_PLAYTYPE_LOOP,TRUE);
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

	//ボスHP表示用変数リセット
	boss_hp.clear();

	//チュートリアル更新
	tutorial->Update();

	//一定時間経過毎のコインの枚数を保存
	if (UserData::timer % 60 == 1)
	{
		UserData::coin_graph.push_back(UserData::coin);
	}
	//一時停止フラグ切り替え
	if (UserData::CheckPause() && !tutorial->GetTutorialFlg() && !boss_anim_flg)
	{
		pause_flg = !pause_flg;
		pause_timer = 0;
	}

	//一時停止フラグか遷移時アニメーションフラグかショップ展開フラグが立っていたら更新しない
	if (((!pause_flg && !start_anim_flg)|| !update_once )&& !shop_flg && !boss_anim_flg)
	{
		if (!CheckSoundMem(gamemain_bgm) && update_once && !UserData::is_gamestop)
		{
			ResourceManager::rPlaySound(gamemain_bgm, DX_PLAYTYPE_LOOP, false);
		}

		//いずれかの時間停止フラグが立っていたらオブジェクトの動きはすべて止める
		if (!UserData::is_gamestop && !tutorial->GetTutoStopFlg() && !ui->GetNewBulletFlg())
		{
			//一定時間立ったら移動チュートリアルをリクエスト
			if (frame > 120)tutorial->StartTutoRequest(TutoType::tMove);

			//カメラ更新
			camera->Update();
			//ガイド表示更新
			UpdateGuideLoc();
			//オブジェクト更新
			objects->Update();
			//チュートリアル更新
			TutorialUpdate();
		

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

		//描画の更新
		MakeGameMainDraw();

		//十分なデータがあれば、60フレーム毎に実行
		if (UserData::coin_graph.size() >= 2 && (int)frame % 60 == 0)
		{
			//リプレイに保存するか判断
			SaveReplay();
		}

////////////////////////////////////////////////////
		//入力機能の取得
		InputKey* input = InputKey::Get();

		//デバッグ用
		if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
		{
			SetZoom({ SCREEN_WIDTH / 2,SCREEN_HEIGHT/2 }, 3, 60, 20);
		}
		if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
		{
			objects->CreateObject({ GetRandLoc(), Vector2D{ BOSS1_WIDTH,BOSS1_HEIGHT }, eBOSS1 });
		}
		if (input->GetKeyState(KEY_INPUT_3) == eInputState::Pressed)
		{
			objects->CreateObject({ GetRandLoc(), Vector2D{ BOSS2_WIDTH,BOSS2_HEIGHT }, eBOSS2 });
		}
		if (input->GetKeyState(KEY_INPUT_4) == eInputState::Pressed)
		{
			objects->CreateObject({ GetRandLoc(), Vector2D{ BOSS3_WIDTH,BOSS3_HEIGHT }, eBOSS3 });
		}
		if (InputPad::OnButton(XINPUT_BUTTON_A))
		{
			//objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{40,40},eHEAL });
			/*objects->CreateObject({ Vector2D{ 150, 30 },Vector2D{40,40},eCOIN, 20.f });  */
		}
//////////////////////////////////////////////////

	}
	//ポーズ画面
	else if (pause_flg)
	{
		//時間測定
		pause_timer++;
		if (!CheckSoundMem(gamemain_bgm))
		{
			ResourceManager::rPlaySound(gamemain_bgm, DX_PLAYTYPE_LOOP, false);
		}
		//タイトル画面に戻る確認画面が出ていなければポーズ更新
		if (!back_title_flg)
		{
			//下入力で項目下移動
			if (UserData::CheckCursorMove(DOWN))
			{
				if (++pause_cursor >= 3)
				{
					pause_cursor = 0;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//上入力で項目上移動
			if (UserData::CheckCursorMove(UP))
			{
				if (--pause_cursor < 0)
				{
					pause_cursor = 2;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//Aボタンでカーソルが合っているところの処理を実行
			if (UserData::CheckEnter())
			{
				ResourceManager::rPlaySound(enter_se, DX_PLAYTYPE_BACK);
				switch (pause_cursor)
				{
				case 0:	//ゲームに戻る
					pause_flg = false;
					break;
				case 1: //オプション
					if (CheckSoundMem(gamemain_bgm))
					{
						StopSoundMem(gamemain_bgm);
					}
					UserData::old_scene = this;
					return eSceneType::eOption;
					break;
				case 2: //タイトルに戻る
					//確認画面を表示
					back_title_flg = true;
					break;
				default:
					break;
				}

			}
			//キャンセルボタンでゲームに戻る
			if (pause_timer > 5 && UserData::CheckCancel())
			{
				ResourceManager::rPlaySound(enter_se, DX_PLAYTYPE_BACK);
				pause_flg = false;
			}
		}
		//タイトル画面に戻る確認画面が出ていれば確認画面更新
		else
		{
			//左右入力で終了のカーソル移動
			if (UserData::CheckCursorMove(LEFT))
			{
				if (--back_title_cursor < 0)
				{
					back_title_cursor = 1;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			if (UserData::CheckCursorMove(RIGHT))
			{
				if (++back_title_cursor > 1)
				{
					back_title_cursor = 0;
				}
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
			//キャンセルボタンで項目の選択を解除
			if (UserData::CheckCancel())
			{
				back_title_flg = false;
				ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
			}
			//決定ボタンでカーソルが合っている場所ごとの処理
			if (UserData::CheckEnter())
			{
				switch (back_title_cursor)
				{
					//項目の選択解除
				case 0:
					back_title_flg = false;
					ResourceManager::rPlaySound(back_se, DX_PLAYTYPE_BACK);
					break;
					//タイトル遷移
				case 1:
					change_scene = eSceneType::eTitle;
					ResourceManager::rPlaySound(enter_se, DX_PLAYTYPE_BACK);
					break;
				default:
					break;
				}
			}
		}
	}
	//ショップ画面
	else if (shop_flg)
	{
		if (update_shop != nullptr)
		{
			update_shop->Update();
		}
	}
	//ボス登場アニメーション
	else if (boss_anim_flg)
	{
		//カメラ更新
		camera->Update();

		//座標のみ更新
		objects->UpdateBossAnim(now_anim_boss);

		MakeGameMainDraw();

		//アニメーション終了後一定時間は止まったまま、カメラはプレイヤーに返す
		if (++boss_anim_count > boss_anim_timer)
		{
			camera->camera_gaze_location = 0;
		}
		//一定時間経過でアニメーション処理全終了
		if (boss_anim_count > boss_anim_timer + 60)
		{
			boss_anim_flg = false;
		}
	}
	//遷移アニメーション
	else
	{
		InputKey* input = InputKey::Get();

		//BGMを止める
		if (CheckSoundMem(gamemain_bgm))
		{
			StopSoundMem(gamemain_bgm);
		}
		//遷移アニメーション早送り
		if (
			(UserData::control_type != 2 && InputPad::OnPressed(XINPUT_BUTTON_A)) ||
			(UserData::control_type == 2 && (input->InputKey::GetKeyState(KEY_INPUT_SPACE) == eInputState::Held || input->GetMouseState(MOUSE_INPUT_LEFT) == eInputState::Held))
			)
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
		//描画の更新
		MakeGameMainDraw();
	}

	//画面ズームの更新
	UpdateZoom();


	//遷移時一回だけ更新
	update_once = true;
	return change_scene;
}

void InGameScene::Draw()const
{

	DrawRotaGraph(now_gm_loc.x+camera->GetRandImpact(), now_gm_loc.y+camera->GetRandImpact(), now_gm_size, 0, gamemain_image, FALSE);

	//一時停止フラグかショップ表示フラグが立っていたら背景を暗くする
	if (pause_flg || shop_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//ショップ画面の描画
	if (update_shop != nullptr)
	{
		update_shop->Draw();
	}

	//一時停止フラグが立っていたら、ポーズ画面の描画
	if (pause_flg)
	{
		SetFontSize(72);
		UserData::DrawStringCenter({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }, "ポーズ中", 0xffffff);
		SetFontSize(48);
		if (back_title_flg)
		{
			DrawString(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 100, "タイトルに戻りますか？", 0xffffff);
			DrawString(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 150, "いいえ", back_title_cursor == 0 ? 0xffff00 : 0xffffff);
			DrawString(SCREEN_WIDTH / 2 + 120, SCREEN_HEIGHT / 2 + 150, "はい", back_title_cursor == 1 ? 0xffff00 : 0xffffff);
			UserData::DrawCoin({ (float)SCREEN_WIDTH / 2 - 200 + (back_title_cursor * 300), SCREEN_HEIGHT / 2 + 185 }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
		}
		else
		{
			DrawString(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 100, "再開", 0xffffff);
			DrawString(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 150, "オプション", 0xffffff);
			DrawString(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 200, "タイトル", 0xffffff);
			UserData::DrawCoin({ (float)SCREEN_WIDTH / 2 - 80, (float)SCREEN_HEIGHT / 2 + (pause_cursor * 50) + 130 }, 20, 227 + abs(((int)frame % 56 - 28)), 200);
		}
	}
	//ズームデバッグ表示
	//DrawBox(0, 0, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5, 0x000000, true);
	//DrawBoxAA((goal_gm_loc.x / 5) - ((SCREEN_WIDTH / 5) / 2 / goal_gm_size),
	//	      (goal_gm_loc.y / 5) - ((SCREEN_HEIGHT / 5) / 2 / goal_gm_size),
	//	      (goal_gm_loc.x / 5) + ((SCREEN_WIDTH / 5) / 2 / goal_gm_size),
	//	      (goal_gm_loc.y / 5) + ((SCREEN_HEIGHT / 5) / 2 / goal_gm_size),
	//	      0xff0000, false);
	//照準チュートリアル描画
	if (tutorial->GetNowTutorial() == TutoType::tAim)
	{
		tutorial->Draw();
	}

	//ボスが居たら画面を薄暗く
	if (boss_hp.size() > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::LoadGameMainResource()
{

	ResourceManager* rm = ResourceManager::GetInstance();

	//画像を読み込んでおく	
	rm->GetImages("Resource/Images/Item/Heal.png", 7, 5, 2, 32, 41);
	rm->GetImages("Resource/Images/Item/Magnet.png", 4, 4, 1, 64, 64);
	rm->GetImages("Resource/Images/Effect/Walk.png", 12, 12, 1, 62, 64);
	rm->GetImages("Resource/Images/Effect/Smoke1.png", 12, 12, 1, 64, 66);
	rm->GetImages("Resource/Images/Effect/Smoke3.png", 12, 12, 1, 64, 64);
	rm->GetImages("Resource/Images/Effect/Smoke4.png", 12, 12, 1, 64, 64);
	rm->GetImages("Resource/Images/Effect/Smoke5.png", 12, 12, 1, 64, 64);
	rm->GetImages("Resource/Images/Effect/Smoke6.png", 12, 12, 1, 64, 64);
	rm->GetImages("Resource/Images/Effect/R_Hit.png", 12, 12, 1, 64, 66);
	rm->GetImages("Resource/Images/Effect/E_PuffAndStar.png", 60, 10, 6, 108, 116);
	rm->GetImages("Resource/Images/Effect/shine.png", 25, 5, 5, 64, 56);
	rm->GetImages("Resource/Images/Enemy5/Enemy5_Walk2.png", 24, 5, 5, 200, 200);
	rm->GetImages("Resource/Images/Enemy5/Enemy5_Death.png", 15, 5, 3, 200, 200);
	rm->GetImages("Resource/Images/Enemy4/Enemy4_Run.png", 12, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Enemy4/Enemy4_Walk.png", 24, 5, 5, 96, 96);
	rm->GetImages("Resource/Images/Enemy4/Enemy4_Death.png", 15, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Enemy3/Enemy3_Idle.png", 18, 5, 4, 96, 96);
	rm->GetImages("Resource/Images/Enemy3/Enemy3_Throw.png", 12, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Enemy3/Enemy3_Death.png", 15, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Enemy2/Enemy2_Run.png", 12, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Enemy2/Enemy2_Death.png", 15, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Enemy1/Enemy1_Walk.png", 24, 5, 5, 96, 96);
	rm->GetImages("Resource/Images/Enemy1/Enemy1_Death.png", 15, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Effect/boss_aura.png", 8, 8, 1, 120, 120);
	rm->GetImages("Resource/Images/Boss1/Boss1_Walk1.png", 24, 5, 5, 96, 96);
	rm->GetImages("Resource/Images/Boss1/Boss1_Idle1.png", 18, 5, 4, 96, 96);
	rm->GetImages("Resource/Images/Boss1/Boss1_Sliding1.png", 6, 5, 2, 96, 96);
	rm->GetImages("Resource/Images/Boss1/Boss1_Death1.png", 15, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Boss2/Boss2_Idle.png", 18, 5, 4, 96, 96);
	rm->GetImages("Resource/Images/Boss2/Boss2_Throw.png", 12, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Boss2/Boss2_Run.png", 12, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Boss2/Boss2_Run_Throw.png", 12, 5, 3, 96, 96);
	rm->GetImages("Resource/Images/Boss2/Boss2_Death.png", 15, 5, 3, 96, 96);

	//BGM、SE読み込み
	gamemain_bgm = rm->GetSounds("Resource/Sounds/BGM/Rail_train.mp3");
	game_clear_se = rm->GetSounds("Resource/Sounds/Direction/victory.mp3");
	game_over_se = rm->GetSounds("Resource/Sounds/Direction/deden.mp3");
	clap_se = rm->GetSounds("Resource/Sounds/Direction/大勢で拍手.mp3");
	coin_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	enter_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
	back_se = rm->GetSounds("Resource/Sounds/Hanahana/button.mp3");
	throw_se = rm->GetSounds("Resource/Sounds/Coin/Throw.mp3");
	cheers_se = rm->GetSounds("Resource/Sounds/Direction/歓声と拍手1.mp3");

	//他のオブジェクトが使うSE
	rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
	rm->GetSounds("Resource/Sounds/Hanahana/reel.mp3");
	rm->GetSounds("Resource/Sounds/Hanahana/big.mp3");
	rm->GetSounds("Resource/Sounds/shot.mp3");
	rm->GetSounds("Resource/Sounds/Player/Heal.mp3");
	rm->GetSounds("Resource/Sounds/Shop/金額表示.mp3");
	rm->GetSounds("Resource/Sounds/Shop/入店チャイム.mp3");


}

void InGameScene::MakeGameMainDraw()
{
	SetDrawScreen(gamemain_image);
	ClearDrawScreen();

	//背景画像描画
	DrawGraphF(-STAGE_SIZE - camera->GetCameraLocation().x, -STAGE_SIZE - camera->GetCameraLocation().y, bg_image, true);

	//オブジェクト描画
	objects->Draw();

	//チュートリアル中でない、且つ画面内に初期位置が移っていないなら
	Vector2D camera_loc = camera->GetCameraLocation();
	if (!tutorial->GetTutorialFlg() &&
		(camera_loc.x > 90 ||
			camera_loc.x < -SCREEN_WIDTH ||
			camera_loc.y > 90 ||
			camera_loc.y < -SCREEN_HEIGHT)
		)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBoxAA(guide_local_loc.x - guide_size.x / 2,
			guide_local_loc.y - guide_size.y / 2,
			guide_local_loc.x + guide_size.x / 2,
			guide_local_loc.y + guide_size.y / 2,
			0x000000, false);
		DrawRotaGraphF(guide_local_loc.x, guide_local_loc.y, 0.5f, 0.f, flower_image, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	SetFontSize(36);
	Vector2D hpbar_loc = { SCREEN_WIDTH / 2,SCREEN_HEIGHT-50 }; 
	Vector2D hp_bar_size = { BOSS_BAR_SIZE,30 };
	int count = 0;
	//ボスHP描画
	for (const auto boss : boss_hp)
	{
		count++;
		UserData::DrawStringCenter({ hpbar_loc.x,hpbar_loc.y - hp_bar_size.y-20 }, boss.name, 0xffffff);
		//HPゲージ内側
		DrawBoxAA(hpbar_loc.x - (hp_bar_size.x / 2),
			hpbar_loc.y - (hp_bar_size.y / 2),
			hpbar_loc.x + (hp_bar_size.x / 2),
			hpbar_loc.y + (hp_bar_size.y / 2),
			0x000000,
			true);

		//HPゲージ本体
		DrawBoxAA(hpbar_loc.x - (hp_bar_size.x / 2),
			hpbar_loc.y - (hp_bar_size.y / 2),
			hpbar_loc.x - (hp_bar_size.x / 2) + (boss.hp * (hp_bar_size.x / boss.max_hp)) + boss.hp_move,
			hpbar_loc.y + (hp_bar_size.y / 2),
			0xcccc00,
			true);

		//HPゲージ減少アニメーション
		DrawBoxAA(hpbar_loc.x - (hp_bar_size.x / 2) + (boss.hp * (hp_bar_size.x / boss.max_hp)),
			hpbar_loc.y - (hp_bar_size.y / 2),
			hpbar_loc.x - (hp_bar_size.x / 2) + (boss.hp * (hp_bar_size.x / boss.max_hp)) + boss.hp_move,
			hpbar_loc.y + (hp_bar_size.y / 2),
			0xff0000,
			true);

		//HPゲージ外枠
		DrawBoxAA(hpbar_loc.x - (hp_bar_size.x / 2),
			hpbar_loc.y - (hp_bar_size.y / 2),
			hpbar_loc.x + (hp_bar_size.x / 2),
			hpbar_loc.y + (hp_bar_size.y / 2),
			0xffffff,
			false);
		//座標をずらす
		hpbar_loc.y -= (hp_bar_size.y + 40);
	}

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

			SetFontSize(72);
			DrawBox((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) - 75, (SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) + 75, 0x000000, true);
			DrawBox((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) - 75, (SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) + 75, 0xffffff, false);
			UserData::DrawStringCenter({ (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 55 }, "GameEnd!", 0xffffff);
			SetFontSize(36);
			UserData::DrawStringCenter({ (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 20 }, "この後結果発表！", 0xffffff);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((255.f / G_END_ANIM_TIME) * change_result_delay));
			DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

			SetFontSize(72);
			DrawBox((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) - 75, (SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) + 75, 0x000000, true);
			DrawBox((SCREEN_WIDTH / 2) - 300, (SCREEN_HEIGHT / 2) - 75, (SCREEN_WIDTH / 2) + 300, (SCREEN_HEIGHT / 2) + 75, 0xffffff, false);
			UserData::DrawStringCenter({ (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) - 55 }, "GameEnd...", 0xffffff);
			SetFontSize(36);
			UserData::DrawStringCenter({ (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + 20 }, "この後結果発表", 0xffffff);
		}
	}

	//遷移時アニメーションフラグが立っていたら、アニメーション処理
	if (start_anim_flg)
	{
		Vector2D coin_loc = { SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 };
		int coin_size = 1350 - start_anim_timer * (1500 / G_START_ANIM_TIME);
		if (coin_size > 30)
		{
			//波動
			DrawCircleAA(coin_loc.x, coin_loc.y, coin_size - (start_anim_timer * 5) % coin_size + coin_size, 50, 0xffffff, false);

			UserData::DrawCoin(coin_loc, coin_size);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, coin_size * 10);
			UserData::DrawCoin(coin_loc, 80 - coin_size * 2, 255, 255, 255);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
		SetFontSize(32);
		if(tutorial->tuto_flg)UserData::DrawButtonAndString({ SCREEN_WIDTH - 120,SCREEN_HEIGHT - 30 }, XINPUT_BUTTON_A, ":早送り", 0x000000);
	}
	//時間経過ボーナス描画
	if (bonus_timer > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255.f / TIME_BONUS) * bonus_timer);
		SetFontSize(72);
		if (UserData::timer < SECOND_BONUS_TIME)
		{
			DrawGraph(0, 0, second_bonus_image,true);
		}
		else if (UserData::timer < FIRST_BONUS_TIME)
		{
			DrawGraph(0, 0, first_bonus_image, true);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//チュートリアルフラグが立っていたら、チュートリアル描画(照準チュートリアルの描画は別で)
	if (tutorial->GetTutorialFlg() && tutorial->GetNowTutorial() != TutoType::tAim)
	{
		tutorial->Draw();
	}
	//最低保証コインが出た時のメッセージ
	if (coin_spawn_once)
	{
		SetFontSize(24);
		DrawString((SCREEN_WIDTH / 2) + 100, (SCREEN_HEIGHT / 2) + 20, "あきらめないで！", (int)frame % 30 > 15 ? 0xcccc00 : 0xffff00);
	}
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
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
				ResourceManager::rPlaySound(game_clear_se, DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			if (!CheckSoundMem(game_over_se))
			{
				ResourceManager::rPlaySound(game_over_se, DX_PLAYTYPE_BACK);
			}
		}
		//ゲームメインBGMを停止
		StopSoundMem(gamemain_bgm);
	}
}

void InGameScene::SpawnItem()
{
	DebugInfomation::Add("frame", frame);

	//マップの中心に定期的にコインをスポーン
	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(GetRand(100)),(float)(GetRand(100))},Vector2D{40,40},eCOIN, 20.f });
	}
	//マップの中心にキラキラエフェクト
	if ((int)frame % 10 == 0)
	{
		objects->CreateEffect(elShine, { (float)(GetRand(100)),(float)(GetRand(100)) }, TRUE, 0xffff00);
	}
	//マップ内のどこかにランダムで箱をスポーン
	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(GetRand(3800)-1900),(float)(GetRand(3800) - 1900)},Vector2D{80,80},eBOX, 20.f });
	}
}
void InGameScene::SpawnEnemy()
{
	//画面外からランダムに一定周期でスポーン
	if ((int)frame % (100 -((DEFAULT_TIMELIMIT - UserData::timer)/300)) == 0)
	{
		//低確率で塊出現
		if (GetRand(30) == 0)
		{
			ObjectData data = GetEnemyData();
			for (int i = 0; i < 10; i++)
			{
				
				objects->CreateObject(data.type,
					{ data.loc.x + (GetRand(50) - 25),data.loc.y + (GetRand(50) - 25) },
					data.size,
					data.radius);
			}
		}
		else
		{
			objects->CreateObject(GetEnemyData());
		}
	}

	//指定の時間にボスをスポーン
	if (!boss_spawn_once[0] && (UserData::timer / 60) == BOSS1_SPAWN)
	{
		objects->CreateObject({ {-1000,0}, Vector2D{BOSS1_WIDTH,BOSS1_HEIGHT}, eBOSS1});
		boss_spawn_once[0] = true;
	}
	if (!boss_spawn_once[1] && (UserData::timer / 60) == BOSS2_SPAWN)
	{
		objects->CreateObject({ {1000,0}, Vector2D{ BOSS2_WIDTH,BOSS2_HEIGHT }, eBOSS2 });
		boss_spawn_once[1] = true;
	}
	if (!boss_spawn_once[2] && (UserData::timer / 60) == BOSS3_SPAWN)
	{
		objects->CreateObject({ {1,1}, Vector2D{ BOSS3_WIDTH,BOSS3_HEIGHT }, eBOSS3 });
		boss_spawn_once[2] = true;
	}
}

void InGameScene::CreatePopUp(Vector2D _location, string _text, int _text_color, float _move, int _life_span)
{
	ui->SetUIData(_location, _text, _text_color, _move, _life_span);
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
	//コインが0～69ならenemy1をスポーン
	if (coin < 70)
	{
		return GetEnemy(eENEMY1, 100);
	}
	//コインが70～150ならenemy1と2をランダムでスポーン
	if (coin < 150)
	{
		return GetEnemy(eENEMY1, 50, eENEMY2, 50);
	}
	//コインが150～299ならenemy2をスポーン
	if (coin < 300)
	{
		return GetEnemy(eENEMY2, 100);
	}
	//コインが300～499ならenemy1と2と3をスポーン
	if (coin < 500)
	{
		return GetEnemy(eENEMY1, 30,eENEMY2, 40, eENEMY3, 30);
	}
	//コインが500～699ならenemy1と2と4をスポーン
	if (coin < 700)
	{
		return GetEnemy(eENEMY1, 30, eENEMY2, 60, eENEMY4, 10);
	}
	//コインが700～999ならenemy1と2と5をスポーン
	if (coin < 1000)
	{
		return GetEnemy(eENEMY1, 20, eENEMY2, 60, eENEMY5, 20);
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
	if (rand <= _prob1)
	{
		//オブジェクト１を返す
		return _list1;
	}
	//ランダムな値がオブジェクト１、２の確率の合計を下回っていたら
	else if (rand <= _prob1 + _prob2)
	{
		//オブジェクト２
		return _list2;
	}
	//ランダムな(略)
	else if (rand <= _prob1 + _prob2 + _prob3)
	{
		//オブジェクト３
		return _list3;
	}
	//ランダムな(略)
	else if (rand <= _prob1 + _prob2 + _prob3 + _prob4)
	{
		//オブジェクト４
		return _list4;
	}
	//ランダムな(略)
	else if (rand <= _prob1 + _prob2 + _prob3 + _prob4 + _prob5)
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
	std::vector<int> deco_image;	//背景画像格納
	std::vector<std::vector<int>> bg_arran;	//背景画像配置

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	background_image = rm->GetImages("Resource/Images/BackGround/terrain_tiles_v2.png", 160, 10, 16, 64, 64);
	deco_image = rm->GetImages("Resource/Images/BackGround/decorations.png", 24, 3, 8, 24, 24);

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

	//ガイド表示用に初期地点の緑地エリアを保存する
	SetDrawScreen(flower_image);
	ClearDrawScreen();
	//コインが生成されるエリアを緑地にする
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			DrawGraph(x * 64, y * 64, background_image[40 + x + (y * 10)], TRUE);
		}
	}
	//コインが生成されるエリアに華を
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			DrawGraph(30 + x * 24 + GetRand(20),20 + y * 24 + GetRand(20), deco_image[GetRand(23)], TRUE);
		}
	}
	SetDrawScreen(bg_image);
	DrawGraph(1970, 1970, flower_image, FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

}

void InGameScene::BonusCoinUpdate()
{
	//一定時間経過でボーナスコイン投げ入れ
	if (bonus_timer % 10 == 0 &&
		((UserData::timer < FIRST_BONUS_TIME && first_bonus_count++ < FIRST_BONUS_NUM) ||
		(UserData::timer < SECOND_BONUS_TIME && second_bonus_count++ < SECOND_BONUS_NUM)))
	{
		Vector2D rand = GetRandLoc();
		Vector2D camera_center = { camera->GetCameraLocation().x + (SCREEN_WIDTH / 2),camera->GetCameraLocation().y + (SCREEN_HEIGHT / 2) };
		Vector2D rand_velocity = { ((camera_center.x - rand.x) + (GetRand(SCREEN_WIDTH - 200) - (SCREEN_WIDTH - 200) / 2)) / 10,
								   ((camera_center.y - rand.y) + (GetRand(SCREEN_HEIGHT - 200) - (SCREEN_HEIGHT - 200) / 2)) / 10 };
		objects->CreateObject(eCOIN, rand, { 40,40 }, 20.f, rand_velocity);
		bonus_timer = TIME_BONUS;
		ResourceManager::rPlaySound(throw_se, DX_PLAYTYPE_BACK);
		if (!CheckSoundMem(cheers_se))
		{
			ResourceManager::rPlaySound(cheers_se, DX_PLAYTYPE_BACK);
		}
	}

	//ボーナスコイン表示時間
	if (--bonus_timer < 0)
	{
		bonus_timer = 0;
	}
}

void InGameScene::CreateBonusDraw()
{
	first_bonus_image = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT,TRUE);
	second_bonus_image = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT,TRUE);

	SetFontSize(72);

	SetDrawScreen(first_bonus_image);
	ClearDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffff55, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawFormatString(SCREEN_WIDTH / 2 - GetDrawStringWidth("1/3 経過ボーナス！", strlen("1/3 経過ボーナス！")) / 2 + 1,
		SCREEN_HEIGHT / 2 - 200 + 1,
		0xffff00,
		"1/3 経過ボーナス！");
	DrawFormatString(SCREEN_WIDTH / 2 - GetDrawStringWidth("1/3 経過ボーナス！", strlen("1/3 経過ボーナス！")) / 2,
		SCREEN_HEIGHT / 2 - 200,
		0xffffff,
		"1/3 経過ボーナス！");

	SetDrawScreen(second_bonus_image);
	ClearDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xffff55, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawFormatString(SCREEN_WIDTH / 2 - GetDrawStringWidth("2/3 経過ボーナス！", strlen("2/3 経過ボーナス！")) / 2 + 1,
		SCREEN_HEIGHT / 2 - 200 + 1,
		0x555500,
		"2/3 経過ボーナス！");
	DrawFormatString(SCREEN_WIDTH / 2 - GetDrawStringWidth("2/3 経過ボーナス！", strlen("2/3 経過ボーナス！")) / 2,
		SCREEN_HEIGHT / 2 - 200,
		0xffffff,
		"2/3 経過ボーナス！");

	SetDrawScreen(DX_SCREEN_BACK);
}

void InGameScene::TutorialUpdate()
{
	//移動チュートリアルが終わっていたらコイン投げ入れ
	if (!tutorial->tuto_flg && tutorial->GetIsEndTutorial(TutoType::tMove) && (int)frame % 10 == 0 && tuto_coin_count++ < 20)
	{
		Vector2D rand = GetRandLoc();
		Vector2D camera_center = { camera->GetCameraLocation().x + (SCREEN_WIDTH / 2),camera->GetCameraLocation().y + (SCREEN_HEIGHT / 2) };
		Vector2D rand_velocity = { ((camera_center.x - rand.x) + (GetRand(SCREEN_WIDTH - 200) - (SCREEN_WIDTH - 200) / 2)) / 10,
								   ((camera_center.y - rand.y) + (GetRand(SCREEN_HEIGHT - 200) - (SCREEN_HEIGHT - 200) / 2)) / 10 };
		objects->CreateObject(eCOIN, rand, { 40,40 }, 20.f, rand_velocity);
		ResourceManager::rPlaySound(throw_se, DX_PLAYTYPE_BACK);
	}
	DebugInfomation::Add("move", tutorial->GetIsEndTutorial(TutoType::tMove));
	BonusCoinUpdate();

	//チュートリアルが終わっていないとタイマーが動かず、敵とコインが湧かないようにする
	if (tutorial->GetIsEndTutorial(TutoType::tAttack))
	{
		//アイテム生成
		SpawnItem();
		//制限時間減少(死んでいない＆ボスが居ないなら)
		auto it = std::find_if(boss_hp.begin(), boss_hp.end(), [](const BossHp& b) {return b.name == "Boss3"; });
		if (!UserData::is_dead && it == boss_hp.end())
		{
			UserData::timer--;
		}
		//if (!UserData::is_dead && boss_hp.size() == 0)UserData::timer--;
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
				Vector2D rand_velocity = { (camera->player_location.x - rand.x) / 7  ,
										   (camera->player_location.y - rand.y) / 7 };
				objects->CreateObject(eHEAL, rand, { 40,40 }, 20.f, rand_velocity);
				tuto_heal_once = true;
				tuto_heal_timer = 0;
				ResourceManager::rPlaySound(throw_se, DX_PLAYTYPE_BACK);
			}
		}
		//HPが回復したらリセット
		else
		{
			tuto_heal_once = false;
		}

	}

}	

void InGameScene::UpdateGuideLoc()
{
	//初期位置とプレイヤーの絶対座標を比較して進行方向を求める
	float rad;
	Vector2D velo;
	rad = atan2f(-camera->player_location.y, -camera->player_location.x);
	velo.x = cosf(rad);
	velo.y = sinf(rad);
	guide_loc = camera->player_location;
	int count = 0;
	//画面端に当たるまで移動を繰り返す
	while (1)
	{
		count++;
		guide_loc += velo;
		guide_local_loc = guide_loc - camera->GetCameraLocation();
		if (count > 40 &&
			(guide_local_loc.x - guide_size.x / 2 < 0 ||
			guide_local_loc.x + guide_size.x / 2 > SCREEN_WIDTH ||
			guide_local_loc.y - guide_size.y / 2 < 0 ||
			guide_local_loc.y + guide_size.y / 2 > SCREEN_HEIGHT))
		{
			break;
		}
	}
}

void InGameScene::SaveReplay()
{
	//一区間のメダルの差枚が＋50枚なら、良い場面として保存
	int num1 = UserData::coin_graph.back();
	int num2 = UserData::coin_graph[UserData::coin_graph.size() - 2];

	DebugInfomation::Add("num2", num2);

	if (num1 - num2 > 50)
	{
		int img = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetDrawScreen(img);
		ClearDrawScreen();
		DrawGraph(0, 0, gamemain_image, FALSE);
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		UserData::replay.push_back({(600 - UserData::timer)/60 ,img ,"大量獲得" });
	}
	if (num2 - num1 > 50)
	{
		int img = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetDrawScreen(img);
		ClearDrawScreen();
		DrawGraph(0, 0, gamemain_image, FALSE);
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		UserData::replay.push_back({ (600 - UserData::timer) / 60 ,img ,"大量失点" });
	}
}

void InGameScene::SetShopFlg(bool _flg, ObjectBase* _shop)
{ 
	shop_flg = _flg; 
	update_shop = _shop;	
}

void InGameScene::UpdateZoom()
{
	//ゴールの座標と倍率のどちらかでも、現在の数値と違っていたら、数値に合わせて移動や拡大を開始する
	//開始した時点から時間を測定し、指定の時間経過したら初期位置、初期倍率にむけて移動を開始する
	//{ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 };←初期位置
	if (now_gm_loc != goal_gm_loc)
	{
		now_gm_loc += (goal_gm_loc - now_gm_loc) / zoom_speed;
		//int型に丸めた時に座標が重なっていれば、完全に重なっているものとする
		if ((int)now_gm_loc.x == (int)goal_gm_loc.x && (int)now_gm_loc.y == (int)goal_gm_loc.y)
		{
			now_gm_loc = goal_gm_loc;
		}
	}
	if (now_gm_size != goal_gm_size)
	{
		now_gm_size += (goal_gm_size - now_gm_size) / zoom_speed;
		//int型に丸めた時に座標が重なっていれば、完全に重なっているものとする
		if ((int)(now_gm_size * 500) == (int)(goal_gm_size * 500))
		{
			now_gm_size = goal_gm_size;
		}
	}
	//初期座標に居ない場合、時間を測定し、指定の時間を越していれば、初期座標をゴールとして移動開始
	if ((int)now_gm_loc.x != SCREEN_WIDTH / 2 || (int)now_gm_loc.y != SCREEN_HEIGHT / 2 || now_gm_size != 1.f)
	{
		if (++zoom_time_count > zoom_time)
		{
			goal_gm_loc = { SCREEN_WIDTH / 2 ,SCREEN_HEIGHT / 2 };
			goal_gm_size = 1.f;
			zoom_time_count = 0;
		}
	}
}

void InGameScene::SetZoom(Vector2D _loc, float _size, int _time, float _speed)
{
	////1(通常倍率)を下回っていたら1にする
	//goal_gm_size = _size < 1.f ? 1.f : _size;
	//Vector2D loc = _loc;
	//Vector2D move_max = { SCREEN_WIDTH -  (SCREEN_WIDTH / 2 /goal_gm_size) ,SCREEN_HEIGHT - (SCREEN_HEIGHT / 2/ goal_gm_size) };
	//Vector2D move_min = { (SCREEN_WIDTH / 2 / goal_gm_size) ,(SCREEN_HEIGHT / 2 / goal_gm_size) };

	////反転
	//goal_gm_loc.x = SCREEN_WIDTH - goal_gm_loc.x;
	//goal_gm_loc.y = SCREEN_HEIGHT - goal_gm_loc.y;

	//画面外が映らないように調整
	//goal_gm_loc.x = loc.x > move_max.x ? move_max.x : loc.x;
	//goal_gm_loc.y = loc.y > move_max.y ? move_max.y : loc.y;
	//goal_gm_loc.x = goal_gm_loc.x < move_min.x ? move_min.x : goal_gm_loc.x;
	//goal_gm_loc.y = goal_gm_loc.y < move_min.y ? move_min.y : goal_gm_loc.y;

	goal_gm_size =2;
	//goal_gm_size = _size < 1.f ? 1.f : _size;
	const float scale = goal_gm_size/2;
	const float baseX = SCREEN_WIDTH ;
	const float baseY = SCREEN_HEIGHT;

	goal_gm_loc.x = (baseX - _loc.x) * scale;
	goal_gm_loc.y = (baseY - _loc.y) * scale;


	zoom_time = _time;
	zoom_time_count = 0;
	zoom_speed = _speed <= 0 ? 1 : _speed;
}

void InGameScene::SetBossHp(const char* _name, float _hp, float _hp_max, float _hp_move)
{
	boss_hp.push_back(BossHp{ (char*)_name,_hp,_hp_max,_hp_move });
}

void InGameScene::SetBossSpawnAnim(ObjectBase* _boss, Vector2D _spawn_loc, int _anim_time)
{
	now_anim_boss = _boss;
	boss_anim_flg = true;
	boss_anim_timer = _anim_time;
	boss_anim_count = 0;
	camera->camera_gaze_location = _spawn_loc;
	SetZoom({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }, 2, boss_anim_timer, 30);
}