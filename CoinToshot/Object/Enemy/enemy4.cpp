#include "Enemy4.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"
#include "../../Scene/InGameScene.h"

Enemy4::Enemy4(InGameScene* _ingame)
{
	ingame = _ingame;

	move_speed = ENEMY4_SPEED;
	max_hp = hp = ENEMY4_HP;
	hit_damage = ENEMY4_DAMAGE;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY4_DROPCOIN + (GetRand(2) - 1);
	//���A�̂Ȃ�R�C���h���b�v��2�{�A�ړ����x�́{10
	if (rare_flg)
	{
		move_speed += 6;
		drop_coin *= 2;
	}
	coin_num = 0;	//�����Ă���R�C��
	steal_flg = false;	//�R�C���𓐂񂾂�

	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy4/Enemy4_Run.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy4/Enemy4_Walk.png", 24, 5, 5, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy4/Enemy4_Death.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);

	image = animation_image[0][0];

	//SE�ǂݍ���
	death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
}

Enemy4::~Enemy4()
{

}

void Enemy4::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 1;
}

void Enemy4::Finalize()
{

}

void Enemy4::Update()
{
	/*********ActorBase�̓��̏���*********/
	frame++;

	//��~�O�Ɉړ����Ă�������ۑ�����
	if (fabsf(velocity.x) > MOVE_LOWER_LIMIT)
	{
		last_velocity.x = velocity.x;
	}
	if (fabsf(velocity.y) > MOVE_LOWER_LIMIT)
	{
		last_velocity.y = velocity.y;
	}

	//HP�̌����������̃A�j���[�V�����p
	if (hpbar_move > 1)
	{
		hpbar_move -= hpbar_move / 10;
	}
	else
	{
		hpbar_move = 0;
	}
	/*************************************/

	//�s���̏���
	Enemy4Move();

	//�ړ�
	Move();

	//�R�C���𓐂񂾌�A�������Ă���Ȃ�A�A�j���[�V����������ɂ���
	if (steal_flg && !death_flg)
	{
		image_line = 1;
	}

	//�A�j���[�V����
	Animation();

	//���S���o�t���O�������Ă���Ȃ�
	if (death_flg)
	{
		//���S���A�j���[�V�����ɑJ��
		image_line = 2;
		anim_span = 3;

		//���ɂȂ���R�C�����܂��U�炷
		if (++death_timer % 15 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand((int)this->GetSize().x)) - this->GetSize().x / 2,(float)(GetRand((int)this->GetSize().y)) - this->GetSize().y / 2 };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
		}

		//���S���o���Ԃ��߂����玩�g���폜
		if (anim_end_flg)
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

			//SE�Đ�
			PlaySoundMem(death_se, DX_PLAYTYPE_BACK);

			//���A�̂Ȃ�A�C�e������
			if (rare_flg)
			{
				//�񕜂�����
				if (GetRand(1) == 0)
				{
					manager->CreateObject(
						eHEAL,
						this->location,
						Vector2D{40, 40});
				}
				else
				{
					manager->CreateObject(
						eMAGNET,
						this->location,
						Vector2D{40, 40});
				}
			}
		}
	}
}

void Enemy4::Draw()const
{
	__super::Draw();
}

void Enemy4::Hit(ObjectBase* hit_object)
{
	//�v���C���[�ɓ���������R�C����D���A�R�C����D�����t���O�𗧂Ă�
	if (!death_flg && !steal_flg && hit_object->GetObjectType() == ePLAYER)
	{
		//�R�C���ʂ����ޗʂ�葽����΋K��ʓ���
		if (UserData::coin > ENEMY4_STEAL)
		{
			UserData::coin -= ENEMY4_STEAL;
			coin_num = ENEMY4_STEAL;
		}
		//�R�C���ʂ����ޗʂ�菭�Ȃ���ΗL����S�ē���
		else
		{
			coin_num = UserData::coin;
			UserData::coin = 0;
		}
		drop_coin += coin_num * 2;
		move_speed = rare_flg ? ENEMY4_ESCAPE_SPEED : ENEMY4_ESCAPE_SPEED * 2;
		steal_flg = true;

		std::string s = "-" + std::to_string(coin_num);
		ingame->CreatePopUp(this->location, s, 0xff0000, -1);
	}
}

void Enemy4::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc);
}

void Enemy4::Enemy4Move()
{		
	//���S���Ă��Ȃ���΂��̏���
	if (!this->death_flg)
	{
		//�v���C���[�ւ̊p�x�v�Z
		double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		//�v���C���[����R�C���𓐂�ł��Ȃ���΁A�v���C���[�Ɍ������Đi��
		if (!steal_flg)
		{
			//�ړ��̏���l�ݒ�
			if (fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
			if (fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);
		}
		//�v���C���[����R�C���𓐂�ł���΁A�v���C���[�Ƃ͔��΂ɐi��
		else
		{
			//�ړ��̏���l�ݒ�
			if (fabsf(velocity.x) < move_speed)velocity.x -= move_speed * cos(radian);
			if (fabsf(velocity.y) < move_speed)velocity.y -= move_speed * sin(radian);
		}
		//�ړ������������ۑ�
		//move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
		move_velocity = velocity;
	}
}
