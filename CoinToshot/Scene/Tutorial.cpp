#include "Tutorial.h"
#include "../Utility/UserData.h"
#include "../Utility/DebugInformation.h"
#include "InGameScene.h"

Tutorial* Tutorial::Get()
{
	static Tutorial instance;
	//自分自身のポインタを返却する
	return &instance;
}


void Tutorial::Initialize()
{
	camera = Camera::Get();
	tutorial_flg = false;		
	tuto_stop_flg = false;		
	now_tuto = TutoType::tNone;	
	timer = 0;					
	text_alpha = 0;		
	stick_anim = 0;
	button_anim = 0;
	aim_timer = 0;
	aim_success_flg = false;
	aim_success_timer = 0;	
	attack_sequence = 0;
	attack_success_timer = 0;
	tuto_end_timer = 0;

	text_box_loc = 0;
	text_box_size = 0;

	for (int i = 0; i < 3; i++)
	{
		text_box[i] = MakeScreen(32, 32, TRUE);
	}
	for (int i = 0; i < TUTO_NUM; i++)
	{
		tuto_executed_flg[i] = DEBUG_SKIP;
	}

	CreateTextBox();
	generate_text_box = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, TRUE);

	ResourceManager* rm = ResourceManager::GetInstance();
	charge_se = rm->GetSounds("Resource/Sounds/charge.mp3");
	nice_se = rm->GetSounds("Resource/Sounds/Direction/成功音.mp3");
}

void Tutorial::Update()
{
	//何かしらのチュートリアルが実行中なら、フラグを立てる
	tutorial_flg = now_tuto != TutoType::tNone;

	//実行中のチュートリアルに応じて、処理をする
	switch (now_tuto)
	{
	case TutoType::tRule:

		//一定時間で終わるタイプのチュートリアルはこの更新//
		UpdateTimeTuto();

		break;
	case TutoType::tMove:

		UpdateMove();
		UpdateTimeTuto();
		break;
	case TutoType::tAim:
		//プレイヤーの行動など、時間以外で終わるチュートリアルなら別途処理
		UpdateAim();
		break;
	case TutoType::tAttack:
		UpdateAttack();
		break;
	case TutoType::tBulletChange:
		UpdateBulletChange();
		UpdateTimeTuto();
		break;
	case TutoType::tSteal:
		UpdateSteal();
		UpdateTimeTuto();
		break;
	case TutoType::tSlot:
		UpdateSlot();
		UpdateTimeTuto();
		break;
	default:
		break;
	}
}

void Tutorial::Draw()const
{

	//実行中のチュートリアルに応じて、処理をする
	switch (now_tuto)
	{
	case TutoType::tRule:
		DrawRule();
		break;
	case TutoType::tMove:
		DrawMove();
		break;
	case TutoType::tAim:
		DrawAim();
		break;
	case TutoType::tAttack:
		DrawAttack();
		break;
	case TutoType::tBulletChange:
		DrawBulletChange();
		break;
	case TutoType::tSteal:
		DrawSteal();
		break;
	case TutoType::tSlot:
		DrawSlot();
		break;
	default:
		break;
	}
}

void Tutorial::GetInGame(InGameScene* _ingame)
{
	ingame = _ingame;
}

bool Tutorial::StartTutoRequest(TutoType _type, ObjectBase* _obj)
{
	//他のチュートリアルが実行中　もしくは既に一回行ったチュートリアルならリクエストは却下
	if (tutorial_flg || tuto_executed_flg[_type])
	{
		return false;
	}
	//何もしていないならチュートリアル開始
	else
	{
		now_tuto = _type;
		tuto_object = _obj;
		InitTuto(_type);
		return true;
	}
}

void Tutorial::InitTuto(TutoType _type)
{
	switch (now_tuto)
	{
	case TutoType::tRule:
		//テキスト量に合った大きさの箱画像を生成
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150 };
		text_box_size = { 600, 200 };
		GenerateTextBox(text_box_size);
		//表示中にゲームを停止状態に
		tuto_stop_flg = true;
		timer = 240;
		break;
	case TutoType::tMove:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150 };
		text_box_size = { 600, 200 };
		timer = 240;
		break;
	case TutoType::tAim:
		//テキスト量に合った大きさの箱画像を生成
		text_box_loc = { SCREEN_WIDTH / 2, 100 };
		text_box_size = { 700, 250 };
		GenerateTextBox(text_box_size);
		//表示中にゲームを停止状態に
		tuto_stop_flg = true;
		ingame->SetZoom(tuto_object->GetLocalLocation(), 5, 180, 15);
		timer = 180;
		break;
	case TutoType::tAttack:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150 };
		text_box_size = { 575, 150 };
		GenerateTextBox(text_box_size);
		tuto_stop_flg = true;
		timer = 180;
		break;
	case TutoType::tBulletChange:
		text_box_loc = { SCREEN_WIDTH / 2, 225 };
		text_box_size = { 375, 150 };
		GenerateTextBox(text_box_size);
		tuto_stop_flg = true;
		timer = 240;
		break;
	case TutoType::tSteal:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150 };
		text_box_size = { 375, 150 };
		GenerateTextBox(text_box_size);
		tuto_stop_flg = true;
		timer = 240;
		break;
	case TutoType::tSlot:
		text_box_loc = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150 };
		text_box_size = { 375, 150 };
		GenerateTextBox(text_box_size);
		tuto_stop_flg = true;
		timer = 300;
		break;
	default:
		break;
	}
}

void Tutorial::TutoEnd()
{
	tutorial_flg = false;
	tuto_stop_flg = false;
	tuto_executed_flg[(int)now_tuto > TUTO_NUM-1 ? TUTO_NUM-1 : (int)now_tuto] = true;
	now_tuto = TutoType::tNone;
	timer = 0;
	text_alpha = 0;
}

bool Tutorial::CheckAim()
{
	//角度が一回も変更されていないなら、反応しない
	if (player_aim_rad > 100)return false;
	//攻撃
	Vector2D attack_loc = camera->player_location;
	float attack_rad = 20;

	for (int i = 0; i < 400; i++)
	{
		attack_loc.x += 2 * cosf(player_aim_rad);
		attack_loc.y += 2 * sinf(player_aim_rad);
		if (tuto_object->CollisionCheckBtoC(tuto_object->GetLocation(), tuto_object->GetSize(), attack_loc, attack_rad))
		{
			return true;
		}
	}	
	return false;
}

void Tutorial::CreateTextBox()const
{
	//角の描画
	SetDrawScreen(text_box[0]);
	ClearDrawScreen();

	DrawBox(2, 2, 32, 32, 0x000000, TRUE);
	DrawLine(2, 2, 32, 2, 0xffffff);
	DrawLine(2, 2, 2, 32, 0xffffff);
	DrawCircle(2, 2, 3, 0x000000, TRUE);
	DrawCircle(2, 2, 3, 0xffffff, FALSE);


	//辺の描画
	SetDrawScreen(text_box[1]);
	ClearDrawScreen();

	DrawBox(0, 2, 32, 32, 0x000000, TRUE);
	DrawLine(0, 2, 32, 2, 0xffffff);


	//内側の描画
	SetDrawScreen(text_box[2]);
	ClearDrawScreen();

	DrawBox(0, 0, 32, 32, 0x000000, TRUE);
	//for (int x = 0; x < 8; x++)
	//{
	//	for (int y = 0; y < 8; y++)
	//	{
	//		//チェック
	//		DrawBox((x * 4), (y * 4), (x * 4) + 4, (y * 4) + 4, (x + y) % 2 == 0 ? 0xffffff : 0x000000, TRUE);
	//	}
	//}
	SetDrawScreen(DX_SCREEN_BACK);
}

void Tutorial::GenerateTextBox(Vector2D _size)const
{
	Vector2D size = { _size.x > 37 ? _size.x : 37,_size.y > 37 ? _size.y : 37 };

	SetDrawScreen(generate_text_box);
	ClearDrawScreen();

	//四隅から描画
	DrawRotaGraphF(		  (TEXT_BOX / 2),		     (TEXT_BOX / 2), 1.0f, 0.f *3.14f,  text_box[0],TRUE);
	DrawRotaGraphF(size.x - (TEXT_BOX / 2),		     (TEXT_BOX / 2), 1.0f, 0.5f*3.14f,  text_box[0],TRUE);
	DrawRotaGraphF(size.x - (TEXT_BOX / 2),  size.y - (TEXT_BOX / 2), 1.0f, 1.f *3.14f,  text_box[0],TRUE);
	DrawRotaGraphF(          (TEXT_BOX / 2),  size.y - (TEXT_BOX / 2), 1.0f, 1.5f*3.14f,  text_box[0],TRUE);

	//内側
	if (size.x > 64 && size.y > 64)
	{
		for (int i = 0; i < size.x - (TEXT_BOX * 2); i += TEXT_BOX)
		{
			//y軸
			for (int j = 0; j < size.y - (TEXT_BOX * 2); j += TEXT_BOX)
			{
				DrawRotaGraphF((TEXT_BOX * 1.5f) + i, (TEXT_BOX * 1.5f) + j, 1.0f, 0.f, text_box[2], TRUE);
			}
		}

	}
	//隙間を辺で埋める

	//X軸
	for (int i = 0; i < size.x - (TEXT_BOX*2); i += TEXT_BOX)
	{
		DrawRotaGraphF((TEXT_BOX * 1.5) + i -1,  (TEXT_BOX / 2), 1.0f, 0.f * 3.14f, text_box[1], TRUE);
		DrawRotaGraphF((TEXT_BOX * 1.5) + i -1,  size.y - (TEXT_BOX / 2), 1.0f, 1.f * 3.14f, text_box[1], TRUE);
	}
	//y軸
	for (int i = 0; i < size.y - (TEXT_BOX * 2); i += TEXT_BOX)
	{
		DrawRotaGraphF((TEXT_BOX / 2), (TEXT_BOX * 1.5) + i, 1.0f, 1.5f * 3.14f, text_box[1], TRUE);
		DrawRotaGraphF(size.x - (TEXT_BOX / 2), (TEXT_BOX * 1.5) + i, 1.0f, 0.5f * 3.14f, text_box[1], TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void Tutorial::UpdateTimeTuto()
{
	//チュートリアルが終わったらフラグを下げる
	if (--timer <= 0)
	{
		TutoEnd();
	}

	if (tutorial_flg)
	{
		//フェード演出
		if (timer > FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (timer <= FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}
	}
	else
	{
		text_alpha = 0;
	}
}

void Tutorial::DrawRule()const
{
	SetFontSize(48);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 70 }, "ルール説明", 0xffffff);
	SetFontSize(64);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 30 }, "ひたすらコインを稼げ！", 0xeeeeee);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::UpdateMove()
{
	//スティックを回すアニメーション
	if (timer % 10 == 0)
	{
		if (++stick_anim > 3)
		{
			stick_anim = 0;
		}
	}
}
void Tutorial::DrawMove()const
{
	SetFontSize(64);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "左スティック：移動", 0xffffff);
	DrawRotaGraphF(text_box_loc.x, text_box_loc.y + 50, 1.5f, 0, UserData::button_image[1][l_stick[stick_anim]], TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::UpdateAim()
{
	//スティックを回すアニメーション
	if (timer % 10 == 0)
	{
		if (++stick_anim > 3)
		{
			stick_anim = 0;
		}
	}
	if (tutorial_flg)
	{
		//フェード演出
		if (--timer > FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (timer <= FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}
	}
	else
	{
		text_alpha = 0;
	}
	//タイマーがマイナスに達したら実践開始
	if (timer < 0)
	{
		//動けるようにする
		if (tuto_stop_flg)
		{
			tuto_stop_flg = false;
		}
		//照準を敵に合わせたらチュートリアルを終了して攻撃チュートリアル
		if (!aim_success_flg && CheckAim())
		{
			if (++aim_timer > 90)
			{
				aim_success_flg = true;
				aim_timer = 0;
				ResourceManager::rPlaySound(nice_se, DX_PLAYTYPE_BACK);
			}
			if (!CheckSoundMem(charge_se))
			{
				ResourceManager::rPlaySound(charge_se, DX_PLAYTYPE_BACK);
			}
		}
		//照準がずれたらリセット
		else
		{
			aim_timer = 0;
			if (CheckSoundMem(charge_se))
			{
				StopSoundMem(charge_se);
			}
		}
		//照準を成功させたら演出
		if (aim_success_flg)
		{
			//一定時間でチュートリアル終了
			if (++aim_success_timer > 120)
			{
				//一応リセット
				aim_success_flg = false;
				aim_success_timer = 0;
				TutoEnd();
				StartTutoRequest(TutoType::tAttack, tuto_object);
			}
		}
	}
}
void Tutorial::DrawAim()const
{
	Vector2D obj_loc = tuto_object->GetLocalLocation();
	Vector2D obj_size = { 100,20 };
	int obj_shift = 30;

	//説明のテキストボックス
	if (timer > 0)
	{
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
		//DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
		DrawGraph(text_box_loc.x - text_box_size.x/2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
		SetFontSize(64);
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "敵に照準を合わせよう", 0xffffff);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//実践のターゲット
	else
	{
		SetFontSize(64);
		UserData::DrawStringCenter({SCREEN_WIDTH/2,SCREEN_HEIGHT/2 - 200 }, "右スティック：照準", 0xffffff);
		DrawRotaGraphF(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 1.5f, 0, UserData::button_image[1][r_stick[stick_anim]], TRUE);
		//照準
		if (aim_timer > 0)
		{
			DrawBoxAA(obj_loc.x - (obj_size.x / 2),
				obj_loc.y - (obj_size.y / 2) - obj_shift,
				obj_loc.x + (obj_size.x / 2),
				obj_loc.y + (obj_size.y / 2) - obj_shift,
				0xaaaa00, TRUE);
			DrawBoxAA(obj_loc.x - (obj_size.x/2),
				obj_loc.y - (obj_size.y / 2) - obj_shift,
				obj_loc.x - (obj_size.x/2) + (obj_size.x / 30) * (aim_timer % 30),
				obj_loc.y + (obj_size.y / 2) - obj_shift,
				0xffaa00, TRUE);
			DrawBoxAA(obj_loc.x - (obj_size.x / 2),
				obj_loc.y - (obj_size.y / 2) - obj_shift,
				obj_loc.x + (obj_size.x / 2),
				obj_loc.y + (obj_size.y / 2) - obj_shift,
				0xffffff, FALSE);

			SetFontSize(32);
			DrawFormatString(obj_loc.x - (GetDrawFormatStringWidth("%d", 3 - (aim_timer / 30)) / 2) + 2, obj_loc.y - 50 + 2, 0x000000, "%d", 3 - (aim_timer / 30));
			DrawFormatString(obj_loc.x - (GetDrawFormatStringWidth("%d", 3 - (aim_timer / 30)) / 2), obj_loc.y - 50, 0xff0000, "%d", 3 - (aim_timer / 30));
		}
		//照準合わせ成功描画
		if (aim_success_flg)
		{
			DrawBoxAA(obj_loc.x - (obj_size.x / 2),
				obj_loc.y - (obj_size.y / 2) - obj_shift,
				obj_loc.x + (obj_size.x / 2),
				obj_loc.y + (obj_size.y / 2) - obj_shift,
				0xffaa00, TRUE);
			DrawBoxAA(obj_loc.x - (obj_size.x / 2),
				obj_loc.y - (obj_size.y / 2) - obj_shift,
				obj_loc.x + (obj_size.x / 2),
				obj_loc.y + (obj_size.y / 2) - obj_shift,
				0xffffff, FALSE);
			SetFontSize(32);
			DrawString(obj_loc.x - GetDrawStringWidth("nice!",strlen("nice!"))/2, obj_loc.y - 50, "nice!", aim_success_timer % 30 > 15 ? 0xaaaa00 : 0xffff00);
		}
	}
}

void Tutorial::UpdateAttack()
{
	timer--;
	switch (attack_sequence)
	{
	case 0: //説明テキスト表示中

		//フェード演出
		if (timer > FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (timer <= FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}

		//表示終了で実践手順移行
		if (timer < 0)attack_sequence = 1;
		break;

	case 1:	//実践
		//ボタンアニメーション
		if (timer % 10 == 0)
		{
			if (++button_anim > 1)
			{
				button_anim = 0;
			}
		}
		//動けるようにする
		if (tuto_stop_flg)
		{
			tuto_stop_flg = false;
		}
		//敵が存在している間は座標を取得
		if (!tuto_object->GetDeathFlg())
		{
			enemy_loc = tuto_object->GetLocation();
		}
		//敵が死んでいたら成功描画手順
		else {
			attack_sequence = 2;
			ResourceManager::rPlaySound(nice_se, DX_PLAYTYPE_BACK);
		}
		break;

	case 2:	//成功描画手順
		if (++attack_success_timer > 120)
		{
			attack_sequence = 3;
			text_alpha = 0;
		}
		break;
	case 3: //基本操作説明完了
		//動けないようにする
		if (!tuto_stop_flg)
		{
			tuto_stop_flg = true;
		}
		if (++tuto_end_timer > 180)
		{
			tuto_end_timer = 0;
			TutoEnd();
		}
		//フェード演出
		if (tuto_end_timer <= FADE_TIME && text_alpha < 255)
		{
			text_alpha += 255.f / FADE_TIME;
		}
		if (tuto_end_timer > 180 - FADE_TIME)
		{
			text_alpha -= 255.f / FADE_TIME;
		}

		break;
	}
}

void Tutorial::DrawAttack()const
{
	switch (attack_sequence)
	{
	case 0: //説明テキスト表示中
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
		DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
		
		SetFontSize(64);
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 70 }, "コインを飛ばして", 0xffffff);
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 20 }, "敵を攻撃！", timer % 30 > 15 ? 0xffffff : 0xff0000);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case 1:	//実践
		SetFontSize(64);
		if (UserData::control_type == 0) {
			UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "LB、RBボタン：攻撃", 0xffffff);
			DrawRotaGraphF(text_box_loc.x - 40, text_box_loc.y + 50, 1.5f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_LEFT_SHOULDER], TRUE);
			DrawRotaGraphF(text_box_loc.x + 40, text_box_loc.y + 50, 1.5f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_RIGHT_SHOULDER], TRUE);
		}
		else if (UserData::control_type == 1)
		{
			UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 50 }, "Bボタン：攻撃", 0xffffff);
			DrawRotaGraphF(text_box_loc.x - 20, text_box_loc.y + 50, 1.5f, 0, UserData::button_image[button_anim][XINPUT_BUTTON_B], TRUE);
		}

		break;
	case 2:	//成功描画手順
		SetFontSize(48);
		DrawString(enemy_loc.x - camera->GetCameraLocation().x - 25, enemy_loc.y - camera->GetCameraLocation().y - 70, "nice!", attack_success_timer % 30 > 15 ? 0xaaaa00 : 0xff4400);
		break;
	case 3: //基本操作説明完了
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
		DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
		
		SetFontSize(52);
		UserData::DrawStringCenter({ text_box_loc.x+15,text_box_loc.y - 70 }, "撃って倒して稼いで、", 0xffffff);
		UserData::DrawStringCenter({ text_box_loc.x+10,text_box_loc.y - 10 }, "ランキング1位を目指せ！", 0xffffff);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	}
}


void Tutorial::UpdateBulletChange()
{

}
void Tutorial::DrawBulletChange()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
	SetFontSize(36);
	DrawString(text_box_loc.x - (text_box_size.x / 2) + 10, text_box_loc.y - 40, "弾の変更をアンロック！", 0xffffff);
	SetFontSize(30);
	if (UserData::control_type == 0) 
	{
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y + 10 }, "左右トリガーで変更", 0xffffff);
	}
	else if (UserData::control_type == 1)
	{
		UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y + 10 }, "LB、RBで変更", 0xffffff);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::UpdateSteal()
{

}

void Tutorial::DrawSteal()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
	SetFontSize(36);
	DrawString(text_box_loc.x - (text_box_size.x / 2) + 15, text_box_loc.y - 40, "コインを盗まれた！", 0xffffff);
	SetFontSize(30);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y + 10 }, "倒すと取り返せる", 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	tuto_object->Draw();
	DrawCircleAA(tuto_object->GetLocalLocation().x, tuto_object->GetLocalLocation().y, tuto_object->GetSize().x * 1.5f, 100, 0xff0000, false);
}

void Tutorial::UpdateSlot()
{

}

void Tutorial::DrawSlot()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha - 150);
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(text_box_loc.x - text_box_size.x / 2, text_box_loc.y - text_box_size.y / 2, generate_text_box, TRUE);
	SetFontSize(36);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y - 40 }, "スロットを発見！", 0xffffff);
	SetFontSize(30);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y + 10 }, "大当たりまで回し続けろ！", 0xffffff);
	UserData::DrawStringCenter({ text_box_loc.x,text_box_loc.y + 40 }, "＊注意　１プレイ３コイン", 0xff0000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	tuto_object->Draw();
	DrawCircleAA(tuto_object->GetLocalLocation().x, tuto_object->GetLocalLocation().y, tuto_object->GetSize().x, 100, 0xff0000, false);
}