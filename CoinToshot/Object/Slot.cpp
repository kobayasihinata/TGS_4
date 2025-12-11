#include "Slot.h"
#include "../Utility/InputPad.h"
#include "ObjectManager.h"
#include "../Scene/InGameScene.h"

Slot::Slot(InGameScene* _ingame)
{
	ingame = _ingame;
	tutorial = Tutorial::Get();
	camera = Camera::Get();

	peka_flg = false;
	can_stop = false;
	bet_once = false;
	reel_wait = 0;
	stop_reel_num = 0;
	timer = 0;
	bonus_wait_count = 0;
	drop_coin = BIG_BONUS;
	drop_coin_count = 0;
	for (int i = 0; i < 3; i++)
	{
		reel[i] = -1;
		now_reel[i] = 0;
	}
	pay_flg = false;
	pay_num = 0;
	pay_count = 0;
	reel_span = 0;
	real_location = 0;
	real_size = { 120.f,100.f };
	hana_location = 0;

	//読み込み
	ResourceManager* rm = ResourceManager::GetInstance();
	slot_image = rm->GetImages("Resource/Images/Slot.png");
	//SE読み込み
	reel_se = rm->GetSounds("Resource/Sounds/Hanahana/reel.mp3");
	button_se = rm->GetSounds("Resource/Sounds/Hanahana/button.mp3");
	bonus_se = rm->GetSounds("Resource/Sounds/Hanahana/big.mp3");
	pay_se = rm->GetSounds("Resource/Sounds/Hanahana/決定ボタンを押す7.mp3");
	bonus_se_play_once = false;
	//PlaySoundMem(reel_se, DX_PLAYTYPE_BACK);
}

Slot::~Slot()
{

}

void Slot::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Slot::Finalize()
{

}

void Slot::Update()
{
	if (++frame > 6000)
	{
		frame = 0;
	}
	//リールの待ち時間
	if (reel_span > 0)
	{
		reel_span--;
	}
	//描画位置更新
	real_location = { local_location.x + (box_size.x / 2) - 110.f,local_location.y - 150.f };
	hana_location = real_location;
	//プレイヤーと自身の距離が一定範囲で、スロットチュートリアルが終わっていないなら、リクエストする
	if (tutorial->GetBasicTuto() && !tutorial->GetTutoNowEnd(TutoType::tSlot) && sqrtf(powf(location.x - camera->player_location.x, 2) + powf(location.y - camera->player_location.y, 2)) < 300)
	{
		tutorial->StartTutoRequest(TutoType::tSlot, this);
	}
	//プレイヤーが触れている間だけリールが回る
	if (can_stop)
	{
		//コイン消費
		if (!bet_once)
		{
			//ペカっていたら一枚掛け
			if (peka_flg)
			{
				UserData::coin -= 1;
				//コイン消費ポップアップ
				ingame->CreatePopUp(this->location, "-1", 0xff0000, -1);
			}
			else
			{
				UserData::coin -= 3;
				//コイン消費ポップアップ
				ingame->CreatePopUp(this->location, "-3", 0xff0000, -1);
			}

			bet_once = true;
			
		}
		//ペカっていたら目押し処理
		if (peka_flg)
		{
			BonusStop();
		}
		//ペカっていなければ自動消化
		else
		{
			AutoPlay();
		}
	}

	//７が揃ったらメダルを出す
	if (CheckStraightLine(7))
	{
		if (!bonus_se_play_once)
		{
			//SEを再生
			ResourceManager::rPlaySound(bonus_se, DX_PLAYTYPE_BACK);
			bonus_se_play_once = true;
		}
		//メダルと同時に攻撃も出して周囲の敵を一掃したい
		if (++bonus_wait_count > 60 && frame % 5 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(40) - 20),(float)(GetRand(40) - 20) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
		}
	}

	//７以外の数字が揃っていたら対応するコインを出す
	if (pay_flg)
	{
		if (frame % 10 == 0 && pay_count < pay_num)
		{
			Vector2D rand = { (float)(GetRand(40) - 20),(float)(GetRand(40) - 20) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			//SEを再生
			ResourceManager::rPlaySound(pay_se, DX_PLAYTYPE_BACK);
			pay_count++;
		}
		if (pay_count >= pay_num)
		{
			pay_flg = false;
			pay_num = 0;
			pay_count = 0;
		}
	}
}

void Slot::Draw()const
{
	__super::Draw();

	DrawRotaGraphF(local_location.x, local_location.y, 0.1, 0, slot_image[0], true);

	//リールは回してる時だけ表示
	if (can_stop)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
		DrawBoxAA(real_location.x,
			real_location.y,
			real_location.x + real_size.x,
			real_location.y + real_size.y,
			0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawBoxAA(real_location.x,
			real_location.y,
			real_location.x + real_size.x,
			real_location.y + real_size.y,
			0xffffff, false);

		SetFontSize(32);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int r = 0;
				//リールが止まっていなければ、回っているリールで計算
				if (reel[i] == -1)
				{
					r = now_reel[i] + j - 1;
				}
				//リールが止まっていたら、止まっているリールで計算
				else
				{
					r = reel[i] + j - 1;
				}
				//数値がリールの外を指していたら（-1や9）、一周した位置にもどす
				r = (r + REEL_NUM) % REEL_NUM;
				//特定の数字だけ描画色を変える
				if (ReelArray[i][r] == 7)
				{
					DrawFormatStringF(real_location.x + (i * 40)+10.f, real_location.y + (j * 32), 0xff0000, "%d", ReelArray[i][r]);
				}
				else if(pay_flg && ReelArray[i][r] == pay_num)
				{
					DrawFormatStringF(real_location.x + (i * 40)+10.f, real_location.y + (j * 32), UserData::GetColorGradation((frame*30)%1530), "%d", ReelArray[i][r]);
				}
				else
				{
					DrawFormatStringF(real_location.x + (i * 40) + 10.f, real_location.y + (j * 32), 0x00ff00, "%d", ReelArray[i][r]);
				}
			}
		}
		//ペカっていたら光らす
		if (peka_flg)
		{
			if (frame % 10 >= 5)
			{
				DrawCircleAA(hana_location.x , hana_location.y, 20, 10, 0xff0000, true);
			}
			else
			{
				DrawCircleAA(hana_location.x + real_size.x, hana_location.y, 20, 10, 0xff0000, true);

			}
			//ボタンガイド表示
			if (reel[0] == -1)
			{
				manager->DrawButton({ local_location.x- 60,local_location.y-40 }, XINPUT_BUTTON_X);
			}
			if (reel[1] == -1)
			{
				manager->DrawButton({ local_location.x - 20,local_location.y-40 }, XINPUT_BUTTON_Y);
			}
			if (reel[2] == -1)
			{
				manager->DrawButton({ local_location.x+20,local_location.y-40 }, XINPUT_BUTTON_B);
			}
			if (stop_reel_num == 3 &&
				reel_wait > REEL_WAIT)
			{
				manager->DrawButton({ local_location.x - 100,local_location.y - 40 }, R_STICK_UP);
				manager->DrawButton({ local_location.x - 100,local_location.y }, R_STICK_DOWN);
			}
		}
	}
}

void Slot::Hit(ObjectBase* hit_object)
{
	//プレイヤーが触れたらリール回転スタート
	if (hit_object->GetObjectType() == ePLAYER &&
		!can_stop &&
		(UserData::coin >= 3 || (peka_flg && UserData::coin >= 1))
		)
	{
		can_stop = true;
		bet_once = false;
		//SEを再生
		ResourceManager::rPlaySound(reel_se, DX_PLAYTYPE_BACK);
	}

	//敵が触れたら弾かれる
	if (hit_object->IsEnemy())
	{
		hit_object->Damage(0, this->location, 30);
	}
}

void Slot::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{

}

void Slot::AutoPlay()
{
	reel_wait++;

	//停止リールが３つより小さいなら
	if (stop_reel_num < 3)
	{
		if (++timer > 300)
		{
			timer = 0;
		}
		if (timer % 5 == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				//対応するリールが止まっていなければリールを進める
				if (reel[i] == -1 && --now_reel[i] < 0)
				{
					now_reel[i] = REEL_NUM - 1;
				}
			}
		}
		//子役フラグが立っていたら、その数字に合わせる
		if (pay_num > 0 && pay_num < 10)
		{
			for (int i = 0; i < 3; i++)
			{
				if (reel_wait >= (REEL_WAIT / 4) && reel[i] == -1 && ReelArray[i][now_reel[i]] == pay_num && reel_span <= 0)
				{
					reel[i] = now_reel[i];
					//SEを再生
					ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
					reel_span = REEL_SPAN;
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (reel_wait >= (REEL_WAIT / 4) * (i + 1) && reel[i] == -1 && reel_span <= 0)
				{
					//ペカっていなくて７が揃ったらずらす
					if (CheckStraightLine(7))
					{
						reel[i] = now_reel[i] - 1;
					}
					else
					{
						reel[i] = now_reel[i];
					}
					//SEを再生
					ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
					reel_span = REEL_SPAN;
				}
			}
		}

		//数字が揃っていたら払い出し開始
		if (reel[0] != -1 &&
			reel[1] != -1 && 
			reel[2] != -1)
		{
			for (int i = 1; i < 10; i++)
			{
				if (CheckStraightLine(i))
				{
					pay_flg = true;
					pay_num = i;
					break;
				}
			}
		}
	}

	DebugInfomation::Add("slot1", ReelArray[0][reel[0]]);
	DebugInfomation::Add("slot2", ReelArray[1][reel[1]]);
	DebugInfomation::Add("slot3", ReelArray[2][reel[2]]);
	//リセット
	stop_reel_num = 0;
	//停止しているリールの数を数える
	for (int i = 0; i < 3; i++)
	{
		if (reel[i] != -1)
		{
			stop_reel_num++;
		}
	}

	//全リール止まっているなら一定時間待ってリセット
	if (stop_reel_num == 3 && reel_wait > REEL_WAIT && !pay_flg)
	{
		reel_wait = 0;
		for (int i = 0; i < 3; i++)
		{
			reel[i] = -1;
		}
		//指定の確率でペカる
		if (GetRand(PEKA - 1) == 0)
		{
			peka_flg = true;
		}
		//子役の抽選
		if (!peka_flg && GetRand(BELL - 1) == 0 && pay_num==0)
		{
			//７以外になるまで繰り返す
			do {
				pay_num = GetRand(8)+1;
			} while (pay_num == 7);
			pay_count = 0;
		}

		//プレイヤーが触れているか判断するフラグをリセット
		can_stop = false;
		//コイン消費フラグをリセット
		bet_once = false;
	}
}

void Slot::BonusStop()
{
	reel_wait++;

	//停止リールが３つより小さいなら
	if (stop_reel_num < 3)
	{
		if (++timer > 300)
		{
			timer = 0;
		}
		if (timer % 5 == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (--now_reel[i] < 0)
				{
					now_reel[i] = REEL_NUM - 1;
				}
			}
		}
		if (UserData::control_type != 2)
		{
			if (CheckButton(XINPUT_BUTTON_X) && InputPad::OnButton(XINPUT_BUTTON_X) && reel[0] == -1 && reel_span <= 0)
			{
				//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
				reel[0] = now_reel[0];
				if (!CheckStraightLine(7))
				{
					reel[0] -= 1;
					if (!CheckStraightLine(7))
					{
						reel[0] -= 1;
						if (!CheckStraightLine(7))
						{
							reel[0] += 2;
						}
					}
				}
				//SEを再生
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
				reel_span = REEL_SPAN;
			}
			if (CheckButton(XINPUT_BUTTON_Y) && InputPad::OnButton(XINPUT_BUTTON_Y) && reel[1] == -1 && reel_span <= 0)
			{
				//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
				reel[1] = now_reel[1];
				if (!CheckStraightLine(7))
				{
					reel[1] -= 1;
					if (!CheckStraightLine(7))
					{
						reel[1] -= 1;
						if (!CheckStraightLine(7))
						{
							reel[1] += 2;
						}
					}
				}
				//SEを再生
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
				reel_span = REEL_SPAN;
			}
			if (CheckButton(XINPUT_BUTTON_B) && InputPad::OnButton(XINPUT_BUTTON_B) && reel[2] == -1 && reel_span <= 0)
			{
				//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
				reel[2] = now_reel[2];
				if (!CheckStraightLine(7))
				{
					reel[2] -= 1;
					if (!CheckStraightLine(7))
					{
						reel[2] -= 1;
						if (!CheckStraightLine(7))
						{
							reel[2] += 2;
						}
					}
				}
				//SEを再生
				ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
				reel_span = REEL_SPAN;
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (UserData::CheckEnter(eInputState::Pressed) && reel[i] == -1 && reel_span <= 0)
				{
					//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
					reel[i] = now_reel[i];
					if (!CheckStraightLine(7))
					{
						reel[i] -= 1;
						if (!CheckStraightLine(7))
						{
							reel[i] -= 1;
							if (!CheckStraightLine(7))
							{
								reel[i] += 2;
							}
						}
					}
					//SEを再生
					ResourceManager::rPlaySound(button_se, DX_PLAYTYPE_BACK);
					reel_span = REEL_SPAN;
					//他のリールが同時に揃わない用にループ終了
					break;
				}
			}
		}

	}

	//リセット
	stop_reel_num = 0;
	//停止しているリールの数を数える
	for (int i = 0; i < 3; i++)
	{
		if (reel[i] != -1)
		{
			stop_reel_num++;
		}
	}
	//BIGの払い出し中ならリールは回せない
	if (!CheckStraightLine(7) || drop_coin_count >= drop_coin)
	{
		//全リール止まっているなら一定時間経過後に右スティックでリール開始
		if (stop_reel_num == 3 &&
			reel_wait > REEL_WAIT &&
			((UserData::control_type != 2 &&(InputPad::OnButton(R_STICK_UP) || InputPad::OnButton(R_STICK_DOWN)))||
			(UserData::control_type == 2 && UserData::CheckEnter(eInputState::Pressed))))
		{
			//BIG当選していたらペカを消す
			if (CheckStraightLine(7))
			{
				peka_flg = false;
			}
			reel_wait = 0;
			drop_coin_count = 0;
			bonus_se_play_once = 0;
			bonus_wait_count = 0;
			for (int i = 0; i < 3; i++)
			{
				reel[i] = -1;
			}
			//プレイヤーが触れているか判断するフラグをリセット
			can_stop = false;
			//コイン消費フラグをリセット
			bet_once = false;
		}
	}

}

bool Slot::CheckButton(int _button)
{
	//リールのボタン3つから引数で受け取ったボタンを除外した2つがいずれも押されていないか調べる
	if (_button == XINPUT_BUTTON_X)
	{
		if (InputPad::OnPressed(XINPUT_BUTTON_Y) || 
			InputPad::OnPressed(XINPUT_BUTTON_B))
		{
			return false;
		}
		return true;
	}
	if (_button == XINPUT_BUTTON_Y)
	{
		if (InputPad::OnPressed(XINPUT_BUTTON_X) ||
			InputPad::OnPressed(XINPUT_BUTTON_B))
		{
			return false;
		}
		return true;
	}
	if (_button == XINPUT_BUTTON_B)
	{
		if (InputPad::OnPressed(XINPUT_BUTTON_Y) ||
			InputPad::OnPressed(XINPUT_BUTTON_X))
		{
			return false;
		}
		return true;
	}

	//想定外の引数が呼ばれたら他のボタンが押されていると返す
	return false;
}

bool Slot::CheckStraightLine(int _check_num)const
{
	//横列を調べる
	for (int i = -1; i < 2; i++)
	{
		int a = ReelArray[0][(reel[0] + 9 + i) % 9];
		int b = ReelArray[1][(reel[1] + 9 + i) % 9];
		int c = ReelArray[2][(reel[2] + 9 + i) % 9];
		if (ReelArray[0][(reel[0] + 9 + i) % 9] == _check_num &&
			ReelArray[1][(reel[1] + 9 + i) % 9] == _check_num &&
			ReelArray[2][(reel[2] + 9 + i) % 9] == _check_num)
		{
			return true;
		}
	}
	//斜めを調べる
	if (ReelArray[0][(reel[0] + 9 - 1) % 9] == _check_num &&
		ReelArray[1][(reel[1] + 9) % 9] == _check_num &&
		ReelArray[2][(reel[2] + 9 + 1) % 9] == _check_num)
	{
		return true;
	}
	if (ReelArray[0][(reel[0] + 9 + 1) % 9] == _check_num &&
		ReelArray[1][(reel[1] + 9) % 9] == _check_num &&
		ReelArray[2][(reel[2] + 9 - 1) % 9] == _check_num)
	{
		return true;
	}
	return false;
}