#include "Enemy1.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy1::Enemy1()
{
	move_speed = ENEMY1_SPEED;
	hp = ENEMY1_HP;
	hit_damage = ENEMY1_DAMAGE;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY1_DROPCOIN + (GetRand(2) - 1);

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
		if (death_timer % 20 == 0 && drop_coin_count < drop_coin)
		{
			manager->CreateObject(
				eCOIN, 
				this->location, 
				Vector2D{40, 40}, 
				20.f, 
				Vector2D{ (float)(GetRand(20) - 10),(float)(GetRand(20) - 10) });
			drop_coin_count++;
		}

		//���S���o���Ԃ��߂����玩�g���폜
		if (--death_timer <= 0)
		{
			manager->DeleteObject(this);
			//���o���ɏo���Ȃ������R�C�����܂Ƃ߂ăh���b�v
			for (int i = drop_coin_count; i < drop_coin; i++)
			{
				Vector2D rand = {(float)(GetRand(30) - 15),(float)(GetRand(30) - 15) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{40, 40},
					20.f,
					rand);
			}
		}
	}
}

void Enemy1::Draw()const
{
	__super::Draw();
	//�G1
	DrawString(local_location.x, local_location.y, "enemy1", 0xffffff);
}

void Enemy1::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
	
	//location -= (velocity * 10);
}

void Enemy1::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);

}

