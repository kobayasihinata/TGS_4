#include "Enemy5.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy5::Enemy5()
{
	move_speed = ENEMY5_SPEED;
	hp = ENEMY5_HP;
	hit_damage = ENEMY5_DAMAGE;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY5_DROPCOIN + (GetRand(2) - 1);

	block_anim_timer = 0;
}

Enemy5::~Enemy5()
{

}

void Enemy5::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Enemy5::Finalize()
{

}

void Enemy5::Update()
{
	__super::Update();

	//�ړ�
	Move();

	//�A�j���[�V����
	Animation();

	//�A�j���[�V�����^�C�}�[���Z
	if (block_anim_timer > 0)
	{
		block_anim_timer--;
	}

	//���S���o�t���O�������Ă���Ȃ�
	if (death_flg)
	{
		//���ɂȂ���R�C�����܂��U�炷
		if (death_timer % 10 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(40) - 20),(float)(GetRand(40) - 20) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{40, 40},
				20.f,
				rand);
			drop_coin_count++;
		}

		//���S���o���Ԃ��߂����玩�g���폜
		if (--death_timer <= 0)
		{
			manager->DeleteObject(this);
			//���o���ɏo���Ȃ������R�C�����܂Ƃ߂ăh���b�v
			for (int i = drop_coin_count; i < drop_coin; i++)
			{
				Vector2D rand = { (float)(GetRand(20) - 10),(float)(GetRand(20) - 10) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{ 40, 40 },
					20.f,
					rand);
			}
		}
	}
}

void Enemy5::Draw()const
{
	__super::Draw();
	//�G3
	DrawString(local_location.x, local_location.y, "enemy5", 0xffffff);

	//�U���u���b�N�A�j���[�V����
	if (block_anim_timer > 0)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawCircleAA(attack_loc.x - camera->GetCameraLocation().x, attack_loc.y - camera->GetCameraLocation().y, block_anim_timer * i%30, 6, 0xff0000, false);
		}
	}
}

void Enemy5::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy5::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	//���Ɏ���_���[�W���󂯂��Ȃ�m�b�N�o�b�N����
	if (hp - _value <= 0)
	{
		__super::Damage(_value, _attack_loc , _knock_back);
	}
	else
	{
		__super::Damage(_value, _attack_loc, 0);

	}

	//�u���b�N�A�j���[�V�����J�n
	block_anim_timer = 20;
	attack_loc = _attack_loc;
}

