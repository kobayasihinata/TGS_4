#include "Player.h"
#include "DxLib.h"
#include "../../Utility/UserData.h"
#include "../ObjectManager.h"
#include "../../Utility/InputPad.h"
#include "../../Utility/common.h"
#include <cmath>
#define _USE_MATH_DEFINES

void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);

	camera = Camera::Get();
	k_input = InputKey::Get();

	shot_rad = 0.f;				

	inv_flg = 0;				
	inv_timer = 0;		
	velocity = Vector2D(0.0f);
	damage_flg = false;
	damage_timer = 0;
	damage_stop = false;

	//HP����
	hp = UserData::player_hp;

	//�摜�Ǎ�
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Player/Idle2.png", 11, 11, 1, 64, 64);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Player/Run2.png", 12, 12, 1, 64, 64);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Player/Hit2.png", 7, 7, 1, 64, 64);
	animation_image.push_back(tmp);

	//�摜�T�C�Y�̔����}�C�i�X
	image_shift = -32;
	//�A�j���[�V���������ݒ�
	anim_span = 3;
	//�A�j���[�V�����ݒ�
	image_line = 0;

	image = animation_image[0][0];
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

	//�_���[�W��̈ړ��s��ԂȂ瑀����󂯕t���Ȃ�
	if (!damage_stop)
	{
		//�e����͏���
		Control();
	}

	//�����ł��ړ����Ă�����\���A�j���[�V������ς���
	if (fabsf(velocity.x) > 0.3f || fabsf(velocity.y) > 0.3f)
	{
		image_line = 1;
	}
	else
	{
		image_line = 0;
	}

	//�ړ�����
	Move();

	//�_���[�W�㖳�G�𑪒肷��
	if (damage_flg && --damage_timer <= 0)
	{
		damage_flg = false;
	}

	//�_���[�W��̈ړ��s��Ԃ��v������
	if (damage_flg && damage_timer <= PLAYER_DAMAGE_CD - PLAYER_DAMAGE_STOP)
	{
		damage_stop = false;
	}

	//�_���[�W��ړ��s��ԂȂ�\���A�j���[�V������ς���
	if (damage_stop)
	{
		image_line = 2;
	}

	//�A�j���[�V��������
	Animation();

	//hp��0�ȉ��̎����S����
	if (hp <= 0)
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
	DrawLineAA(local_location.x, 
		local_location.y,
		local_location.x + (cosf(shot_rad) * 60),
		local_location.y + (sinf(shot_rad) * 60),
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
		damage_stop = true;
		//���t���[�����G
		damage_timer = PLAYER_DAMAGE_CD;
	}
}

void Player::Death()
{
	//�Q�[���I�[�o�[�ɐݒ�
	UserData::is_clear = false;
	//���U���g�J��
	manager->Result();

	//���X�|�[���ʒu�Ɉړ�
	location = 0;

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
		//�����̏���l���z���Ȃ��悤�ɉ��Z����
		if (fabsf(velocity.x) < PLAYER_SPEED)
		{
			velocity.x = InputPad::TipLStick(STICKL_X) * PLAYER_SPEED;
		}
	}
	if (fabsf(InputPad::TipLStick(STICKL_Y)) > 0.1f)
	{
		//�����̏���l���z���Ȃ��悤�ɉ��Z����
		if (fabsf(velocity.y) < PLAYER_SPEED)
		{
			velocity.y = -InputPad::TipLStick(STICKL_Y) * PLAYER_SPEED;
		}
	}

	//�E�X�e�B�b�N�̌X�������ȏ�Ȃ�p�x��ύX����
	if (fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f)
	{
		//�E�X�e�B�b�N�̌X���Ŕ��ˊp�x�����߂� (-1.5f = ���꒲��)
		shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.5f;
	}

	//�R�C�����ꖇ�ȏ�Ȃ�A�P����Œe�𔭎˂���
	if (UserData::coin > 0 && (InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER) || InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER)))
	{
		manager->CreateAttack(GetBulletData());
		UserData::coin--;
	}

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

BulletData Player::GetBulletData()
{
	BulletData _data;
	_data.b_angle = shot_rad;
	_data.delete_time = 60;
	_data.h_count = 3;
	_data.location = this->location;
	_data.radius = 20;
	_data.speed = 15;
	_data.who = this;

	return _data;
}

int Player::GetImages()
{
	return image;
}
