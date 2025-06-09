#include "Tutorial.h"
#include "../Utility/UserData.h"

Tutorial* Tutorial::Get()
{
	static Tutorial instance;
	//自分自身のポインタを返却する
	return &instance;
}


void Tutorial::Initialize()
{
	tutorial_flg = false;		
	tuto_stop_flg = false;		
	now_tuto = TutoType::tNone;	
	timer = 0;					
	text_alpha = 0;		

	for (int i = 0; i < 3; i++)
	{
		text_box[i] = MakeScreen(32, 32, TRUE);
	}
	CreateTextBox();
}

void Tutorial::Update()
{
	//何かしらのチュートリアルが実行中なら、フラグを立てる
	tutorial_flg = now_tuto != TutoType::tNone;

	//実行中のチュートリアルに応じて、処理をする
	switch (now_tuto)
	{
	case TutoType::tRule:
		tuto_stop_flg = true;
		break;
	case TutoType::tMove:
		break;
	case TutoType::tAim:
		break;
	case TutoType::tAttack:
		break;
	case TutoType::tBulletChange:
		break;
	case TutoType::tEnemyBullet:
		break;
	case TutoType::tSlot:
		break;
	default:
		break;
	}

	//チュートリアルが終わったらフラグを下げる
	if (--timer <= 0)
	{
		tutorial_flg = false;
		tuto_stop_flg = false;
		now_tuto = TutoType::tNone;
		timer = 0;
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

void Tutorial::Draw()const
{
	DrawFormatString(100, 80, 0x00ff00, "%d", timer);
	//実行中のチュートリアルに応じて、処理をする
	switch (now_tuto)
	{
	case TutoType::tRule:
		DrawRule();
		break;
	case TutoType::tMove:
		DrawString(100, 100, "操作説明", 0x00ff00);
		break;
	case TutoType::tAim:
		DrawString(100, 100, "エイム説明", 0x00ff00);
		break;
	case TutoType::tAttack:
		DrawString(100, 100, "攻撃説明", 0x00ff00);
		break;
	case TutoType::tBulletChange:
		DrawString(100, 100, "弾変更説明", 0x00ff00);
		break;
	case TutoType::tEnemyBullet:
		DrawString(100, 100, "敵攻撃説明", 0x00ff00);
		break;
	case TutoType::tSlot:
		DrawString(100, 100, "スロット説明", 0x00ff00);
		break;
	default:
		break;
	}
}

bool Tutorial::StartTutoRequest(TutoType _type)
{
	//他のチュートリアルが実行中ならリクエストは却下
	if (tutorial_flg)
	{
		return false;
	}
	//何もしていないならチュートリアル開始
	else
	{
		now_tuto = _type;
		timer = 60;
	}
}


void Tutorial::DrawRule()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawTextBox({ 200,200 }, { 200, 100 });
	DrawString(100, 100, "ルール説明", 0x00ff00);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Tutorial::CreateTextBox()const
{
	//角の描画
	SetDrawScreen(text_box[0]);
	ClearDrawScreen();

	DrawBox(2, 2, 32, 32, 0x000000, TRUE);
	DrawLine(2, 2, 32, 2, 0xffffff);
	DrawLine(2, 2, 2, 32, 0xffffff);
	DrawCircle(2, 2, 4, 0x000000, TRUE);
	DrawCircle(2, 2, 4, 0xffffff, FALSE);


	//辺の描画
	SetDrawScreen(text_box[1]);
	ClearDrawScreen();

	DrawBox(2, 2, 32, 32, 0x000000, TRUE);
	DrawLine(2, 2, 32, 2, 0xffffff);


	//内側の描画
	SetDrawScreen(text_box[2]);
	ClearDrawScreen();

	DrawBox(0, 0, 32, 32, 0x000000, TRUE);

	SetDrawScreen(DX_SCREEN_BACK);
}

void Tutorial::DrawTextBox(Vector2D _loc, Vector2D _size)const
{
	for (int i = 0; i < 3; i++)
	{
		DrawGraph(_loc.x + 33 * i, _loc.y, text_box[i], TRUE);
	}
}