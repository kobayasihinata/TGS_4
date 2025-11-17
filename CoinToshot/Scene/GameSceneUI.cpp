#include "DxLib.h"
#include "GameSceneUI.h"
#include "Camera/Camera.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/DebugInformation.h"
#include "../Object/Player/PlayerBullet.h"
#include "RankColor.h"
#include "../Scene/InGameScene.h"

void GameSceneUI::Initialize(InGameScene* _ingame)
{
	tutorial = Tutorial::Get();
	camera = Camera::Get();
	ingame = _ingame;

	frame = 0;
	bullet_image = MakeScreen(300, 225, TRUE);
	lock_image = MakeScreen(60, 75, TRUE);
	old_bullet_type = UserData::now_bullet;
	bullet_change_timer = 0;
	change_anim_move = 300.f / PLATER_BULLET_CHANGE_CD;
	change_anim_once = false;
	move_dir = false;
	old_bullet_num = 2;	//初期に所持している弾の種類数に合わせる
	new_bullet_flg = false;
	new_bullet_timer = 0;
	new_bullet_fade = 0;

	player_ui_loc = { SCREEN_WIDTH - 525,0 };
	now_coin_num = 0;
	old_coin_num = 0;
	first_bonus_timer = 0;
	second_bonus_timer = 0;
	rank_anim_flg = false;
	rank_timer = 0;
	old_rank_keep = 0;
	old_rank = now_rank = -1;
	damage_timer = 0;
	con_spawn = 1;

	//ロックの画像生成
	CreateLockImage();
	//紙吹雪の画像生成
	CreateConfettiImage();
	//ランキング画像の生成
	CreateRankingImage();

	ex_se_once = ex_anim_once = tutorial->GetBasicTuto();
	ex_anim_timer = 0;
	now_image = 0;
	now_aura_image = 0;
	ResourceManager* rm = ResourceManager::GetInstance();
	aura_image = rm->GetImages("Resource/Images/Effect/boss_aura.png", 8, 8, 1, 120, 120);
	ex_anim = rm->GetImages("Resource/Images/Effect/E_PuffAndStar.png", 60, 10, 6, 108, 116);
	ex_se = rm->GetSounds("Resource/Sounds/explsion_big.mp3");
	lock_se = rm->GetSounds("Resource/Sounds/lock.mp3");
	count_se = rm->GetSounds("Resource/Sounds/lock.mp3");
	unlock_se = rm->GetSounds("Resource/Sounds/Direction/成功音.mp3");
	unlock2_se = rm->GetSounds("Resource/Sounds/Direction/大勢で拍手.mp3");
	rank_down_se = rm->GetSounds("Resource/Sounds/Direction/スタジアムの歓声2.mp3");

	shine_image = rm->GetImages("Resource/Images/Effect/yellow_shine.png", 40, 8, 5, 96, 96);
	now_shine_image = 0;
}

void GameSceneUI::Update()
{
	//フレーム計測
	if (++frame >= 6000)
	{
		frame = 0;
	}

	//オーラ更新
	if (frame % 5 == 0)
	{
		if (++now_aura_image >= aura_image.size())
		{
			now_aura_image = 0;
		}
	}

	for (auto& ui_data : ui_data)
	{
		//時間を過ぎたら消す
		if (++ui_data.life_count >= ui_data.life_span)
		{
			delete_ui_data.push_back(ui_data);
		}
	}

	//オブジェクト配列から削除する処理
	for (const auto& delete_data : delete_ui_data)
	{
		for (auto it = ui_data.begin(); it != ui_data.end();)
		{
			if (*it == delete_data)
			{
				it = ui_data.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	//フラグが立っているなら、紙吹雪を生成
	if (confetti_flg)
	{
		if (frame % con_spawn == 0)
		{
			confetti_data.push_back(GetConfettiData());
			con_spawn = GetRand(5) + 1;
		}
	}

	//紙吹雪更新
	for (auto& con_data : confetti_data)
	{
		//角度と目標角度の差
		float difference = (con_data.move_radian - con_data.radian);
		con_data.radian += difference / 20;
		//角度の調整が完了していたら、新しい目的地を設定する
		if (fabsf(difference) <= 0.05f)
		{
			con_data.move_radian = ((float)GetRand(100) / 100) + 1.f;
		}
		//移動角度に応じてvelocity変更
		con_data.velocity.x = con_data.speed * cosf(con_data.radian);
		con_data.velocity.y = con_data.speed * sinf(con_data.radian);

		//移動
		con_data.location += con_data.velocity;

		//画面外に出たら消す
		if (con_data.location.y > SCREEN_HEIGHT + 30)
		{
			delete_confetti_data.push_back(con_data);
		}
	}

	//配列から削除する処理
	for (const auto& delete_data : delete_confetti_data)
	{
		for (auto it = confetti_data.begin(); it != confetti_data.end();)
		{
			if (*it == delete_data)
			{
				it = confetti_data.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}


	//最大値の更新
	if (UserData::attraction_timer > max_attraction)max_attraction = UserData::attraction_timer;
	//ゲージのリセット
	if (UserData::attraction_timer <= 0)max_attraction = 0;


	//弾種類が増えているならアニメーション開始
	if (old_bullet_num < UserData::get_bullet.size())
	{
		new_bullet_flg = true;
		ingame->SetZoom({ SCREEN_WIDTH / 2,0 }, 2, NEW_BULLET_ANIM, 30);
	}

	//一フレーム前の弾所持数更新
	old_bullet_num = UserData::get_bullet.size();

	//新弾獲得演出中なら、アニメーションしない
	if (!new_bullet_flg)
	{
		//弾の種類が変わっているなら、アニメーションする
		if (UserData::get_bullet[old_bullet_type] != UserData::get_bullet[UserData::now_bullet])
		{
			if (!change_anim_once)
			{
				move_dir = CheckMoveDirection(UserData::get_bullet[UserData::now_bullet], UserData::get_bullet[old_bullet_type]);
				change_anim_once = true;
			}
			//アニメーション終了条件
			if (++bullet_change_timer >= PLATER_BULLET_CHANGE_CD)
			{
				old_bullet_type = UserData::now_bullet;
				bullet_change_timer = 0;
			}

		}
		else
		{
			//方向決定リセット
			change_anim_once = false;
		}
	}
	//新弾獲得演出
	else
	{
		//一定時間経過で演出終了
		if (++new_bullet_timer > NEW_BULLET_ANIM)
		{
			new_bullet_timer = 0;
			new_bullet_flg = false;
		}
		//透明度の値を上げる
		if (new_bullet_timer <= NEW_BULLET_ANIM_FADE)
		{
			new_bullet_fade += (255.f / NEW_BULLET_ANIM_FADE);
		}
		//透明度の値を下げる
		if (new_bullet_timer > NEW_BULLET_ANIM_1 && new_bullet_timer <= NEW_BULLET_ANIM_1 + NEW_BULLET_ANIM_FADE)
		{
			new_bullet_fade -= (255.f / NEW_BULLET_ANIM_FADE);
		}
		//弾の種類を変える
		if (new_bullet_timer >= NEW_BULLET_ANIM_1 &&UserData::now_bullet != UserData::get_bullet.size()-1)
		{
			UserData::now_bullet = UserData::get_bullet.size() - 1;
			old_bullet_type = UserData::now_bullet;
			ResourceManager::rPlaySound(unlock_se, DX_PLAYTYPE_BACK);
			ResourceManager::rPlaySound(unlock2_se, DX_PLAYTYPE_BACK);
		}
	}

	//爆発音再生
	if (UserData::can_bullet_change_flg && !ex_se_once)
	{
		ResourceManager::rPlaySound(ex_se, DX_PLAYTYPE_BACK);
		ex_se_once = true;
	}
	//爆発アニメーション
	if (!ex_anim_once && UserData::can_bullet_change_flg && ++ex_anim_timer % 2 == 0)
	{
		if (now_image < ex_anim.size() - 1)
		{
			now_image++;
		}
		else
		{
			//アニメーション終了
			ex_anim_once = true;
		}
	}

	//弾種類変更解禁前にトリガーを入力した時のロック演出
	if (!UserData::can_bullet_change_flg &&
		(UserData::CheckBulletChangeButtonLeft() || UserData::CheckBulletChangeButtonRight()))
	{
		ResourceManager::rPlaySound(lock_se, DX_PLAYTYPE_BACK);
	}
	
	//ダメージ演出用時間測定
	if (UserData::player_damage_flg)
	{
		damage_timer++;
	}
	else
	{
		damage_timer = 0;
	}

	//現在のランキング計測
	old_rank = now_rank;
	now_rank = -1;
	for (int i = 0; i < 10; i++)
	{
		if (UserData::ranking_data[i].coin < UserData::coin + ((DEFAULT_TIMELIMIT - UserData::timer) / 60) + (UserData::player_hp * 10))
		{
			now_rank = UserData::ranking_data[i].num;
			break;
		}
	}

	//ランキングが変わっているならアニメーション開始
	if (now_rank != old_rank)
	{
		rank_anim_flg = true;
		rank_timer = 0;
		old_rank_keep = old_rank;

		//現在のランクの方が高いなら良いSE
		if ((now_rank < old_rank && now_rank != -1) || old_rank == -1)
		{    
			ResourceManager::rPlaySound(unlock2_se, DX_PLAYTYPE_BACK);
		}
		else
		{
			ResourceManager::rPlaySound(rank_down_se, DX_PLAYTYPE_BACK);
		}
	}

	if (rank_anim_flg)
	{
		if (++rank_timer > RANKUP_TIMER)
		{
			rank_anim_flg = false;
		}
		if (++now_shine_image > 39)
		{
			now_shine_image = 0;
		}
	}

	player_ui_loc.x = SCREEN_WIDTH - 525 + camera->GetRandImpact();
	player_ui_loc.y = camera->GetRandImpact();

	DebugInfomation::Add("impact", camera->impact);

	//弾種類見た目生成
	CreateBulletTypeImage();

	//削除したオブジェクトは消去
	delete_ui_data.clear();
	delete_confetti_data.clear();

	//１フレーム前のコイン枚数を格納
	old_coin_num = now_coin_num;
	now_coin_num = UserData::coin;
}

void GameSceneUI::Draw()const
{
	//カメラ座標取得
	Camera* camera = Camera::Get();

	//プレイヤーが弾種類UIと被ったら透過する
	if (camera->player_location.x - camera->GetCameraLocation().x > SCREEN_WIDTH - 520 &&
		camera->player_location.y - camera->GetCameraLocation().y < 150)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	}
	//プレイヤー情報描画
	DrawPlayerUI();

	//文字透過リセット
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//コイン吸い寄せ処理の描画
	if (UserData::attraction_timer > 0)
	{
		DrawBox(camera->player_location.x - camera->GetCameraLocation().x - max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 20,
			camera->player_location.x - camera->GetCameraLocation().x + max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y- 10,
			0x000000, true);
		DrawBoxAA(camera->player_location.x - camera->GetCameraLocation().x - max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 20,
			camera->player_location.x - camera->GetCameraLocation().x + max_attraction / 20 - ((max_attraction - UserData::attraction_timer)/10),
			camera->player_location.y - camera->GetCameraLocation().y - 10,
			0x00ffff, true);
		DrawBox(camera->player_location.x - camera->GetCameraLocation().x - max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 20,
			camera->player_location.x - camera->GetCameraLocation().x + max_attraction / 20,
			camera->player_location.y - camera->GetCameraLocation().y - 10,
			0xffffff, false);
	}

	Vector2D button_lt = { SCREEN_WIDTH / 2 - 195, 45 };
	Vector2D button_rt = { SCREEN_WIDTH / 2 + 195, 45 };
	//ボタンを押したら画像を変える
	if (UserData::control_type == 0)
	{
		DrawRotaGraphF(button_lt.x, button_lt.y, 1.5f, 0, UserData::button_image[(int)InputPad::OnButton(L_TRIGGER)][L_TRIGGER], true);
		DrawRotaGraphF(button_rt.x, button_rt.y, 1.5f, 0, UserData::button_image[(int)InputPad::OnButton(R_TRIGGER)][R_TRIGGER], true);
	}
	else if(UserData::control_type == 1)
	{
		DrawRotaGraphF(button_lt.x, button_lt.y, 1.f, 0, UserData::button_image[(int)InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER)][XINPUT_BUTTON_LEFT_SHOULDER], true);
		DrawRotaGraphF(button_rt.x, button_rt.y, 1.f, 0, UserData::button_image[(int)InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER)][XINPUT_BUTTON_RIGHT_SHOULDER], true);
	}

	//弾種類を変えられない状態なら、ボタンの上に×を描画
	if (!UserData::can_bullet_change_flg)
	{
		if (UserData::CheckBulletChangeButtonLeft())
		{
			button_lt.x += 5;
		}
		if (UserData::CheckBulletChangeButtonRight())
		{
			button_rt.x += 5;
		}
		DrawRotaGraphF(button_lt.x, button_lt.y, 1.f, 0, lock_image, TRUE);
		DrawRotaGraphF(button_rt.x, button_rt.y, 1.f, 0, lock_image, TRUE);
	}

	//爆発アニメーション
	if (!ex_anim_once && UserData::can_bullet_change_flg)
	{
		DrawRotaGraphF(SCREEN_WIDTH / 2 - 195, 30, 1.f, 0, ex_anim[now_image], TRUE);
		DrawRotaGraphF(SCREEN_WIDTH / 2 + 195, 30, 1.f, 0, ex_anim[now_image], TRUE);
	}

	//プレイヤーが弾種類UIと被ったら透過する
	if (camera->player_location.x - camera->GetCameraLocation().x > (SCREEN_WIDTH / 2) - 150 &&
		camera->player_location.x - camera->GetCameraLocation().x < (SCREEN_WIDTH / 2) + 150 &&
		camera->player_location.y - camera->GetCameraLocation().y < 200)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	}
	//弾の種類描画
	if (new_bullet_flg && new_bullet_timer < NEW_BULLET_ANIM_1)
	{
		DrawRotaGraph(SCREEN_WIDTH / 2 +camera->GetRandImpact()+ GetRand(10) - 5, 112+camera->GetRandImpact()+ GetRand(10) - 5, 1.0f, 0, bullet_image, TRUE);
	}
	else
	{
		DrawRotaGraph(SCREEN_WIDTH / 2 + camera->GetRandImpact(), 112 + camera->GetRandImpact(), 1.0f, 0, bullet_image, TRUE);
	}

	SetFontSize(36);

	//ui_dataの描画
	for (const auto ui_data : ui_data)
	{
		//フォントサイズ変更
		//SetFontSize(ui_data.font_size);
		//文字透過設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (255 / ui_data.life_span) * ui_data.life_count);
		DrawFormatStringF(ui_data.location.x - camera->GetCameraLocation().x + 1.f,
			ui_data.location.y - camera->GetCameraLocation().y - (ui_data.life_count * ui_data.move_speed) + 1.f,
			0x000000,
			"%s",
			ui_data.text.c_str()
		);
		DrawFormatStringF(ui_data.location.x - camera->GetCameraLocation().x,
			ui_data.location.y - camera->GetCameraLocation().y - (ui_data.life_count * ui_data.move_speed),
			ui_data.text_color,
			"%s",
			ui_data.text.c_str()
		);
	}
	int i = 0;
	int size = GetFontSize();

	//文字透過リセット
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//紙吹雪の描画
	for (const auto con_data : confetti_data)
	{
		DrawRotaGraphF(con_data.location.x, con_data.location.y, 1.f, con_data.radian, con_data.image, TRUE);
	}

	//ダメージ描画
	if (UserData::player_damage_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120 - (damage_timer*5));
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xff0000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//弾獲得アニメーション
	if (new_bullet_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)new_bullet_fade);
		DrawBox(SCREEN_WIDTH / 2 - 150,
			0,
			SCREEN_WIDTH / 2 + 150,
			150, 0xffffff, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		if (new_bullet_timer > NEW_BULLET_ANIM_1)
		{
			DrawCircle(SCREEN_WIDTH / 2, 
				75,
				(new_bullet_timer - NEW_BULLET_ANIM_1) * 10,
				0xffffff,false);
			UserData::DrawStringCenter({ SCREEN_WIDTH / 2,160 }, "新しい弾をゲット！", new_bullet_timer % 30 > 15 ? 0xffffff : 0xff0000);
		}
	}
}

void GameSceneUI::SetUIData(Vector2D _location, string _text, int _text_color, float _move, int _life_span)
{
	UIData data;
	data.life_count = 0;
	data.life_span = _life_span;
	data.location = _location;
	data.move_speed = _move;
	data.text = _text;
	data.text_color = _text_color;

	ui_data.push_back(data);
}

ConfettiData GameSceneUI::GetConfettiData()const
{
	ConfettiData ret;
	ret.location.x = GetRand(SCREEN_WIDTH + 200) - 100;
	ret.location.y = -20;
	ret.image = confetti_image[GetRand(7)];
	ret.radian = ((float)GetRand(100) / 100) + 1.f;
	ret.move_radian = ((float)GetRand(100) / 100) + 1.f;
	ret.speed = ((float)GetRand(500) / 100) + 2;
	return ret;
}

void GameSceneUI::CreateBulletTypeImage()const
{
	SetDrawScreen(bullet_image);
	ClearDrawScreen();

	int draw_color;
	//撃てない弾のUIは薄暗くする
	if (pBullet[UserData::get_bullet[UserData::now_bullet]].cost <= UserData::coin || new_bullet_flg)
	{
		draw_color = 0xffffff;
	}
	else
	{
		draw_color = 0xaaaaaa;
		//撃てない旨を伝える
		if (frame % 30 >= 15)
		{
			SetFontSize(32);
			DrawString(150 - GetDrawFormatStringWidth("コイン不足！") / 2, 160, "コイン不足！", 0xff0000);
		}
	}

	//弾変更が有れば変更前と変更後の箱を描画
	if (UserData::get_bullet[old_bullet_type] != UserData::get_bullet[UserData::now_bullet])
	{
		//変更前と変更後を比べて、右にアニメーションするか左にアニメーションするか判断
		if (move_dir)
		{
			//右移動
			DrawBullet({ change_anim_move * bullet_change_timer - 300, GetRand(1)}, UserData::get_bullet[UserData::now_bullet], UserData::get_bullet_cd[UserData::now_bullet]);
			DrawBullet({ change_anim_move * bullet_change_timer,  GetRand(1) }, UserData::get_bullet[old_bullet_type], UserData::get_bullet_cd[old_bullet_type]);
		}
		else
		{
			//左移動
			DrawBullet({ change_anim_move * -bullet_change_timer,  GetRand(1) }, UserData::get_bullet[old_bullet_type], UserData::get_bullet_cd[old_bullet_type]);
			DrawBullet({ (change_anim_move * -bullet_change_timer) + 300,  GetRand(1) }, UserData::get_bullet[UserData::now_bullet],UserData::get_bullet_cd[UserData::now_bullet]);
		} 
	}
	//変更が無いなら通常の描画
	else
	{
		DrawBullet({ 0, 0 }, UserData::get_bullet[UserData::now_bullet], UserData::get_bullet_cd[UserData::now_bullet]);
	}

	//文字大きさ設定
	SetFontSize(36);

	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::CreateLockImage()const
{
	SetDrawScreen(lock_image);
	ClearDrawScreen();
	
	DrawBox(0, 37, 60, 75, 0xff0000, TRUE);
	DrawCircle(30, 37, 24, 0xff0000, FALSE);
	DrawCircle(30, 37, 23, 0xff0000, FALSE);
	DrawCircle(30, 37, 22, 0xff0000, FALSE);

	DrawCircle(30, 50, 8, 0x000000, TRUE);
	DrawBox(27, 50, 33, 70, 0x000000, TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::CreateConfettiImage()
{
	//色格納
	int color[8] = { 0xff0000,0xffaa00,0xffff00,0xaaff00,0x00ff00,0x00ffff,0x0000ff,0xff00ff };
	for (int i = 0; i < 8; i++)
	{
		confetti_image[i] = MakeScreen(24, 24, FALSE);
		SetDrawScreen(confetti_image[i]);
		ClearDrawScreen();

		DrawBox(0, 0, 24, 24, color[i], TRUE);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::CreateRankingImage()
{
	int rank_width = 400;
	int rank_height = 150;
	int num_width = 100;
	int num_height = 75;

	for (int i = 0; i < 2; i++)
	{
		rank_image[i] = MakeScreen(rank_width, rank_height, TRUE);
	}
	for (int i = 0; i < 11; i++)
	{
		rank_num_image[i] = MakeScreen(num_width, num_height,TRUE);
	}
	SetFontSize(48);

	//ランクアップ
	SetDrawScreen(rank_image[0]);
	ClearDrawScreen();
	//DrawBox(0, 0, rank_width, rank_height, 0xff0000, FALSE);
	DrawStringF(rank_width / 2 - GetDrawStringWidth("RANK UP!!!", strlen("RANK UP!!!")) / 2, 10, "RANK UP!!!", 0xffffff);
	DrawString(rank_width / 2 - GetDrawStringWidth(" → ", strlen(" → ")) / 2, 80, " → ", 0xffffff);

	//ランクダウン
	SetDrawScreen(rank_image[1]);
	ClearDrawScreen();
	//DrawBox(0, 0, rank_width, rank_height, 0xff0000, FALSE);
	DrawStringF(rank_width / 2 - GetDrawStringWidth("RANK DOWN...", strlen("RANK DOWN...")) / 2, 10, "RANK DOWN...", 0x0000aa);
	DrawString(rank_width / 2 - GetDrawStringWidth(" → ", strlen(" → ")) / 2, 80, " → ", 0xffffff);

	//ランキング圏外
	SetDrawScreen(rank_num_image[0]);
	ClearDrawScreen();
	//DrawBox(0, 0, num_width, num_height, 0xff0000, FALSE);
	DrawStringF(num_width / 2 - GetDrawStringWidth("圏外", strlen("圏外")) / 2, 0, "圏外", 0x000000);

	//数字
	for (int i = 1; i < 11; i++)
	{
		SetDrawScreen(rank_num_image[i]);
		ClearDrawScreen();
		//DrawBox(0, 0, num_width, num_height, 0xff0000, FALSE);
		DrawFormatStringF(num_width / 2 - GetDrawFormatStringWidth("%d位", i)/2, 0, rank_color[i - 1], "%d位", i);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::DrawBullet(Vector2D _loc, int _type, int _cd)const
{
	//撃てない弾のUIは薄暗くする
	int draw_color = pBullet[_type].cost <= UserData::coin || new_bullet_flg? GetColor(pBullet[_type].color[0], pBullet[_type].color[1], pBullet[_type].color[2]) : 0xaaaa55;

	//弾の種類描画
	DrawBox(_loc.x, _loc.y, _loc.x + 300, _loc.y + 150, 0x777722, true);
	DrawBox(_loc.x, _loc.y, _loc.x + 300, _loc.y + 150, draw_color, false);

	SetFontSize(70);
	DrawFormatString(_loc.x + 150 - GetDrawFormatStringWidth(pBullet[_type].name) / 2, _loc.y, draw_color, "%s", pBullet[_type].name);

	SetFontSize(30);
	UserData::DrawCoin({ _loc.x + 115, _loc.y + 120 }, 25);
	DrawFormatString(_loc.x + 135, _loc.y + 110, draw_color, " - %d", pBullet[_type].cost);
	//DrawFormatString(_loc.x + 90, _loc.y + 70, draw_color, "power:%d", (int)pBullet[_type].damage);
	
	//弾のクールダウン描画
	if (_cd > 0)
	{
		pBullet[_type].life_span;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
		DrawBoxAA(_loc.x, _loc.y, _loc.x + (300.f / pBullet[_type].cooldown) * _cd, _loc.y + 150, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

void GameSceneUI::DrawPlayerUI()const
{
	SetFontSize(36);
	int width = 200;
	DrawQuadrangle(player_ui_loc.x - width+170, player_ui_loc.y,
		player_ui_loc.x + 630, player_ui_loc.y,
		player_ui_loc.x + 555, player_ui_loc.y + 120,
		player_ui_loc.x - width +200, player_ui_loc.y + 120,
		0x333300, TRUE);
	DrawQuadrangle(player_ui_loc.x - width +180, player_ui_loc.y, 
		player_ui_loc.x + 600, player_ui_loc.y,
		player_ui_loc.x + 525, player_ui_loc.y + 125,
		player_ui_loc.x - width +210, player_ui_loc.y + 125,
		0x666600, TRUE);
	DrawFormatString(player_ui_loc.x - width +240, player_ui_loc.y+22, 0xffffff, "HP:%d", (int)(UserData::player_hp));
	//ボスによってタイマーが停止していたら、エフェクトを出す
	if (ingame->GetBossTimerStop())
	{
		DrawRotaGraphF(player_ui_loc.x - GetDrawFormatStringWidth("TIME:%d", (int)(UserData::timer / 60)) + 450, player_ui_loc.y + 42, 1.f, 0, aura_image[now_aura_image], TRUE);

	}
	DrawFormatString(player_ui_loc.x - GetDrawFormatStringWidth("TIME:%d", (int)(UserData::timer/60))+325, player_ui_loc.y+22, 0xffffff, "TIME:%d", (int)(UserData::timer/60));
	
	int coin_text_color = 0xffffff;
	//コイン加算時のアニメーション
	if (now_coin_num != old_coin_num)
	{
		//コインが減ったか増えたかでフォントの大きさと色を変える
		SetFontSize(GetFontSize() + (now_coin_num > old_coin_num ? 3 : -3));
		coin_text_color = now_coin_num > old_coin_num ? 0xffff00 : 0xff0000;
	}
	else
	{
		if (GetFontSize() != 36)
		{
			SetFontSize(36);
		}
	}

	UserData::DrawCoin({ player_ui_loc.x - GetDrawFormatStringWidth("×%d", UserData::coin) + 480, 
		player_ui_loc.y + 45 }, 
		30);
	DrawFormatString(player_ui_loc.x - GetDrawFormatStringWidth("×%d", UserData::coin) + 510, 
		player_ui_loc.y + 22, 
		coin_text_color,
		"×%d", 
		UserData::coin);

	SetFontSize(48);

	//ランキング描画
	DrawRanking();

	SetFontSize(36);
	//一時停止可能な時だけ表示する
	if (!tutorial->GetTutorialFlg())
	{
		DrawString(75, 15, ": 一時停止", 0x000000);
		UserData::DrawButtonImage({45, 30 }, XINPUT_BUTTON_START, 75);
	}

	//残り時間１０秒以下なら、カウントダウンを描画する
	if (UserData::timer < 600 && UserData::timer > 0)
	{
		SetFontSize(120 - (UserData::timer % 60 * 2));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((UserData::timer % 60) * 6));
		DrawFormatString(SCREEN_WIDTH / 2 - GetDrawFormatStringWidth("%d", UserData::timer / 60) / 2, SCREEN_HEIGHT / 2 - GetFontSize() / 2 - 100, GetColor(255,(UserData::timer/3),0), "%d", (UserData::timer / 60));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		//カウントダウンSE
		if (UserData::timer % 60 == 0)
		{
			ResourceManager::rPlaySound(count_se, DX_PLAYTYPE_BACK);
		}
	}
}

bool GameSceneUI::CheckMoveDirection(int _now, int _old)const
{
	//右端から左端へ移動する時の判断
	if (UserData::CheckBulletChangeButtonRight())return false;
	//左端から右端へ移動する時の判断
	if (UserData::CheckBulletChangeButtonLeft())return true;
	//現在の項目が前の項目より小さいなら右
	if (_now < _old)return true;
	//現在の項目が前の項目より大きいなら左
	if (_now > _old) return false;
	//どれでもないなら
	return -1;
}

void GameSceneUI::DrawRanking()const
{
	//新記録なら
	if (now_rank > -1)
	{
		SetFontSize(36);
		DrawString(SCREEN_WIDTH - GetDrawStringWidth("新記録！", strlen("新記録！")) - 145,
			player_ui_loc.y + 70,
			"新記録！",
			frame % 30 > 15 ? 0xff0000 : 0xffffff);
		DrawFormatString(SCREEN_WIDTH - GetDrawStringWidth("新記録！", strlen("新記録！")) - 25,
			player_ui_loc.y + 70,
			frame % 30 > 15 ? 0xff0000 : 0xffffff,
			"現在%d位!",
			now_rank);
	}
	//順位更新アニメーション
	if (rank_anim_flg)
	{
		if (rank_timer > RANKUP_TIMER - 30)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - (rank_timer - (RANKUP_TIMER-30)) * (255 / 30));
		}
		Vector2D r_loc = { (SCREEN_WIDTH / 2) - 180,(SCREEN_HEIGHT / 2) + 150 };
		bool is_rankup = old_rank_keep > now_rank;	//falseならランキングが下がった処理
		if (frame % 30 < 15)DrawGraphF(r_loc.x, r_loc.y, is_rankup || old_rank_keep == -1 ? rank_image[0] : rank_image[1], TRUE);
		//上位3位なら輝き
		if (now_rank < 4 && is_rankup && now_rank != -1)
		{
			DrawRotaGraphF(r_loc.x + 300, r_loc.y + 117, 1.3f, 0, shine_image[now_shine_image], TRUE);
		}
		//ランキング変動演出
		if (old_rank_keep != -1)
		{
			DrawGraphF(r_loc.x+50, r_loc.y + 80, rank_num_image[old_rank_keep], TRUE);

		}
		else
		{
			DrawGraphF(r_loc.x + 50, r_loc.y + 80, rank_num_image[0], TRUE);
		}
		if (now_rank != -1)
		{
			DrawGraphF(r_loc.x + 250, r_loc.y + 80, rank_num_image[now_rank], TRUE);
		}
		else
		{
			DrawGraphF(r_loc.x + 250, r_loc.y + 80, rank_num_image[0], TRUE);
		}
		if (rank_timer > RANKUP_TIMER - 30)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}
}