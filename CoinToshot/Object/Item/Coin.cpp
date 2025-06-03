#include "Coin.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Coin::Coin(InGameScene* _ingame, Vector2D _init_velocity)
{
	camera = Camera::Get();

	ingame = _ingame;
	velocity = _init_velocity;
	add_num = 1;
	check_flg = false;

	color_r = GetRand(255);
	color_g = GetRand(255);
	color_b = GetRand(255);
	//SE�ǂݍ���
	ResourceManager* rm = ResourceManager::GetInstance();

	get_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
}

Coin::~Coin()
{

}

void Coin::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Coin::Finalize()
{
	
}

void Coin::Update()
{
	Move();

	//�l�������ɉ����ăf�J���Ȃ�
	radius = 20.f + add_num;
	box_size = radius * 2;
	//�z���񂹂̃t���O�������Ă�����A�v���C���[�Ɍ����Ĉړ�����
	if (UserData::attraction_flg)
	{
		velocity.x += ((camera->player_location.x - this->location.x) / 200);
		velocity.y += ((camera->player_location.y - this->location.y) / 200);
	}
	if (++frame % 30 == 0)
	{
		manager->CreateEffect(elShine, { location.x+GetRand((int)radius)-(radius/2),location.y + GetRand((int)radius) - (radius / 2) }, TRUE, GetColor(GetRand(255), GetRand(255), GetRand(255)));
	}
}

void Coin::Draw()const
{
	__super::Draw();

	UserData::DrawCoin(local_location, radius, color_r, color_g, color_b);
#ifdef _DEBUG
	//���g�̊l��������\��
	DrawFormatStringF(local_location.x, local_location.y, 0xff0000, "%d", (int)add_num);
#endif
}

void Coin::Hit(ObjectBase* hit_object)
{
	//�G�ꂽ�Ώۂ�����ł�����I��
	if (hit_object->GetDeathFlg())
	{
		return;
	}
	//�G�ꂽ�Ώۂ��R�C���ŁA�t���O�������Ă���Ȃ�I��
	//if (hit_object->GetObjectType() == eCOIN && check_flg)
	//{
	//	return;
	//}
	//�G�ꂽ�Ώۂ��v���C���[�ƃR�C���ȊO�Ȃ�I��
	if (hit_object->GetObjectType() != ePLAYER && hit_object->GetObjectType() != eCOIN)
	{
		return;
	}
	
	//�R�C�����m�������񂹂���
	if (hit_object->GetObjectType() == eCOIN && 
		UserData::player_hp > 0 &&
		COIN_FUSION_RANGE < sqrtf(powf(this->location.x - hit_object->GetLocation().x,2) + powf(this->location.y - hit_object->GetLocation().y,2)) &&
		this->add_num > 0 && 
		static_cast<Coin*>(hit_object)->add_num > 0)
	{
		location.x += ((hit_object->GetLocation().x - this->location.x) / 200);
		location.y += ((hit_object->GetLocation().y - this->location.y) / 200);
	}


	//�R�C���ɐG�ꂽ�炻�̃R�C���������A���̃R�C���̊l�����������Z����
	//�����P�A�Q:velocity��0(�ړ����Ă��Ȃ�) or ���W�����S�Ɉ�v
	//�����R:hit_object��Coin�ł���
	//�����S:�v���C���[������ł��Ȃ�
	//�����T:���g��hit_object��_�Ԃ̋������w�肵���l�ȉ�
	//�����U:���g�̃R�C�����Z����0�ȏ�
	//�����V:hit_object(coin)�̉��Z����0�ȏ�
	if ((((int)velocity.x == 0 && (int)velocity.y == 0)||
		(location == hit_object->GetLocation()))
		&&
		hit_object->GetObjectType() == eCOIN &&
		UserData::player_hp > 0 &&
		COIN_FUSION_RANGE > sqrtf(powf(this->location.x - hit_object->GetLocation().x, 2) + powf(this->location.y - hit_object->GetLocation().y, 2)) &&
		this->add_num > 0 &&
		static_cast<Coin*>(hit_object)->add_num > 0
		)
	{
		add_num += static_cast<Coin*>(hit_object)->add_num;
		static_cast<Coin*>(hit_object)->add_num = 0;
		manager->DeleteObject(hit_object);

		return;
	}

	if ((int)velocity.x == 0 &&
		(int)velocity.y == 0)
	{
		//��̏����͈ړ���~��̈�񂾂�
		check_flg = true;
	}

	//�v���C���[�ɐG�ꂽ��R�C�����Z���ď�����
	if (this->add_num > 0 && hit_object->GetObjectType() == ePLAYER)
	{
		UserData::coin += add_num;
		std::string s = "+" + std::to_string((int)add_num);
		ingame->CreatePopUp(this->location, s, GetRand(100), 0xffff00, 1, 60);

		manager->DeleteObject(this);
		PlaySoundMem(get_se, DX_PLAYTYPE_BACK);
		for (int i = 0; i < 10; i++)
		{
			manager->CreateEffect(elShine, { location.x + GetRand((int)radius) - (radius / 2),location.y + GetRand((int)radius) - (radius / 2) }, TRUE, 0xffff00);
		}
		return;
	}
}

