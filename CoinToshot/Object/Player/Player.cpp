#include "Player.h"

#include <cmath>

#include "DxLib.h"
#include "../../Utility/UserData.h"
#include "../ObjectManager.h"

/// <summary>
/// ����������
/// </summary>
/// <param name="init_location">�������W</param>
void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);

	camera = Camera::Get();

	g_velocity = 0.0f;
	velocity = Vector2D(0.0f);
	damage_flg = false;
	damage_timer = 0;

	//HP����
	hp = UserData::player_hp;

	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Tri-pilot/1.png");
	animation_image.push_back(tmp[0]);
	tmp = rm->GetImages("Resource/Images/Tri-pilot/2.png");
	animation_image.push_back(tmp[0]);
	image = animation_image[0];
}

/// <summary>
/// �I������
/// </summary>
void Player::Finalize()
{

}

/// <summary>
/// �X�V����
/// </summary>
void Player::Update()
{
	//HP����
	UserData::player_hp = hp;

	__super::Update();

	//�J�����ɍ��W��n��
	camera->player_location = this->location;

	//�ړ�����
	Move();

	//�A�j���[�V��������
	Animation();

	//hp��0�ȉ��̎����S����
	if (hp <= 0 )
	{
		Death();
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Draw()const
{
	if (!damage_flg || (damage_flg && frame % 3 == 0))
	{
		__super::Draw();
	}
	DebugInfomation::Add("hp", UserData::player_hp);
}

/// <summary>
/// �����蔻�菈��
/// </summary>
/// <param name="hit_Object">���ׂ鑊��</param>
void Player::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);

}

/// <summary>
/// �_���[�W����
/// </summary>
/// <param name="value">�󂯂��_���[�W��</param>
void Player::Damage(float _value, Vector2D _attack_loc)
{
	//�_���[�W�㖳�G�łȂ��Ȃ�_���[�W���󂯂�
	if (!damage_flg)
	{
		__super::Damage(_value, _attack_loc);
		damage_flg = true;
		//120�t���[�����G
		damage_timer = 120;
	}
}

/// <summary>
/// ���S����
/// </summary>
void Player::Death()
{
	//�c�@�}�C�i�X�P
	UserData::life--;

	//�c�@���O�ȉ��ɂȂ�����Q�[���I�[�o�[
	if (UserData::life < 0)
	{
		//�Q�[���I�[�o�[�ɐݒ�
		UserData::is_clear = false;
		//���U���g�J��
		manager->Result();
	}

	//���X�|�[���ʒu�Ɉړ�
	location = UserData::spawn_loc[UserData::now_stage];

	//�e�l���Z�b�g
	hp = DEFAULT_HP;
	UserData::player_hp = hp;
	velocity = 0;
	damage_flg = false;
	damage_timer = 0;
	inv_flg = false;
	inv_timer = 0;
}

/// <summary>
/// �`��摜���擾
/// </summary>
/// <returns>�摜�n���h��</returns>
int Player::GetImages()
{
	return image;
}
