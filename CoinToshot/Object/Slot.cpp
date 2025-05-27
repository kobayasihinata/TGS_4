#include "Slot.h"
#include "../Utility/InputPad.h"
#include "ObjectManager.h"
#include "../Scene/InGameScene.h"

Slot::Slot(InGameScene* _ingame)
{
	ingame = _ingame;

	peka_flg = false;
	can_stop = false;
	bet_once = false;
	timer = 0;
	reel_wait = 0;
	stop_reel_num = 0;
	drop_coin = BIG_BONUS;
	drop_coin_count = 0;
	for (int i = 0; i < 3; i++)
	{
		reel[i] = -1;
		now_reel[i] = 0;
	}
	real_location = 0;
	real_size = { 120.f,100.f };
	hana_location = 0;
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
	//�`��ʒu�X�V
	real_location = { local_location.x + (box_size.x / 2) - 110.f,local_location.y - 150.f };
	hana_location = real_location;
	//�v���C���[���G��Ă���Ԃ������[�������
	if (can_stop)
	{
		//�R�C������
		if (!bet_once)
		{
			//�y�J���Ă�����ꖇ�|��
			if (peka_flg)
			{
				UserData::coin -= 1;
				//�R�C������|�b�v�A�b�v
				ingame->CreatePopUp(this->location, "-1", GetRand(100), 0xff0000, -1, 60);
			}
			else
			{
				UserData::coin -= 3;
				//�R�C������|�b�v�A�b�v
				ingame->CreatePopUp(this->location, "-3", GetRand(100), 0xff0000, -1, 60);
			}

			bet_once = true;
			
		}
		//�y�J���Ă�����ډ�������
		if (peka_flg)
		{
			BonusStop();
		}
		//�y�J���Ă��Ȃ���Ύ�������
		else
		{
			AutoPlay();
		}
	}

	//�V���������烁�_�����o��
	if (CheckBigBonus())
	{
		//���_���Ɠ����ɍU�����o���Ď��͂̓G����|������
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
		}
	}
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
	//���[���͉񂵂Ă鎞�����\��
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

		int old = GetFontSize();
		SetFontSize(32);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int r = 0;
				//���[�����~�܂��Ă��Ȃ���΁A����Ă��郊�[���Ōv�Z
				if (reel[i] == -1)
				{
					r = now_reel[i] + j - 1;
				}
				//���[�����~�܂��Ă�����A�~�܂��Ă��郊�[���Ōv�Z
				else
				{
					r = reel[i] + j - 1;
				}
				//���l�����[���̊O���w���Ă�����i-1��9�j�A��������ʒu�ɂ��ǂ�
				r = (r + REEL_NUM) % REEL_NUM;
				//����̐��������`��F��ς���
				if (ReelArray[i][r] == 7)
				{
					DrawFormatStringF(real_location.x + (i * 40)+10.f, real_location.y + (j * 32), 0xff0000, "%d", ReelArray[i][r]);
				}
				else
				{
					DrawFormatStringF(real_location.x + (i * 40)+10.f, real_location.y + (j * 32), 0x00ff00, "%d", ReelArray[i][r]);
				}
			}
		}
		//�y�J���Ă�������炷
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
			//�{�^���K�C�h�\��
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
		SetFontSize(old);
	}
}

void Slot::Hit(ObjectBase* hit_object)
{
	//�v���C���[���G�ꂽ�烊�[����]�X�^�[�g
	if (hit_object->GetObjectType() == ePLAYER &&
		!can_stop &&
		(UserData::coin >= 3 || (peka_flg && UserData::coin >= 1))
		)
	{
		can_stop = true;
		bet_once = false;
	}

	//�G���G�ꂽ��e�����
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

	//��~���[�����R��菬�����Ȃ�
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
				//�Ή����郊�[�����~�܂��Ă��Ȃ���΃��[����i�߂�
				if (reel[i] == -1 && --now_reel[i] < 0)
				{
					now_reel[i] = REEL_NUM - 1;
				}
			}
		}
		if (reel_wait >= (REEL_WAIT / 4) && reel[0] == -1)
			{
				//�y�J���Ă��Ȃ��ĂV���������炸�炷
				if (CheckBigBonus())
				{
					reel[0] = ReelArray[0][now_reel[0] - 1];
				}
				else
				{
					reel[0] = now_reel[0];
				}
			}
		if (reel_wait >= (REEL_WAIT / 4) * 2 && reel[1] == -1)
			{
				//�y�J���Ă��Ȃ��ĂV���������炸�炷
				if (CheckBigBonus())
				{
					reel[1] = ReelArray[1][now_reel[1] - 1];
				}
				else
				{
					reel[1] = now_reel[1];
				}
			}
		if (reel_wait >= (REEL_WAIT / 4) * 3 && reel[2] == -1)
			{
				//�y�J���Ă��Ȃ��ĂV���������炸�炷
				if (CheckBigBonus())
				{
					reel[2] = ReelArray[2][now_reel[2] - 1];
				}
				else
				{
					reel[2] = now_reel[2];
				}
			}
	}

	//���Z�b�g
	stop_reel_num = 0;
	//��~���Ă��郊�[���̐��𐔂���
	for (int i = 0; i < 3; i++)
		{
			if (reel[i] != -1)
			{
				stop_reel_num++;
			}
		}
	//�S���[���~�܂��Ă���Ȃ��莞�ԑ҂��ă��Z�b�g
	if (stop_reel_num == 3 && reel_wait > REEL_WAIT)
	{
		reel_wait = 0;
		for (int i = 0; i < 3; i++)
		{
			reel[i] = -1;
		}
		//�w��̊m���Ńy�J��
		if (GetRand(PEKA) == 0)
		{
			peka_flg = true;
		}
		//�v���C���[���G��Ă��邩���f����t���O�����Z�b�g
		can_stop = false;
		//�R�C������t���O�����Z�b�g
		bet_once = false;
	}
}

void Slot::BonusStop()
{
	reel_wait++;

	//��~���[�����R��菬�����Ȃ�
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
		if (CheckButton(XINPUT_BUTTON_X) && InputPad::OnButton(XINPUT_BUTTON_X) && reel[0] == -1)
			{
				//���ƈ���ɂ��ꂽ��BIG�������Ƃ�����ԂȂ�A���ɂ��炷
				reel[0] = now_reel[0];
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
				//���ƈ���ɂ��ꂽ��BIG�������Ƃ�����ԂȂ�A���ɂ��炷
				reel[1] = now_reel[1];
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
				//���ƈ���ɂ��ꂽ��BIG�������Ƃ�����ԂȂ�A���ɂ��炷
				reel[2] = now_reel[2];
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

	//���Z�b�g
	stop_reel_num = 0;
	//��~���Ă��郊�[���̐��𐔂���
	for (int i = 0; i < 3; i++)
	{
		if (reel[i] != -1)
		{
			stop_reel_num++;
		}
	}
	//BIG�̕����o�����Ȃ烊�[���͉񂹂Ȃ�
	if (!CheckBigBonus() || drop_coin_count >= drop_coin)
	{
		//�S���[���~�܂��Ă���Ȃ��莞�Ԍo�ߌ�ɉE�X�e�B�b�N�Ń��[���J�n
		if (stop_reel_num == 3 &&
			reel_wait > REEL_WAIT &&
			(InputPad::OnButton(R_STICK_UP) || InputPad::OnButton(R_STICK_DOWN)))
		{
			//BIG���I���Ă�����y�J������
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
			//�v���C���[���G��Ă��邩���f����t���O�����Z�b�g
			can_stop = false;
			//�R�C������t���O�����Z�b�g
			bet_once = false;
		}
	}
}

bool Slot::CheckButton(int _button)
{
	//���[���̃{�^��3��������Ŏ󂯎�����{�^�������O����2���������������Ă��Ȃ������ׂ�
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

	//�z��O�̈������Ă΂ꂽ�瑼�̃{�^����������Ă���ƕԂ�
	return false;
}

bool Slot::CheckBigBonus()const
{
	//����𒲂ׂ�
	for (int i = -1; i < 2; i++)
	{
		if (ReelArray[0][reel[0] + i] == 7 &&
			ReelArray[1][reel[1] + i] == 7 &&
			ReelArray[2][reel[2] + i] == 7)
		{
			return true;
		}
	}
	//�΂߂𒲂ׂ�
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