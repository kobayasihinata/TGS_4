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
	//�v���C���[���G��Ă���Ԃ������[�������
	if (can_stop)
	{
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
		//���_���Ɠ����ɍU�����o���Ď��͂̓G����|
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
			//���G�t�F�N�g����
			manager->CreateEffect(elSmoke, this->location + rand, true, 20);

		}
	}

	//�v���C���[���G��Ă��邩���f����t���O�����Z�b�g
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
			//���[�����~�܂��Ă��Ȃ���΁A����Ă��郊�[���Ōv�Z
			if (reel[i] == -1)
			{
				r = now_reel + j - 1;
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
				DrawFormatStringF(local_location.x + (i * 20)-20, local_location.y + (j * 15)-20, 0xff0000, "%d", ReelArray[i][r]);
			}
			else
			{
				DrawFormatStringF(local_location.x + (i * 20)-20, local_location.y + (j * 15)-20, 0x00ff00, "%d", ReelArray[i][r]);
			}
		}
	}
	//�y�J���Ă�������炷
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
	//�v���C���[���G�ꂽ�烊�[����]�X�^�[�g
	if (hit_object->GetObjectType() == ePLAYER)
	{
		spin_flg = true;
		can_stop = true;
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
	//���[����]���͎��ԑ���
	if (spin_flg)
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
				if (--now_reel < 0)
				{
					now_reel = REEL_NUM - 1;
				}
			}
			if (reel_wait >= (REEL_WAIT / 4) && reel[0] == -1)
			{
				//�y�J���Ă��Ȃ��ĂV���������炸�炷
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
				//�y�J���Ă��Ȃ��ĂV���������炸�炷
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
				//�y�J���Ă��Ȃ��ĂV���������炸�炷
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
	}
}

void Slot::BonusStop()
{
	//���[����]���͎��ԑ���
	if (spin_flg)
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
				if (--now_reel < 0)
				{
					now_reel = REEL_NUM - 1;
				}
			}
			if (CheckButton(XINPUT_BUTTON_X) && InputPad::OnButton(XINPUT_BUTTON_X) && reel[0] == -1)
			{
				//���ƈ���ɂ��ꂽ��BIG�������Ƃ�����ԂȂ�A���ɂ��炷
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
				//���ƈ���ɂ��ꂽ��BIG�������Ƃ�����ԂȂ�A���ɂ��炷
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
				//���ƈ���ɂ��ꂽ��BIG�������Ƃ�����ԂȂ�A���ɂ��炷
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