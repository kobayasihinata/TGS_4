#include "Enemy3.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy3::Enemy3()
{
	
	move_speed = ENEMY3_SPEED;
	hp = ENEMY3_HP;
	hit_damage = ENEMY3_DAMAGE;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY3_DROPCOIN + (GetRand(2) - 1);

	////�摜�Ǎ�
	//ResourceManager* rm = ResourceManager::GetInstance();
	//std::vector<int>tmp;
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/1.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/2.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/3.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/4.png");
	//animation_image.push_back(tmp[0]);
	//tmp = rm->GetImages("Resource/Images/GoldEnemy/5.png");
	//animation_image.push_back(tmp[0]);
	//image = animation_image[0];
}

Enemy3::~Enemy3()
{

}

void Enemy3::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Enemy3::Finalize()
{

}

void Enemy3::Update()
{
	__super::Update();

	//�ړ�
	Move();

	//�A�j���[�V����
	Animation();


	//�w��̎������ɒe�𔭎˂���
	if (!death_flg && frame % ENEMY3_ATTACK_SPAN == 0)
	{
		//�v���C���[�̈ʒu�Ŕ��ˊp�x�����߂�
		shot_rad = atan2f(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
		manager->CreateAttack(GetBulletData());
	}

	//���S���o�t���O�������Ă���Ȃ�
	if (death_flg)
	{
		//���S���o���Ԃ��߂����玩�g���폜
		if (--death_timer <= 0)
		{
			manager->DeleteObject(this);
			//�R�C�����h���b�v
			for (int i = 0; i < drop_coin; i++)
			{
				manager->CreateObject({ Vector2D{this->location.x + (float)(GetRand(50) - 25),this->location.y + (float)(GetRand(50) - 25)},Vector2D{40,40},eCOIN, 20.f });
			}
		}
	}
}

void Enemy3::Draw()const
{
	__super::Draw();
	//�G3
	DrawString(local_location.x, local_location.y, "enemy3", 0xffffff);
}

void Enemy3::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy3::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

BulletData Enemy3::GetBulletData()
{
	BulletData _data;
	_data.b_angle = shot_rad;
	_data.delete_time = ENEMY3_ATTACK_LIMIT;
	_data.h_count = 1;
	_data.location = this->location;
	_data.radius = 15;
	_data.speed = ENEMY3_ATTACK_SPEED;
	_data.who = this;

	return _data;
}
