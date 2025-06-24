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

InGameScene* Player::ingame;		//現在のシーンのポインタを保存

Player* Player::Get(InGameScene* _ingame)
{
	ingame = _ingame;
	static Player instance;
	//自分自身のポインタを返却する
	return &instance;
}

void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);

	camera = Camera::Get();
	k_input = InputKey::Get();
	tutorial = Tutorial::Get();

	bullet_change_cd = 0;
	danger_once = false;
	arrow_image = MakeScreen(100, 100, TRUE);
	aim_once_flg = false;
	shot_rad = 123456.f;	//ありえない値を入れておく
	old_shot_rad = 0.f;
	
	inv_flg = 0;				
	inv_timer = 0;		
	damage_flg = false;
	damage_timer = 0;
	damage_stop = false;

	//死亡処理に使う
	drop_coin = 100;
	death_timer = DEFAULT_DEATH_TIMER * 3;

	//HP同期
	max_hp = hp = UserData::player_hp;

	//画像生成
	CreateArrowImage();
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

	//SE読み込み
	shot_se = rm->GetSounds("Resource/Sounds/shot.mp3");
	walk_se = rm->GetSounds("Resource/Sounds/Player/Walk.mp3");
	cursor_se = rm->GetSounds("Resource/Sounds/cursor.mp3");
	not_shoot_se = rm->GetSounds("Resource/Sounds/Player/CannotShoot.mp3");
	death_se = rm->GetSounds("Resource/Sounds/explsion_big.mp3");
	bullet_change_se = rm->GetSounds("Resource/Sounds/メッセージ表示音2.mp3");
	danger_se = rm->GetSounds("Resource/Sounds/Direction/警告音2.mp3");

	//音量調節
	SetVolumeSoundMem(9500, walk_se);
	//PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);

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
	hp = UserData::player_hp;
	//移動可能か同期
	UserData::player_damage_flg = damage_stop;
	__super::Update();

	//吸い寄せ処理の時間測定
	if (--UserData::attraction_timer < 0)
	{
		UserData::attraction_timer = 0;
	}
	//吸い寄せの時間が１以上ならフラグを立てる
	UserData::attraction_flg = (UserData::attraction_timer > 0) ? true : false;

	//コインが100を超えた時、弾種類変更可能フラグを立て、弾変更チュートリアルをリクエスト
	if (!UserData::can_bullet_change_flg && UserData::coin >= 50)
	{
		UserData::can_bullet_change_flg = true;
		tutorial->StartTutoRequest(TutoType::tBulletChange);
	}
	
	//カメラに座標を渡す
	camera->player_location = this->location;

	//ダメージ後の移動不可状態なら操作を受け付けない 死んでても受け付けない
	if (!damage_stop && !death_flg)
	{
		//各種入力処理
		Control();
	}

	//弾種類変更クールダウン測定
	if (bullet_change_cd > 0)
	{
		bullet_change_cd--;
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

	//チュートリアルが終わっていないなら、カメラの外に出ることはできない
	if (!tutorial->GetBasicTuto())
	{
		//カメラの外に居たら前の座標に戻す
		//左端
		if (location.x - (box_size.x / 2) <= camera->GetCameraLocation().x)
		{
			location.x = camera->GetCameraLocation().x + (box_size.x / 2);
		}
		//右端
		if (location.x + (box_size.x / 2) > camera->GetCameraLocation().x + SCREEN_WIDTH)
		{
			location.x = camera->GetCameraLocation().x + SCREEN_WIDTH - (box_size.x / 2);
		}
		//上端
		if (location.y - (box_size.y / 2) <= camera->GetCameraLocation().y)
		{
			location.y = camera->GetCameraLocation().y + (box_size.y / 2);
		}
		//下端
		if (location.y + (box_size.y / 2) > camera->GetCameraLocation().y + SCREEN_HEIGHT)
		{
			location.y = camera->GetCameraLocation().y + SCREEN_HEIGHT - (box_size.y / 2);
		}
	}

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
	CreateArrowImage();

	//アニメーション位置に応じて足音を鳴らす
	if (image_line == 1 &&
		(image_num == 4 || image_num == 10)&&
		!CheckSoundMem(walk_se))
	{
		PlaySoundMem(walk_se, DX_PLAYTYPE_BACK);
		manager->CreateEffect(elWalk, { location.x,location.y+(box_size.y/2) }, FALSE, 0xffffff, velocity.x > 0 ? FALSE : TRUE, 30);
	}

	//hpが0以下の時死亡処理
	if (hp <= 0)
	{
		death_flg = true;
	}

	//HPが3以下になったら一回だけSE
	if (hp <= 3)
	{
		if (!danger_once)
		{
			PlaySoundMem(danger_se, DX_PLAYTYPE_BACK);
			danger_once = true;
		}
	}
	else
	{
		danger_once = false;
	}

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡中
		UserData::is_dead = true;
		//吸い寄せ強制終了
		UserData::attraction_timer = 0;
		death_timer--;
		//死にながらコインをまき散らす
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
			//死亡SE再生
			PlaySoundMem(death_se, DX_PLAYTYPE_BACK);
			//爆発エフェクト
			manager->CreateEffect(elExplosion, { location.x + (GetRand(100) - 50),location.y + (GetRand(100) - 50) });
		}

		//死亡演出時間を過ぎたら自身を削除
		if (death_timer == DEFAULT_DEATH_TIMER * 2)
		{
			//演出中に出せなかったコインをまとめてドロップ
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
			//死亡SE再生
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
	//フォントサイズ保存
	int old = GetFontSize();

	//吸い寄せエフェクト
	if (UserData::attraction_flg)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawCircleAA(local_location.x, local_location.y, -frame * i % 60, 30, 0x00ffff, false);
		}
	}

	//カーソルを動かせる状態なら表示
	if (aim_once_flg)
	{
		//弾の軌道描画
		DrawBulletOrbit();
	}

	if ((!damage_flg || (damage_flg && frame % 3 == 0)) && (!death_flg || death_timer > DEFAULT_DEATH_TIMER * 2))
	{
		__super::Draw();
	}

	if (hp > 0)
	{
		//HPゲージ内側
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0x000000,
			true
		);
		//HPゲージ本体
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 9,
			local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)) + hpbar_move,
			local_location.y - (box_size.y / 2) - 1,
			hp > 3 ? 0x22ff00 : frame % (int)(10 * hp) > (10 * hp) / 2 ? 0xff1100 : 0xffcc00,
			true
		);
		//HPゲージ減少アニメーション
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)),
			local_location.y - (box_size.y / 2) - 9,
			local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)) + hpbar_move,
			local_location.y - (box_size.y / 2) - 1,
			0xff0000,
			true
		);
		//HPゲージ外枠
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0xffffff,
			false
		);
		//HPゲージ限界突破見た目
		if (UserData::player_hp > DEFAULT_HP)
		{
			DrawBoxAA(local_location.x + (HPBAR_SIZE / 2) - 2,
				local_location.y - (box_size.y / 2) - 9,
				local_location.x + (HPBAR_SIZE / 2) + 2,
				local_location.y - (box_size.y / 2) - 1,
				0x22ff00,
				true
			);
			DrawLine(local_location.x + (HPBAR_SIZE / 2),
				local_location.y - (box_size.y / 2) - 10,
				local_location.x + (HPBAR_SIZE / 2) + 5,
				local_location.y - (box_size.y / 2) - 15,
				0xffffff);
			DrawLine(local_location.x + (HPBAR_SIZE / 2),
				local_location.y - (box_size.y / 2),
				local_location.x + (HPBAR_SIZE / 2) + 5,
				local_location.y - (box_size.y / 2) + 5,
				0xffffff);
		}
		//HPピンチ表示
		if (hp <= 3)
		{
			DrawTriangleAA(local_location.x, local_location.y - 80,
				local_location.x + 30, local_location.y - 30,
				local_location.x - 30, local_location.y - 30,
				frame % (int)(10 * hp) > (10 * hp) / 2 ? 0xff1100 : 0xffcc00,
				false);
			SetFontSize(40);
			DrawStringF(local_location.x-7, local_location.y - 75, "!", frame % (int)(10 * hp) > (10 * hp) / 2 ? 0xff1100 : 0xffcc00);
		}
	}

	//フォント大きさ元通り
	SetFontSize(old);
}

void Player::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Player::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	//ダメージ後無敵でないならダメージを受ける
	if (!damage_flg && !death_flg)
	{
		__super::Damage(_value, _attack_loc, _knock_back);
		UserData::player_hp = hp;
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
	manager->Result(240);

	//リスポーン位置に移動
	location = 0;

	//各値リセット
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
			//移動したい方向保存
			move_velocity.x = velocity.x;
		}
	}
	if (fabsf(InputPad::TipLStick(STICKL_Y)) > 0.1f)
	{
		//加速の上限値を越さないように加算する
		if (fabsf(velocity.y) < PLAYER_SPEED)
		{
			velocity.y = -InputPad::TipLStick(STICKL_Y) * PLAYER_SPEED;
			//移動したい方向保存
			move_velocity.y = velocity.y;
		}
	}

	//照準チュートリアルが完了している　もしくは照準チュートリアル中なら右スティックの傾きが一定以上なら角度を変更する
	if (tutorial->GetTutoNowEnd(TutoType::tAim)&&
		(fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f))
	{
		//カーソルを動かした事を格納する
		if (!aim_once_flg)aim_once_flg = true;
		//前の傾きを保存する
		old_shot_rad = shot_rad;
		//右スティックの傾きで発射角度を決める (-1.55f = ずれ調整)
		tutorial->player_aim_rad = shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.55f;
		
		//角度が一定以上変更されていたらSEを鳴らす
		if (fabsf(old_shot_rad - shot_rad) > 0.01f && frame % 4==0)
		{
			PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
		}
	}

	//攻撃チュートリアルが完了している　もしくは攻撃チュートリアル中ならコイン消費で弾を発射する
	if (tutorial->GetTutoNowEnd(TutoType::tAttack)&&
		(InputPad::OnButton(XINPUT_BUTTON_LEFT_SHOULDER) || InputPad::OnButton(XINPUT_BUTTON_RIGHT_SHOULDER)))
	{
		if (UserData::coin >= pBullet[UserData::bullet_type].cost)
		{
			ShotBullet();
			UserData::coin -= pBullet[UserData::bullet_type].cost;
			PlaySoundMem(shot_se, DX_PLAYTYPE_BACK);
			std::string s = "-" + std::to_string(pBullet[UserData::bullet_type].cost);
			ingame->CreatePopUp(this->location, s, 0xff0000, -1);
		}
		//発射失敗SE
		else
		{
			PlaySoundMem(not_shoot_se, DX_PLAYTYPE_BACK);
		}
	}

	//弾種類変更が可能＆クールダウンが終わっていたら弾を変更出来る
	if (UserData::can_bullet_change_flg && bullet_change_cd <= 0)
	{
		//トリガーで弾の種類を変える
		if (InputPad::OnPressed(L_TRIGGER))
		{
			if (--UserData::bullet_type < 0)
			{
				UserData::bullet_type = BULLET_NUM - 1;
			}
			bullet_change_cd = PLATER_BULLET_CHANGE_CD;
			if (!CheckSoundMem(bullet_change_se))
			{
				PlaySoundMem(bullet_change_se, DX_PLAYTYPE_BACK);
			}
		}
		else if (InputPad::OnPressed(R_TRIGGER))
		{
			if (++UserData::bullet_type > BULLET_NUM - 1)
			{
				UserData::bullet_type = 0;
			}
			bullet_change_cd = PLATER_BULLET_CHANGE_CD;
			if (!CheckSoundMem(bullet_change_se))
			{
				PlaySoundMem(bullet_change_se, DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			if (CheckSoundMem(bullet_change_se))
			{
				StopSoundMem(bullet_change_se);
			}
		}
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
	for (int i = 0; i < 3; i++)
	{
		_data.color[i] = pBullet[UserData::bullet_type].color[i];
	}
	return _data;
}

void Player::ShotBullet()
{
	//一発以上撃つ弾種なら
	if (pBullet[UserData::bullet_type].bullet_num > 1)
	{
		float t = (pBullet[UserData::bullet_type].space * pBullet[UserData::bullet_type].bullet_num) / 2;
		for (int i = 0; i < pBullet[UserData::bullet_type].bullet_num; i++)
		{
			manager->CreateAttack(GetBulletData(shot_rad - t + i * pBullet[UserData::bullet_type].space));
		}
	}
	//単発なら
	else
	{
		manager->CreateAttack(GetBulletData(shot_rad));
	}
}

void Player::DrawBulletOrbit()const
{
	DrawRotaGraph(local_location.x+40*sinf(shot_rad+1.5f), local_location.y-40*cosf(shot_rad+ 1.5f), 1.0f, shot_rad + 1.55f, arrow_image, TRUE);
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
	//文字透過リセット
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetDrawScreen(DX_SCREEN_BACK);
}