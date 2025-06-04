#include "Enemy3.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy3::Enemy3()
{
	
	move_speed = ENEMY3_SPEED;
	max_hp = hp = ENEMY3_HP;
	hit_damage = ENEMY3_DAMAGE;
	shot_span = ENEMY3_ATTACK_SPAN;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY3_DROPCOIN + (GetRand(2) - 1);
	//���A�̂Ȃ�R�C���h���b�v��2�{�AHP��1.5�{�A�e���ˑ��x5�{
	if (rare_flg)
	{
		max_hp = hp *= 1.5f;
		drop_coin *= 2;
		shot_span /= 5;
	}
	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy3/Enemy3_Idle.png", 18, 5, 4, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy3/Enemy3_Throw.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy3/Enemy3_Death.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);

	image = animation_image[0][0];

	shot_once = false;
	
	//SE�ǂݍ���
	death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
}

Enemy3::~Enemy3()
{

}

void Enemy3::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 2;
}

void Enemy3::Finalize()
{

}

void Enemy3::Update()
{
	__super::Update();

	//�ړ�
	Move();

	//�����Ă�Ȃ�A��̕��������ς���
	if (!death_flg)
	{
		if (camera->player_location.x > this->location.x)
		{
			move_velocity.x = 1.f;
		}
		else
		{
			move_velocity.x = -1.f;
		}
	}

	//�A�j���[�V����
	Animation();

	//�����A�j���[�V���������������ʏ�A�j���[�V�����ɖ߂�
	if (image_line == 1 && anim_end_flg)
	{
		image_line = 0;
	}

	//�����A�j���[�V�������w��̉摜�܂ōĐ����ꂽ��A�e�𔭎˂���
	if (!shot_once && image_line == 1 && image_num == 4)
	{
		//�v���C���[�̈ʒu�Ŕ��ˊp�x�����߂�
		shot_rad = atan2f(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		manager->CreateAttack(GetBulletData());
		//��񂾂�����
		shot_once = true;
	}
	//�w��̎������ɃA�j���[�V�����𓊝��ɕς���
	if (!death_flg && frame % shot_span == 0)
	{
		//�����A�j���[�V�����J�n
		image_line = 1;
		anim_timer = 0;
		image_num = 0;
		//��񂾂����p�ϐ����Z�b�g
		shot_once = false;
	}

	//���S���o�t���O�������Ă���Ȃ�
	if (death_flg)
	{

		//���S�A�j���[�V������\��
		image_line = 2;
		anim_span = 3;

		//���ɂȂ���R�C�����܂��U�炷
		if (++death_timer % 20 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
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
			//�G�t�F�N�g
			manager->CreateEffect(elExplosion, this->location);
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

void Enemy3::Draw()const
{
	__super::Draw();
}

void Enemy3::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy3::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc,_knock_back);
}

BulletData Enemy3::GetBulletData()
{
	BulletData _data;
	_data.b_angle = shot_rad;
	_data.damage = ENEMY3_DAMAGE;
	_data.delete_time = ENEMY3_ATTACK_LIMIT;
	_data.h_count = 1;
	_data.location = this->location;
	_data.radius = 15;
	_data.speed = ENEMY3_ATTACK_SPEED;
	_data.who = this;
	_data.b_type = BulletType::bNormal;
	return _data;
}
