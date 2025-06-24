#include "DxLib.h"
#include "GameSceneUI.h"
#include "Camera/Camera.h"
#include "../Utility/ResourceManager.h"
#include "../Utility/DebugInformation.h"
#include "../Object/Player/PlayerBullet.h"

void GameSceneUI::Initialize()
{
	tutorial = Tutorial::Get();

	frame = 0;
	bullet_image = MakeScreen(200, 150, TRUE);
	lock_image = MakeScreen(40, 50, TRUE);
	old_bullet_type = UserData::bullet_type;
	bullet_change_timer = 0;
	change_anim_move = 200.f / PLATER_BULLET_CHANGE_CD;
	player_ui_loc = { SCREEN_WIDTH - 350,0 };
	now_coin_num = 0;
	old_coin_num = 0;
	first_bonus_timer = 0;
	second_bonus_timer = 0;
	damage_timer = 0;
	con_spawn = 1;
	//ロックの画像生成
	CreateLockImage();

	//紙吹雪の画像生成
	CreateConfettiImage();

	ex_anim_once =false;
	ex_se_once = false;
	ex_anim_timer = 0;
	now_image = 0;
	ResourceManager* rm = ResourceManager::GetInstance();
	ex_anim = rm->GetImages("Resource/Images/Effect/E_PuffAndStar.png", 60, 10, 6, 108, 116);
	ex_se = rm->GetSounds("Resource/Sounds/explsion_big.mp3");
	lock_se = rm->GetSounds("Resource/Sounds/lock.mp3");
	count_se = rm->GetSounds("Resource/Sounds/lock.mp3");
}

void GameSceneUI::Update()
{
	//フレーム計測
	if (++frame >= 6000)
	{
		frame = 0;
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

	//弾の種類が変わっているなら、アニメーションする
	if (old_bullet_type != UserData::bullet_type)
	{
		//アニメーション終了条件
		if (++bullet_change_timer >= PLATER_BULLET_CHANGE_CD)
		{
			old_bullet_type = UserData::bullet_type;
			bullet_change_timer = 0;
		}
	}

	//爆発音再生
	if (UserData::can_bullet_change_flg && !ex_se_once)
	{
		PlaySoundMem(ex_se, DX_PLAYTYPE_BACK);
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
		(InputPad::OnButton(L_TRIGGER) || InputPad::OnButton(R_TRIGGER)))
	{
		PlaySoundMem(lock_se, DX_PLAYTYPE_BACK);
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
	int old = GetFontSize();
	//カメラ座標取得
	Camera* camera = Camera::Get();

	//プレイヤーが弾種類UIと被ったら透過する
	if (camera->player_location.x - camera->GetCameraLocation().x > SCREEN_WIDTH - 350 &&
		camera->player_location.y - camera->GetCameraLocation().y < 100)
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

	Vector2D button_lt = { SCREEN_WIDTH / 2 - 130, 30 };
	Vector2D button_rt = { SCREEN_WIDTH / 2 + 130, 30 };
	//ボタンを押したら画像を変える
	DrawRotaGraphF(button_lt.x, button_lt.y, 1.f,0,UserData::button_image[(int)InputPad::OnPressed(L_TRIGGER)][L_TRIGGER], true);
	DrawRotaGraphF(button_rt.x, button_rt.y, 1.f,0,UserData::button_image[(int)InputPad::OnPressed(R_TRIGGER)][R_TRIGGER], true);

	//弾種類を変えられない状態なら、ボタンの上に×を描画
	if (!UserData::can_bullet_change_flg)
	{
		if (InputPad::OnButton(L_TRIGGER))
		{
			button_lt.x += 5;
		}
		if (InputPad::OnButton(R_TRIGGER))
		{
			button_rt.x += 5;
		}
		DrawRotaGraphF(button_lt.x, button_lt.y, 1.f, 0, lock_image, TRUE);
		DrawRotaGraphF(button_rt.x, button_rt.y, 1.f, 0, lock_image, TRUE);
	}

	//爆発アニメーション
	if (!ex_anim_once && UserData::can_bullet_change_flg)
	{
		DrawRotaGraphF(SCREEN_WIDTH / 2 - 130, 30, 1.f, 0, ex_anim[now_image], TRUE);
		DrawRotaGraphF(SCREEN_WIDTH / 2 + 130, 30, 1.f, 0, ex_anim[now_image], TRUE);
	}

	//プレイヤーが弾種類UIと被ったら透過する
	if (camera->player_location.x - camera->GetCameraLocation().x > (SCREEN_WIDTH / 2) - 100 &&
		camera->player_location.x - camera->GetCameraLocation().x < (SCREEN_WIDTH / 2) + 100 &&
		camera->player_location.y - camera->GetCameraLocation().y < 140)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	}
	//弾の種類描画
	DrawRotaGraph(SCREEN_WIDTH / 2, 75, 1.0f, 0, bullet_image, TRUE);

	SetFontSize(24);

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
	SetFontSize(old);
}

void GameSceneUI::SetUIData(Vector2D _location, string _text, int _text_color, float _move, int _life_span, int _font_size)
{
	UIData data;
	data.font_size = _font_size;
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
	if (pBullet[UserData::bullet_type].cost <= UserData::coin)
	{
		draw_color = 0xffffff;
	}
	else
	{
		draw_color = 0xaaaaaa;
		//撃てない旨を伝える
		if (frame % 30 >= 15)
		{
			DrawString(100 - GetDrawFormatStringWidth("コイン不足！") / 2, 120, "コイン不足！", 0xff0000);
		}
	}

	//弾変更が有れば変更前と変更後の箱を描画
	if (old_bullet_type != UserData::bullet_type)
	{
		//変更前と変更後を比べて、右にアニメーションするか左にアニメーションするか判断
		if (CheckMoveDirection(UserData::bullet_type, old_bullet_type))
		{
			//右移動
			DrawBullet({ change_anim_move * bullet_change_timer - 200, 0 }, UserData::bullet_type);
			DrawBullet({ change_anim_move * bullet_change_timer, 0 }, old_bullet_type);
		}
		else
		{
			//左移動
			DrawBullet({ change_anim_move * -bullet_change_timer, 0 }, old_bullet_type);
			DrawBullet({ (change_anim_move * -bullet_change_timer) + 200, 0 }, UserData::bullet_type);
		} 
	}
	//変更が無いなら通常の描画
	else
	{
		DrawBullet({ 0, 0 }, UserData::bullet_type);
	}

	//文字大きさ設定
	SetFontSize(24);

	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::CreateLockImage()const
{
	SetDrawScreen(lock_image);
	ClearDrawScreen();
	
	DrawBox(0, 25, 40, 50, 0xff0000, TRUE);
	DrawCircle(20, 25, 17, 0xff0000, FALSE);
	DrawCircle(20, 25, 16, 0xff0000, FALSE);
	DrawCircle(20, 25, 15, 0xff0000, FALSE);

	DrawCircle(20, 35, 5, 0x000000, TRUE);
	DrawBox(17, 35, 23, 45, 0x000000, TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::CreateConfettiImage()
{
	//色格納
	int color[8] = { 0xff0000,0xffaa00,0xffff00,0xaaff00,0x00ff00,0x00ffff,0x0000ff,0xff00ff };
	for (int i = 0; i < 8; i++)
	{
		confetti_image[i] = MakeScreen(16, 16, FALSE);
		SetDrawScreen(confetti_image[i]);
		ClearDrawScreen();

		DrawBox(0, 0, 16, 16, color[i], TRUE);
	}
	SetDrawScreen(DX_SCREEN_BACK);
}

void GameSceneUI::DrawBullet(Vector2D _loc, int _type)const
{
	//撃てない弾のUIは薄暗くする
	int draw_color = pBullet[_type].cost <= UserData::coin ? GetColor(pBullet[_type].color[0], pBullet[_type].color[1], pBullet[_type].color[2]) : 0xaaaa55;

	//弾の種類描画
	DrawBox(_loc.x, _loc.y, _loc.x + 200, _loc.y + 100, 0x777722, true);
	DrawBox(_loc.x, _loc.y, _loc.x + 200, _loc.y + 100, draw_color, false);

	SetFontSize(60);
	DrawFormatString(_loc.x + 100 - GetDrawFormatStringWidth(pBullet[_type].name) / 2, _loc.y, draw_color, "%s", pBullet[_type].name);

	SetFontSize(20);
	UserData::DrawCoin({ _loc.x + 20, _loc.y + 80 }, 15);
	DrawFormatString(_loc.x + 35, _loc.y + 70, draw_color, " - %d", pBullet[_type].cost);
	DrawFormatString(_loc.x + 90, _loc.y + 70, draw_color, "power:%d", (int)pBullet[_type].damage);
}

void GameSceneUI::DrawPlayerUI()const
{
	int old = GetFontSize();

	SetFontSize(24);
	int width = GetDrawFormatStringWidth("HP:%d %d %d", (int)(UserData::player_hp)+1, (int)(UserData::timer / 60), UserData::coin);
	DrawQuadrangle(player_ui_loc.x - width+170, player_ui_loc.y,
		player_ui_loc.x + 420, player_ui_loc.y,
		player_ui_loc.x + 370, player_ui_loc.y + 65,
		player_ui_loc.x - width +190, player_ui_loc.y + 65,
		0x333300, TRUE);
	DrawQuadrangle(player_ui_loc.x - width +180, player_ui_loc.y, 
		player_ui_loc.x + 400, player_ui_loc.y,
		player_ui_loc.x + 350, player_ui_loc.y + 70,
		player_ui_loc.x - width +200, player_ui_loc.y + 70,
		0x666600, TRUE);
	DrawFormatString(player_ui_loc.x - width +200, player_ui_loc.y+15, 0xffffff, "HP:%d", (int)(UserData::player_hp));
	DrawFormatString(player_ui_loc.x - GetDrawFormatStringWidth("TIME:%d %d", (int)(UserData::timer/60), UserData::coin)+280, player_ui_loc.y+15, 0xffffff, "TIME:%d", (int)(UserData::timer/60));
	
	int coin_text_color = 0xffffff;
	//コイン加算時のアニメーション
	if (now_coin_num != old_coin_num)
	{
		//コインが減ったか増えたかでフォントの大きさと色を変える
		SetFontSize(GetFontSize() + (now_coin_num > old_coin_num ? 2 : -2));
		coin_text_color = now_coin_num > old_coin_num ? 0xffff00 : 0xff0000;
	}
	else
	{
		if (GetFontSize() != 24)
		{
			SetFontSize(24);
		}
	}
	//新記録なら
	if (UserData::ranking_data[9].coin < UserData::coin)
	{
		DrawString(SCREEN_WIDTH - GetDrawStringWidth("新記録！", strlen("新記録！")),
			player_ui_loc.y + 40, 
			"新記録！", 
			frame % 30 > 15 ?0xff0000: 0xffffff);
	}
	UserData::DrawCoin({ player_ui_loc.x - GetDrawFormatStringWidth("×%d", UserData::coin) + 320, 
		player_ui_loc.y + 30 }, 
		20);
	DrawFormatString(player_ui_loc.x - GetDrawFormatStringWidth("×%d", UserData::coin) + 340, 
		player_ui_loc.y + 15, 
		coin_text_color,
		"×%d", 
		UserData::coin);
	SetFontSize(old);

	//残り時間１０秒以下なら、カウントダウンを描画する
	if (UserData::timer < 600 && UserData::timer > 0)
	{
		SetFontSize(90 - (UserData::timer % 60 * 1.5f));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - ((UserData::timer % 60) * 6));
		DrawFormatString(SCREEN_WIDTH / 2 - GetDrawFormatStringWidth("%d", UserData::timer / 60) / 2, SCREEN_HEIGHT / 2 - GetFontSize() / 2 - 100, GetColor(255,(UserData::timer/3),0), "%d", (UserData::timer / 60));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		//カウントダウンSE
		if (UserData::timer % 60 == 0)
		{
			PlaySoundMem(count_se, DX_PLAYTYPE_BACK);
		}
	}
}

bool GameSceneUI::CheckMoveDirection(int _now, int _old)const
{
	//右端から左端へ移動する時の判断
	if (_now == 0 && _old == BULLET_NUM - 1)return false;
	//左端から右端へ移動する時の判断
	if (_now == BULLET_NUM - 1 && _old == 0)return true;
	//現在の項目が前の項目より小さいなら右
	if (_now < _old)return true;
	//現在の項目が前の項目より大きいなら左
	if (_now > _old) return false;
	//どれでもないなら
	return -1;
}