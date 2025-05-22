#include "Slot.h"
#include "../Utility/InputPad.h"
#include "ObjectManager.h"

Slot::Slot()
{
	spin_flg = false;
	peka_flg = false;
	can_stop = false;
	timer = 0;
	now_reel = 0;
	reel_wait = 0;
	stop_reel_num = 0;
	drop_coin = BIG_BONUS;
	drop_coin_count = 0;
	for (int i = 0; i < 3; i++)
	{
		reel[i] = -1;
	}
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
	//プレイヤーが触れている間だけリールが回る
	if (can_stop)
	{
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
	if (CheckBigBonus())
	{
		//メダルと同時に攻撃も出して周囲の敵を一掃
		if (frame % 5 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(40) - 20),(float)(GetRand(40) - 20) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
			//煙エフェクト生成
			manager->CreateEffect(elSmoke, this->location + rand, true, 20);

		}
	}

	//プレイヤーが触れているか判断するフラグをリセット
	can_stop = false;
}

void Slot::Draw()const
{
	__super::Draw();

	DrawBoxAA(local_location.x - (box_size.x / 2),
		local_location.y - (box_size.y / 2),
		local_location.x + (box_size.x / 2),
		local_location.y + (box_size.y / 2),
		0x000000, true);
	DrawBoxAA(local_location.x - (box_size.x / 2),
		local_location.y - (box_size.y / 2),
		local_location.x + (box_size.x / 2),
		local_location.y + (box_size.y / 2),
		0xffffff, false);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int r = 0;
			//リールが止まっていなければ、回っているリールで計算
			if (reel[i] == -1)
			{
				r = now_reel + j - 1;
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
				DrawFormatStringF(local_location.x + (i * 20)-20, local_location.y + (j * 15)-20, 0xff0000, "%d", ReelArray[i][r]);
			}
			else
			{
				DrawFormatStringF(local_location.x + (i * 20)-20, local_location.y + (j * 15)-20, 0x00ff00, "%d", ReelArray[i][r]);
			}
		}
	}
	//ペカっていたら光らす
	if (peka_flg)
	{
		if (frame % 10 >= 5)
		{
			DrawCircleAA(local_location.x + (box_size.x / 2)-20, local_location.y - (box_size.y / 2)+20, 10, 10, 0xff0000, true);
		}
		else
		{
			DrawCircleAA(local_location.x - (box_size.x / 2)+20, local_location.y - (box_size.y / 2)+20, 10, 10, 0xff0000, true);

		}
	}
}

void Slot::Hit(ObjectBase* hit_object)
{
	//プレイヤーが触れたらリール回転スタート
	if (hit_object->GetObjectType() == ePLAYER)
	{
		spin_flg = true;
		can_stop = true;
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
	//リール回転中は時間測定
	if (spin_flg)
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
				if (--now_reel < 0)
				{
					now_reel = REEL_NUM - 1;
				}
			}
			if (reel_wait >= (REEL_WAIT / 4) && reel[0] == -1)
			{
				//ペカっていなくて７が揃ったらずらす
				if (CheckBigBonus())
				{
					reel[0] = ReelArray[0][now_reel - 1];
				}
				else
				{
					reel[0] = now_reel;
				}
			}
			if (reel_wait >= (REEL_WAIT / 4) * 2 && reel[1] == -1)
			{
				//ペカっていなくて７が揃ったらずらす
				if (CheckBigBonus())
				{
					reel[1] = ReelArray[1][now_reel - 1];
				}
				else
				{
					reel[1] = now_reel;
				}
			}
			if (reel_wait >= (REEL_WAIT / 4) * 3 && reel[2] == -1)
			{
				//ペカっていなくて７が揃ったらずらす
				if (CheckBigBonus())
				{
					reel[2] = ReelArray[2][now_reel - 1];
				}
				else
				{
					reel[2] = now_reel;
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
	}
	//全リール止まっているなら一定時間待ってリセット
	if (stop_reel_num == 3 && reel_wait > REEL_WAIT)
	{
		reel_wait = 0;
		for (int i = 0; i < 3; i++)
		{
			reel[i] = -1;
		}
		//指定の確率でペカる
		if (GetRand(PEKA) == 0)
		{
			peka_flg = true;
		}
	}
}

void Slot::BonusStop()
{
	//リール回転中は時間測定
	if (spin_flg)
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
				if (--now_reel < 0)
				{
					now_reel = REEL_NUM - 1;
				}
			}
			if (CheckButton(XINPUT_BUTTON_X) && InputPad::OnButton(XINPUT_BUTTON_X) && reel[0] == -1)
			{
				//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
				reel[0] = now_reel;
				if (!CheckBigBonus())
				{
					reel[0] += 1;
					if (!CheckBigBonus())
					{
						reel[0] += 1;
						if (!CheckBigBonus())
						{
							reel[0] -= 2;
						}
					}
				}
			}
			if (CheckButton(XINPUT_BUTTON_Y) && InputPad::OnButton(XINPUT_BUTTON_Y) && reel[1] == -1)
			{
				//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
				reel[1] = now_reel;
				if (!CheckBigBonus())
				{
					reel[1] += 1;
					if (!CheckBigBonus())
					{
						reel[1] += 1;
						if (!CheckBigBonus())
						{
							reel[1] -= 2;
						}
					}
				}
			}
			if (CheckButton(XINPUT_BUTTON_B) && InputPad::OnButton(XINPUT_BUTTON_B) && reel[2] == -1)
			{
				//あと一つ下にずれたらBIGが揃うという状態なら、下にずらす
				reel[2] = now_reel;
				if (!CheckBigBonus())
				{
					reel[2] += 1;
					if (!CheckBigBonus())
					{
						reel[2] += 1;
						if (!CheckBigBonus())
						{
							reel[2] -= 2;
						}
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
	}
	//BIGの払い出し中ならリールは回せない
	if (!CheckBigBonus() || drop_coin_count >= drop_coin)
	{
		//全リール止まっているなら一定時間経過後に右スティックでリール開始
		if (stop_reel_num == 3 &&
			reel_wait > REEL_WAIT &&
			(InputPad::OnButton(R_STICK_UP) || InputPad::OnButton(R_STICK_DOWN)))
		{
			//BIG当選していたらペカを消す
			if (CheckBigBonus())
			{
				peka_flg = false;
			}
			reel_wait = 0;
			drop_coin_count = 0;
			for (int i = 0; i < 3; i++)
			{
				reel[i] = -1;
			}
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

bool Slot::CheckBigBonus()const
{
	//横列を調べる
	for (int i = -1; i < 2; i++)
	{
		if (ReelArray[0][reel[0] + i] == 7 &&
			ReelArray[1][reel[1] + i] == 7 &&
			ReelArray[2][reel[2] + i] == 7)
		{
			return true;
		}
	}
	//斜めを調べる
	if (ReelArray[0][reel[0] - 1] == 7 &&
		ReelArray[1][reel[1]] == 7 &&
		ReelArray[2][reel[2] + 1] == 7)
	{
		return true;
	}
	if (ReelArray[0][reel[0] + 1] == 7 &&
		ReelArray[1][reel[1]] == 7 &&
		ReelArray[2][reel[2] - 1] == 7)
	{
		return true;
	}
	return false;
}