#include "Enemy5.h"
#include "../ObjectManager.h"
#include "../../Utility/common.h"

Enemy5::Enemy5()
{

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

void Enemy5::Draw()const
{
	__super::Draw();
	//�G3
	DrawString(local_location.x, local_location.y, "enemy4", 0xffffff);
}

void Enemy5::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Enemy5::Damage(float _value, Vector2D _attack_loc)
{
	__super::Damage(_value, _attack_loc);
}

