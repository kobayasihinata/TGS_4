#include "Player.h"
#include "DxLib.h"
#include "../../Utility/UserData.h"
#include "../ObjectManager.h"
#include "../../Utility/InputPad.h"
#include "../../Utility/common.h"
#include <cmath>
#define _USE_MATH_DEFINES

void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);

	camera = Camera::Get();
	k_input = InputKey::Get();

	shot_rad = 0.f;				

	inv_flg = 0;				
	inv_timer = 0;		
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

void Player::Update()
{
	//HP����
	UserData::player_hp = hp;

	__super::Update();

	//�J�����ɍ��W��n��
	camera->player_location = this->location;

	//�e����͏���
	Control();

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

void Player::Draw()const
{
	if (!damage_flg || (damage_flg && frame % 3 == 0))
	{
		__super::Draw();
	}
	//DebugInfomation::Add("hp", UserData::player_hp);
	DebugInfomation::Add("rad", shot_rad);

	//���ˊp�x�`��
	DrawLineAA(local_location.x + (box_size.x/2), 
		local_location.y + (box_size.y / 2),
		local_location.x + (box_size.x / 2) + (cosf(shot_rad) * 60),
		local_location.y + (box_size.y / 2) + (sinf(shot_rad) * 60),
		0xff0000,
		TRUE);

}

void Player::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);

}

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

void Player::Control()
{
	//���X�e�B�b�N�̌X�������ȏ�Ȃ�ړ�����
	if (fabsf(InputPad::TipLStick(STICKL_X)) > 0.1f)
	{
		velocity.x = InputPad::TipLStick(STICKL_X) * PLAYER_SPEED;
	}
	if (fabsf(InputPad::TipLStick(STICKL_Y)) > 0.1f)
	{
		velocity.y = -InputPad::TipLStick(STICKL_Y) * PLAYER_SPEED;
	}

	//�E�X�e�B�b�N�̌X�������ȏ�Ȃ�p�x��ύX����
	if (fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f)
	{
		//�E�X�e�B�b�N�̌X���Ŕ��ˊp�x�����߂� (-1.5f = ���꒲��)
		shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.5f;
	}

	//�g���K�[�Œe�𔭎˂���
	if (InputPad::OnButton(L_TRIGGER) || InputPad::OnButton(R_TRIGGER))
	{
		manager->CreateAttack(this->location, Vector2D{40, 40}, this, 30, shot_rad);
	}

	//�p�x�����Z����
	//if (InputPad::OnPressed(XINPUT_BUTTON_A))
	//{
	//	shot_rad += 0.01f;
	//}
#ifdef _DEBUG
	//���L�[�ō��ړ�
	if (k_input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Held)
	{
		velocity.x = -10;
	}
	//�E�L�[�ŉE�ړ�
	if (k_input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Held)
	{
		velocity.x = 10;
	}
	//��L�[�ŏ�ړ�
	if (k_input->GetKeyState(KEY_INPUT_UP) == eInputState::Held)
	{
		velocity.y = -10;
	}
	//���L�[�ŉ��ړ�
	if (k_input->GetKeyState(KEY_INPUT_DOWN) == eInputState::Held)
	{
		velocity.y = 10;
	}
#endif // _DEBUG
}

/// <summary>
/// �`��摜���擾
/// </summary>
/// <returns>�摜�n���h��</returns>
int Player::GetImages()
{
	return image;
}
