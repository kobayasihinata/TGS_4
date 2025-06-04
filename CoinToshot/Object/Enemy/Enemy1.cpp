#include "Enemy1.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy1::Enemy1()
{
	move_speed = ENEMY1_SPEED;
	max_hp = hp = ENEMY1_HP;
	hit_damage = ENEMY1_DAMAGE;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY1_DROPCOIN + (GetRand(2) - 1);

	//���A�̂Ȃ�R�C���h���b�v��2�{�AHP��1.5�{�A�ړ����x�́{0.5
	if (rare_flg)
	{
		move_speed += 0.5f;
		max_hp = hp *= 1.5f;
		drop_coin *= 2;
	}

	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy1/Enemy1_Walk.png",24,5,5,96,96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy1/Enemy1_Death.png",15,5,3,96,96);
	animation_image.push_back(tmp);
	//tmp = rm->GetImages("Resource/Images/Player/Run2.png", 12, 12, 1, 64, 64);
	//animation_image.push_back(tmp);
	image = animation_image[0][0];
	
	//SE�ǂݍ���
	death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
}

Enemy1::~Enemy1()
{

}

void Enemy1::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 2;
}

void Enemy1::Finalize()
{

}

void Enemy1::Update()
{
	__super::Update();

	//�ړ�
	Move();

	//�A�j���[�V����
	Animation();

	//���S���o�t���O�������Ă���Ȃ�
	if (death_flg)
	{
		//���S�A�j���[�V������\��
		image_line = 1;
		anim_span = 3;

		//���ɂȂ���R�C�����܂��U�炷
		if (++death_timer % 20 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
			manager->CreateObject(
				eCOIN, 
				this->location + rand,
				Vector2D{40, 40}, 
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
				Vector2D rand = {(float)(GetRand(25) - 12),(float)(GetRand(25) - 12) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{40, 40},
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

void Enemy1::Draw()const
{
	__super::Draw();
}

void Enemy1::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
	
	//location -= (velocity * 10);
}

void Enemy1::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc, _knock_back);

}

