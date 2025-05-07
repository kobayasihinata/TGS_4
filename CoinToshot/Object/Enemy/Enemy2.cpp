#include "Enemy2.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy2::Enemy2()
{
	move_speed = ENEMY2_SPEED;
	hp = ENEMY2_HP;
	hit_damage = ENEMY2_DAMAGE;
	//�w�肵���h���b�v�ʂ���}1�̊ԂŃ����_���ɃR�C�����h���b�v
	drop_coin = ENEMY2_DROPCOIN + (GetRand(2) - 1);

	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy2/Enemy2_Run.png", 12, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy2/Enemy2_Death.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);

	image = animation_image[0][0];
}

Enemy2::~Enemy2()
{

}

void Enemy2::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 2;
}

void Enemy2::Finalize()
{

}

void Enemy2::Update()
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

void Enemy2::Draw()const
{
	__super::Draw();
	//�G2
	DrawString(local_location.x, local_location.y, "enemy2", 0xffffff);
}

void Enemy2::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy2::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

