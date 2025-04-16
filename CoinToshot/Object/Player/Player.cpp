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

	//HP同期
	hp = UserData::player_hp;

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Tri-pilot/1.png");
	animation_image.push_back(tmp[0]);
	tmp = rm->GetImages("Resource/Images/Tri-pilot/2.png");
	animation_image.push_back(tmp[0]);
	image = animation_image[0];
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

	//各種入力処理
	Control();

	//移動処理
	Move();

	//アニメーション処理
	Animation();

	//hpが0以下の時死亡処理
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

	//発射角度描画
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
	//ダメージ後無敵でないならダメージを受ける
	if (!damage_flg)
	{
		__super::Damage(_value, _attack_loc);
		damage_flg = true;
		//120フレーム無敵
		damage_timer = 120;
	}
}

void Player::Death()
{
	//残機マイナス１
	UserData::life--;

	//残機が０以下になったらゲームオーバー
	if (UserData::life < 0)
	{
		//ゲームオーバーに設定
		UserData::is_clear = false;
		//リザルト遷移
		manager->Result();
	}

	//リスポーン位置に移動
	location = UserData::spawn_loc[UserData::now_stage];

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
		velocity.x = InputPad::TipLStick(STICKL_X) * PLAYER_SPEED;
	}
	if (fabsf(InputPad::TipLStick(STICKL_Y)) > 0.1f)
	{
		velocity.y = -InputPad::TipLStick(STICKL_Y) * PLAYER_SPEED;
	}

	//右スティックの傾きが一定以上なら角度を変更する
	if (fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f)
	{
		//右スティックの傾きで発射角度を決める (-1.5f = ずれ調整)
		shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.5f;
	}

	//トリガーで弾を発射する
	if (InputPad::OnButton(L_TRIGGER) || InputPad::OnButton(R_TRIGGER))
	{
		manager->CreateAttack(this->location, Vector2D{40, 40}, this, 30, shot_rad);
	}

	//角度を加算する
	//if (InputPad::OnPressed(XINPUT_BUTTON_A))
	//{
	//	shot_rad += 0.01f;
	//}
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

/// <summary>
/// 描画画像を取得
/// </summary>
/// <returns>画像ハンドル</returns>
int Player::GetImages()
{
	return image;
}
