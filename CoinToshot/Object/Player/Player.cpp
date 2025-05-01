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

	//HP同期
	hp = UserData::player_hp;

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Player/Idle2.png", 11, 11, 1, 64, 64);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Player/Run2.png", 12, 12, 1, 64, 64);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Player/Hit2.png", 7, 7, 1, 64, 64);
	animation_image.push_back(tmp);

	//画像サイズの半分マイナス
	image_shift = -32;
	//アニメーション周期設定
	anim_span = 3;
	//アニメーション設定
	image_line = 0;

	image = animation_image[0][0];
}

/// <summary>
/// 終了処理
/// </summary>
void Player::Finalize()
{

}

void Player::Update()
{
	//HP同期
	UserData::player_hp = hp;

	__super::Update();

	//カメラに座標を渡す
	camera->player_location = this->location;

	//ダメージ後の移動不可状態なら操作を受け付けない
	if (!damage_stop)
	{
		//各種入力処理
		Control();
	}

	//少しでも移動していたら表示アニメーションを変える
	if (fabsf(velocity.x) > 0.3f || fabsf(velocity.y) > 0.3f)
	{
		image_line = 1;
	}
	else
	{
		image_line = 0;
	}

	//移動処理
	Move();

	//ダメージ後無敵を測定する
	if (damage_flg && --damage_timer <= 0)
	{
		damage_flg = false;
	}

	//ダメージ後の移動不可状態を計測する
	if (damage_flg && damage_timer <= PLAYER_DAMAGE_CD - PLAYER_DAMAGE_STOP)
	{
		damage_stop = false;
	}

	//ダメージ後移動不可状態なら表示アニメーションを変える
	if (damage_stop)
	{
		image_line = 2;
	}

	//アニメーション処理
	Animation();

	//hpが0以下の時死亡処理
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

	//発射角度描画
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
	//ダメージ後無敵でないならダメージを受ける
	if (!damage_flg)
	{
		__super::Damage(_value, _attack_loc);
		damage_flg = true;
		damage_stop = true;
		//一定フレーム無敵
		damage_timer = PLAYER_DAMAGE_CD;
	}
}

void Player::Death()
{
	//ゲームオーバーに設定
	UserData::is_clear = false;
	//リザルト遷移
	manager->Result();

	//リスポーン位置に移動
	location = 0;

	//各値リセット
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
	//左スティックの傾きが一定以上なら移動する
	if (fabsf(InputPad::TipLStick(STICKL_X)) > 0.1f)
	{
		//加速の上限値を越さないように加算する
		if (fabsf(velocity.x) < PLAYER_SPEED)
		{
			velocity.x = InputPad::TipLStick(STICKL_X) * PLAYER_SPEED;
		}
	}
	if (fabsf(InputPad::TipLStick(STICKL_Y)) > 0.1f)
	{
		//加速の上限値を越さないように加算する
		if (fabsf(velocity.y) < PLAYER_SPEED)
		{
			velocity.y = -InputPad::TipLStick(STICKL_Y) * PLAYER_SPEED;
		}
	}

	//右スティックの傾きが一定以上なら角度を変更する
	if (fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f)
	{
		//右スティックの傾きで発射角度を決める (-1.5f = ずれ調整)
		shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.5f;
	}

	//コインが一枚以上なら、１消費で弾を発射する
	if (UserData::coin > 0 && (InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER) || InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER)))
	{
		manager->CreateAttack(GetBulletData());
		UserData::coin--;
	}

#ifdef _DEBUG
	//左キーで左移動
	if (k_input->GetKeyState(KEY_INPUT_LEFT) == eInputState::Held)
	{
		velocity.x = -10;
	}
	//右キーで右移動
	if (k_input->GetKeyState(KEY_INPUT_RIGHT) == eInputState::Held)
	{
		velocity.x = 10;
	}
	//上キーで上移動
	if (k_input->GetKeyState(KEY_INPUT_UP) == eInputState::Held)
	{
		velocity.y = -10;
	}
	//下キーで下移動
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
