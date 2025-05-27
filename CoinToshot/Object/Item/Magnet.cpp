#include "Magnet.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Magnet::Magnet()
{
	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Item/Heal.png", 7, 5, 2, 32, 41);
	animation_image.push_back(tmp);
	image = animation_image[0][0];


	shine_image = rm->GetImages("Resource/Images/Effect/green_shine.png", 40, 8, 5, 96, 96);
	now_shine_image = 0;
}

Magnet::~Magnet()
{

}

void Magnet::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 5;
}

void Magnet::Finalize()
{

}

void Magnet::Update()
{
	Animation();

	//�P���A�j���[�V�����X�V
	if (++now_shine_image > 39)
	{
		now_shine_image = 0;
	}
}

void Magnet::Draw()const
{
	//�P���`��
	DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, shine_image[now_shine_image], TRUE);

	__super::Draw();
}

void Magnet::Hit(ObjectBase* hit_object)
{
	//�v���C���[�ɐG�ꂽ��X�R�A���Z���ď�����
	if (hit_object->GetObjectType() == ePLAYER)
	{
		UserData::attraction_timer += COIN_ATTRACTON_TIMER;
		manager->DeleteObject(this);
	}
}


