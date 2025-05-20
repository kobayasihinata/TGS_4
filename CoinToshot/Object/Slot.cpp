#include "Slot.h"
#include "../Utility/InputPad.h"

Slot::Slot()
{
	spin_flg = false;
	timer = 0;
	now_reel = 0;
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
	if (spin_flg)
	{
		if (++timer > 300)
		{
			timer = 0;
		}
		if (timer % 5 == 0)
		{
			if (--now_reel < 0)
			{
				now_reel = REAL_NUM -1;
			}
		}
		if (InputPad::OnButton(XINPUT_BUTTON_X))
		{
			reel[0] = now_reel;
		}
		if (InputPad::OnButton(XINPUT_BUTTON_Y))
		{
			reel[1] = now_reel;
		}
		if (InputPad::OnButton(XINPUT_BUTTON_B))
		{
			reel[2] = now_reel;
		}
	}
}

void Slot::Draw()const
{
	__super::Draw();
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
			r = (r + REAL_NUM) % REAL_NUM;
			//特定の数字だけ描画色を変える
			if (ReelArray[i][r] == 7)
			{
				DrawFormatStringF(local_location.x + (i * 20), local_location.y + (j * 15), 0xff0000, "%d", ReelArray[i][r]);
			}
			else
			{
				DrawFormatStringF(local_location.x + (i * 20), local_location.y + (j * 15), 0x00ff00, "%d", ReelArray[i][r]);
			}
		}
	}
}

void Slot::Hit(ObjectBase* hit_object)
{
	if (hit_object->GetObjectType() == ePLAYER)
	{
		spin_flg = true;
	}
}

void Slot::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{

}
