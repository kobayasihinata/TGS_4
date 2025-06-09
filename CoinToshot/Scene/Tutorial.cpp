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
	generate_text_box = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, TRUE);
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

	GenerateTextBox({ 200,200 }, { (float)UserData::variable, (float)UserData::variable });
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
		InitTuto(_type);
	}
}

void Tutorial::InitTuto(TutoType _type)
{
	switch (now_tuto)
	{
	case TutoType::tRule:
		//テキスト量に合った大きさの箱画像を生成
		GenerateTextBox({ 200,200 }, { 200, 100 });
		break;
	case TutoType::tMove:
		//テキスト量に合った大きさの箱画像を生成
		GenerateTextBox({ 200,200 }, { 300, 250 });
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
}

void Tutorial::DrawRule()const
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)text_alpha);
	DrawGraph(200, 100, generate_text_box, TRUE);
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

void Tutorial::GenerateTextBox(Vector2D _loc, Vector2D _size)const
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
		DrawRotaGraphF((TEXT_BOX * 1.5) + i,  (TEXT_BOX / 2), 1.0f, 0.f * 3.14f, text_box[1], TRUE);
		DrawRotaGraphF((TEXT_BOX * 1.5) + i,  size.y - (TEXT_BOX / 2), 1.0f, 1.f * 3.14f, text_box[1], TRUE);
	}
	//y軸
	for (int i = 0; i < size.y - (TEXT_BOX * 2); i += TEXT_BOX)
	{
		DrawRotaGraphF(+(TEXT_BOX / 2), (TEXT_BOX * 1.5) + i, 1.0f, 1.5f * 3.14f, text_box[1], TRUE);
		DrawRotaGraphF(+size.x - (TEXT_BOX / 2), (TEXT_BOX * 1.5) + i, 1.0f, 0.5f * 3.14f, text_box[1], TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);
}