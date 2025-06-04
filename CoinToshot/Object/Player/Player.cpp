#include "Player.h"
#include "DxLib.h"
#include "../../Utility/UserData.h"
#include "../ObjectManager.h"
#include "../../Utility/InputPad.h"
#include "../../Utility/common.h"
#include "PlayerBullet.h"
#include "../Base/EffectList.h"
#include "../../Scene/InGameScene.h"

#include <cmath>
#define _USE_MATH_DEFINES

Player::Player(InGameScene* _ingame)
{
	ingame = _ingame;
}

void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);

	camera = Camera::Get();
	k_input = InputKey::Get();

	bullet_change_cd = 0;
	arrow_image = MakeScreen(100, 100, TRUE);
	shot_rad = 0.f;	
	old_shot_rad = 0.f;

	inv_flg = 0;				
	inv_timer = 0;		
	damage_flg = false;
	damage_timer = 0;
	damage_stop = false;

	//���S�����Ɏg��
	drop_coin = 100;
	death_timer = DEFAULT_DEATH_TIMER * 3;

	//HP����
	max_hp = hp = UserData::player_hp;

	//�摜����
	CreateArrowImage();
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

	//SE�ǂݍ���
	shot_se = rm->GetSounds("Resource/Sounds/shot.mp3");
	walk_se = rm->GetSounds("Resource/Sounds/Player/Walk.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	not_shoot_se = rm->GetSounds("Resource/Sounds/Player/CannotShoot.mp3");
	death_se = rm->GetSounds("Resource/Sounds/explsion_big.mp3");

	//���ʒ���
	SetVolumeSoundMem(7500, walk_se);
	//		PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);

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
	hp = UserData::player_hp;

	__super::Update();

	//�z���񂹏����̎��ԑ���
	if (--UserData::attraction_timer < 0)
	{
		UserData::attraction_timer = 0;
	}
	//�z���񂹂̎��Ԃ��P�ȏ�Ȃ�t���O�𗧂Ă�
	UserData::attraction_flg = (UserData::attraction_timer > 0) ? true : false;

	//�J�����ɍ��W��n��
	camera->player_location = this->location;

	//�_���[�W��̈ړ��s��ԂȂ瑀����󂯕t���Ȃ� ����łĂ��󂯕t���Ȃ�
	if (!damage_stop && !death_flg)
	{
		//�e����͏���
		Control();
	}

	//�e��ޕύX�N�[���_�E������
	if (bullet_change_cd > 0)
	{
		bullet_change_cd--;
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
	CreateArrowImage();

	//�A�j���[�V�����ʒu�ɉ����đ�����炷
	if (image_line == 1 &&
		(image_num == 4 || image_num == 10)&&
		!CheckSoundMem(walk_se))
	{
		PlaySoundMem(walk_se, DX_PLAYTYPE_BACK);
	}
	//hp��0�ȉ��̎����S����
	if (hp <= 0)
	{
		death_flg = true;
	}

	//���S���o�t���O�������Ă���Ȃ�
	if (death_flg)
	{
		//�z���񂹋����I��
		UserData::attraction_timer = 0;
		death_timer--;
		//���ɂȂ���R�C�����܂��U�炷
		if (death_timer % 10 == 0 && drop_coin_count < drop_coin && death_timer >= DEFAULT_DEATH_TIMER * 2)
		{
			Vector2D rand = { (float)(GetRand(40) - 20),(float)(GetRand(40) - 20) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
			//���SSE�Đ�
			PlaySoundMem(death_se, DX_PLAYTYPE_BACK);
			//�����G�t�F�N�g
			manager->CreateEffect(elExplosion, { location.x + (GetRand(100) - 50),location.y + (GetRand(100) - 50) });
		}

		//���S���o���Ԃ��߂����玩�g���폜
		if (death_timer == DEFAULT_DEATH_TIMER * 2)
		{
			//���o���ɏo���Ȃ������R�C�����܂Ƃ߂ăh���b�v
			for (int i = drop_coin_count; i < drop_coin; i++)
			{
				Vector2D rand = { (float)(GetRand(20) - 10),(float)(GetRand(20) - 10) };
				manager->CreateObject(
					eCOIN,
					this->location + (rand*2),
					Vector2D{ 40, 40 },
					20.f,
					rand);
			}
			//���SSE�Đ�
			PlaySoundMem(death_se, DX_PLAYTYPE_BACK);
		}
		if (death_timer <= 0)
		{
			Death();
		}
	}

#ifdef _DEBUG
	//if (frame % 30 == 0)
	//{
	//	manager->CreateEffect(elSmoke, this->location);
	//}
	
#endif // _DEBUG

}

void Player::Draw()const
{
	//�t�H���g�T�C�Y�ۑ�
	int old = GetFontSize();

	//�z���񂹃G�t�F�N�g
	if (UserData::attraction_flg)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawCircleAA(local_location.x, local_location.y, -frame * i % 60, 30, 0x00ffff, false);
		}
	}

	//�e�̋O���`��
	DrawBulletOrbit();

	if ((!damage_flg || (damage_flg && frame % 3 == 0)) && (!death_flg || death_timer > DEFAULT_DEATH_TIMER * 2))
	{
		__super::Draw();
	}

	//�t�H���g�傫�����ʂ�
	SetFontSize(old);
}

void Player::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Player::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	//�_���[�W�㖳�G�łȂ��Ȃ�_���[�W���󂯂�
	if (!damage_flg)
	{
		__super::Damage(_value, _attack_loc, _knock_back);
		UserData::player_hp = hp;
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
	manager->Result(120);

	//���X�|�[���ʒu�Ɉړ�
	location = 0;

	//�e�l���Z�b�g
	UserData::player_hp = hp = DEFAULT_HP;
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
			//�ړ������������ۑ�
			move_velocity.x = velocity.x;
		}
	}
	if (fabsf(InputPad::TipLStick(STICKL_Y)) > 0.1f)
	{
		//�����̏���l���z���Ȃ��悤�ɉ��Z����
		if (fabsf(velocity.y) < PLAYER_SPEED)
		{
			velocity.y = -InputPad::TipLStick(STICKL_Y) * PLAYER_SPEED;
			//�ړ������������ۑ�
			move_velocity.y = velocity.y;
		}
	}

	//�E�X�e�B�b�N�̌X�������ȏ�Ȃ�p�x��ύX����
	if (fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f)
	{
		//�O�̌X����ۑ�����
		old_shot_rad = shot_rad;
		//�E�X�e�B�b�N�̌X���Ŕ��ˊp�x�����߂� (-1.5f = ���꒲��)
		shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.55f;
		//�p�x�����ȏ�ύX����Ă�����SE��炷
		if (fabsf(old_shot_rad - shot_rad) > 0.01f && frame % 4==0)
		{
			PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
		}
	}

	//�R�C�����ꖇ�ȏ�Ȃ�A�P����Œe�𔭎˂���
	if (InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER) || InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		if (UserData::coin >= pBullet[UserData::bullet_type].cost)
		{
			ShotBullet();
			UserData::coin -= pBullet[UserData::bullet_type].cost;
			PlaySoundMem(shot_se, DX_PLAYTYPE_BACK);
			std::string s = "-" + std::to_string(pBullet[UserData::bullet_type].cost);
			ingame->CreatePopUp(this->location, s, 0xff0000, -1);
		}
		//���ˎ��sSE
		else
		{
			PlaySoundMem(not_shoot_se, DX_PLAYTYPE_BACK);
		}
	}

	//�N�[���_�E�����I����Ă�����e��ύX�o����
	if (bullet_change_cd <= 0)
	{
		//�g���K�[�Œe�̎�ނ�ς���
		if (InputPad::OnPressed(L_TRIGGER))
		{
			if (--UserData::bullet_type < 0)
			{
				UserData::bullet_type = BULLET_NUM - 1;
			}
			bullet_change_cd = PLATER_BULLET_CHANGE_CD;
		}
		if (InputPad::OnPressed(R_TRIGGER))
		{
			if (++UserData::bullet_type > BULLET_NUM - 1)
			{
				UserData::bullet_type = 0;
			}
			bullet_change_cd = PLATER_BULLET_CHANGE_CD;
		}
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

BulletData Player::GetBulletData(float _shot_rad)
{
	BulletData _data;
	_data.damage = pBullet[UserData::bullet_type].damage;
	_data.b_angle = _shot_rad;
	_data.delete_time = pBullet[UserData::bullet_type].life_span;
	_data.h_count = pBullet[UserData::bullet_type].h_count;
	_data.location = this->location;
	_data.radius = pBullet[UserData::bullet_type].radius;
	_data.speed = pBullet[UserData::bullet_type].speed;
	_data.who = this;
	_data.b_type = (BulletType)UserData::bullet_type;

	return _data;
}

void Player::ShotBullet()
{
	//�ꔭ�ȏ㌂�e��Ȃ�
	if (pBullet[UserData::bullet_type].bullet_num > 1)
	{
		float t = (pBullet[UserData::bullet_type].space * pBullet[UserData::bullet_type].bullet_num) / 2;
		for (int i = 0; i < pBullet[UserData::bullet_type].bullet_num; i++)
		{
			manager->CreateAttack(GetBulletData(shot_rad - t + i * pBullet[UserData::bullet_type].space));
		}
	}
	//�P���Ȃ�
	else
	{
		manager->CreateAttack(GetBulletData(shot_rad));
	}
}

void Player::DrawBulletOrbit()const
{
	DrawRotaGraph(local_location.x+45*sinf(shot_rad + 1.6f), local_location.y-45*cosf(shot_rad + 1.6f), 1.0f, shot_rad + 1.5f, arrow_image, TRUE);
}

void Player::CreateArrowImage()const
{
	SetDrawScreen(arrow_image);
	ClearDrawScreen();
	for (int i = 1; i < 5; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20)*10));
		DrawQuadrangleAA(50, (i * 20) - (frame % 20),
			25, 25 + (i * 20) - (frame % 20),
			30, 30 + (i * 20) - (frame % 20),
			50, 10 + (i * 20) - (frame % 20),
			0xff0000, TRUE);
		DrawQuadrangleAA(50, (i * 20) - (frame % 20),
			75, 25 + (i * 20) - (frame % 20),
			70, 30 + (i * 20) - (frame % 20),
			50, 10 + (i * 20) - (frame % 20),
			0xff0000, TRUE);
	}
	//�������߃��Z�b�g
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawScreen(DX_SCREEN_BACK);
}