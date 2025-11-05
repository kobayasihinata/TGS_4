#include "Player.h"
#include "DxLib.h"
#include "../../Utility/UserData.h"
#include "../ObjectManager.h"
#include "../../Utility/InputPad.h"
#include "../../Utility/common.h"
#include "PlayerBullet.h"
#include "../Base/EffectList.h"
#include "../../Scene/InGameScene.h"
#include "Color.h"

#include <cmath>
#define _USE_MATH_DEFINES

InGameScene* Player::ingame;		//現在のシーンのポインタを保存
bool Player::player_once;

Player* Player::Get(InGameScene* _ingame)
{
	if (!player_once)
	{
		player_once = true;
		ingame = _ingame;
		static Player instance;
		//自分自身のポインタを返却する
		return &instance;
	}
	else
	{
		return nullptr;
	}
}

void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);

	camera = Camera::Get();
	k_input = InputKey::Get();
	tutorial = Tutorial::Get();

	bullet_change_cd = 0;
	danger_once = false;
	change_flg = false;
	change_timer = 0;
	player_image = MakeScreen(PLAYER_IMAGE_SIZE, PLAYER_IMAGE_SIZE, TRUE);
	arrow_image = MakeScreen(100, 110, TRUE);
	flash_time = 0;
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

	flash_image;

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
}

/// <summary>
/// 終了処理
/// </summary>
void Player::Finalize()
{
	player_once = false;
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

	//チュートリアルを終えた時、弾種類変更可能フラグを立て、弾変更チュートリアルをリクエスト
	if (!UserData::can_bullet_change_flg && tutorial->GetBasicTuto())
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

	//弾変更アニメーション用処理
	if (change_flg)
	{
		if (++change_timer > 20)
		{
			change_timer = 0;
			change_flg = false;
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

	//マズルフラッシュ用
	if (flash_time > 0)
	{
		flash_time--;
	}

	//アニメーション位置に応じて足音を鳴らす
	if (image_line == 1 &&
		(image_num == 4 || image_num == 10)&&
		!CheckSoundMem(walk_se))
	{
		ResourceManager::rPlaySound(walk_se, DX_PLAYTYPE_BACK);
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
			ResourceManager::rPlaySound(danger_se, DX_PLAYTYPE_BACK);
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
			ResourceManager::rPlaySound(death_se, DX_PLAYTYPE_BACK);
			//爆発エフェクト
			manager->CreateEffect(elExplosion, { location.x + (GetRand(100) - 50),location.y + (GetRand(100) - 50) },{0,0});
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
			ResourceManager::rPlaySound(death_se, DX_PLAYTYPE_BACK);
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

	//吸い寄せエフェクト
	if (UserData::attraction_flg)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawCircleAA(local_location.x, local_location.y, -frame * i % 60, 30, 0x00ffff, false);
		}
	}

	//マズルフラッシュ
	if (flash_time > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		//
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	//カーソルを動かせる状態なら表示
	if (aim_once_flg && !death_flg)
	{
		//弾の軌道描画
		DrawBulletOrbit(local_location);
	}

	if ((!damage_flg || (damage_flg && frame % 3 == 0)) && (!death_flg || death_timer > DEFAULT_DEATH_TIMER * 2))
	{
		__super::Draw();
	}

	Vector2D bar_loc = { local_location.x - (HPBAR_WIDTH / 2),local_location.y - (box_size.y/2) - (HPBAR_HEIGHT / 2) };
	if (hp > 0)
	{
		//10HP毎にバーの段を変える
		for (int i = 0; i <= (hp-1) / 10; i++)
		{
			float shift = HPBAR_HEIGHT * ((hp-1) / 10) - (HPBAR_HEIGHT * i);
			int line_hp = hp - (max_hp * i) > max_hp ? max_hp : hp - (max_hp * i);
			//HPゲージ内側
			DrawBoxAA(bar_loc.x,
				bar_loc.y - shift,
				bar_loc.x + HPBAR_WIDTH,
				bar_loc.y + HPBAR_HEIGHT -shift,
				0x000000,
				true
			);
			//HPゲージ本体
			DrawBoxAA(bar_loc.x,
				bar_loc.y + 1 - shift,
				bar_loc.x + (line_hp * (HPBAR_WIDTH / max_hp)) + hpbar_move,
				bar_loc.y + HPBAR_HEIGHT - 1 - shift,
				hp > 3 ? 0x22ff00 : frame % (int)(10 * hp) > (10 * hp) / 2 ? 0xff1100 : 0xffcc00,
				true
			);
			if (i == (int)((hp - 1) / 10))
			{
				//HPゲージ減少アニメーション
				DrawBoxAA(bar_loc.x + (line_hp * (HPBAR_WIDTH / max_hp)),
					bar_loc.y + 1 - shift,
					bar_loc.x + (line_hp * (HPBAR_WIDTH / max_hp)) + hpbar_move,
					bar_loc.y + HPBAR_HEIGHT - 1 - shift,
					0xff0000,
					true
				);
			}
			//HPゲージ外枠
			DrawBoxAA(bar_loc.x,
				bar_loc.y - shift,
				bar_loc.x + HPBAR_WIDTH,
				bar_loc.y + HPBAR_HEIGHT - shift,
				0xffffff,
				false
			);
			////HPゲージ限界突破見た目
			//if (UserData::player_hp > DEFAULT_HP)
			//{
			//	for (int i = 0; i < UserData::player_hp - DEFAULT_HP; i++)
			//	{
			//		DrawBoxAA(local_location.x + (HPBAR_WIDTH / 2) + (i * 5),
			//			local_location.y - (box_size.y / 2) - 9,
			//			local_location.x + (HPBAR_WIDTH / 2) + 5 + (i * 5),
			//			local_location.y - (box_size.y / 2) - 1,
			//			color[i % 7], true);
			//	}
			//}

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
			DrawStringF(local_location.x - 7, local_location.y - 75, "!", frame % (int)(10 * hp) > (10 * hp) / 2 ? 0xff1100 : 0xffcc00);
		}
	}

	if (change_flg)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - change_timer * (255 / 20));
		DrawRotaGraphF(local_location.x, local_location.y, 1.f + (float)(change_timer/10.f), 0, player_image, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
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
		//カメラ振動
		camera->impact = 20;
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
	//キーマウ操作なら処理しない
	if (UserData::control_type != 2)
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
		if (tutorial->GetTutoNowEnd(TutoType::tAim) &&
			(fabsf(InputPad::TipRStick(STICKL_X)) > 0.3f || fabsf(InputPad::TipRStick(STICKL_Y)) > 0.3f))
		{
			//カーソルを動かした事を格納する
			if (!aim_once_flg)aim_once_flg = true;
			//前の傾きを保存する
			old_shot_rad = shot_rad;
			//右スティックの傾きで発射角度を決める (-1.55f = ずれ調整)
			tutorial->player_aim_rad = shot_rad = atan2f(InputPad::TipRStick(STICKL_X), InputPad::TipRStick(STICKL_Y)) - 1.55f;

			//角度が一定以上変更されていたらSEを鳴らす
			if (fabsf(old_shot_rad - shot_rad) > 0.01f && frame % 4 == 0)
			{
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
		}
	}
	//キーマウ操作なら実行する
	else
	{
		//照準チュートリアルが完了している　もしくは照準チュートリアル中ならマウスカーソルに応じて照準角度を変更する
		if (tutorial->GetTutoNowEnd(TutoType::tAim) &&
			(k_input->GetMouseState(MOUSE_INPUT_RIGHT) == eInputState::Held))
		{
			//カーソルを動かした事を格納する
			if (!aim_once_flg)aim_once_flg = true;
			//前の傾きを保存する
			old_shot_rad = shot_rad;
			//右スティックの傾きで発射角度を決める (-1.55f = ずれ調整)
			tutorial->player_aim_rad = shot_rad = atan2f(k_input->GetMouseCursor().x - this->local_location.x, (k_input->GetMouseCursor().y - this->local_location.y) * -1) - 1.55f;

			//角度が一定以上変更されていたらSEを鳴らす
			if (fabsf(old_shot_rad - shot_rad) > 0.01f && frame % 4 == 0)
			{
				ResourceManager::rPlaySound(cursor_se, DX_PLAYTYPE_BACK);
			}
		}
	}

	//十字ボタン、WASDでの移動
	if (UserData::CheckPlayerMove(UP))
	{
		velocity.y = -PLAYER_SPEED;
		//移動したい方向保存
		move_velocity.y = velocity.y;
	}
	else if (UserData::CheckPlayerMove(DOWN))
	{
		velocity.y = PLAYER_SPEED;
		//移動したい方向保存
		move_velocity.y = velocity.y;
	}
	if (UserData::CheckPlayerMove(RIGHT))
	{
		velocity.x = PLAYER_SPEED;
		//移動したい方向保存
		move_velocity.x = velocity.x;
	}
	else if (UserData::CheckPlayerMove(LEFT))
	{
		velocity.x = -PLAYER_SPEED;
		//移動したい方向保存
		move_velocity.x = velocity.x;
	}


	//攻撃チュートリアルが完了している　もしくは攻撃チュートリアル中ならコイン消費で弾を発射する

	if (tutorial->GetTutoNowEnd(TutoType::tAttack) &&
		UserData::CheckBulletButton())
	{
		if (UserData::get_bullet_cd[UserData::now_bullet] <= 0 && UserData::coin >= pBullet[UserData::get_bullet[UserData::now_bullet]].cost)
		{
			ShotBullet();
			UserData::get_bullet_cd[UserData::now_bullet] = pBullet[UserData::get_bullet[UserData::now_bullet]].cooldown;
			UserData::coin -= pBullet[UserData::get_bullet[UserData::now_bullet]].cost;
			ResourceManager::rPlaySound(shot_se, DX_PLAYTYPE_BACK);
			std::string s = "-" + std::to_string(pBullet[UserData::get_bullet[UserData::now_bullet]].cost);
			ingame->CreatePopUp(this->location, s, 0xff0000, -1);
			flash_time = MUZZLE_FLAST_TIME;
		}
		//発射失敗SE
		else
		{
			ResourceManager::rPlaySound(not_shoot_se, DX_PLAYTYPE_BACK);
		}
	}

	//弾種類変更が可能＆クールダウンが終わっていたら弾を変更出来る
	if (UserData::can_bullet_change_flg && bullet_change_cd <= 0)
	{
		//弾の種類を変える
		if (UserData::CheckBulletChangeButtonLeft())
		{
			CreatePlayerImage();
			if (--UserData::now_bullet < 0)
			{
				UserData::now_bullet = UserData::get_bullet.size() - 1;
			}
			bullet_change_cd = PLATER_BULLET_CHANGE_CD;
			if (!CheckSoundMem(bullet_change_se))
			{
				ResourceManager::rPlaySound(bullet_change_se, DX_PLAYTYPE_BACK);
			}
			change_flg = true;
		}
		else if (UserData::CheckBulletChangeButtonRight())
		{
			CreatePlayerImage();

			if (++UserData::now_bullet > UserData::get_bullet.size()-1)
			{
				UserData::now_bullet = 0;
			}
			bullet_change_cd = PLATER_BULLET_CHANGE_CD;
			if (!CheckSoundMem(bullet_change_se))
			{
				ResourceManager::rPlaySound(bullet_change_se, DX_PLAYTYPE_BACK);
			}
			change_flg = true;
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

#endif // _DEBUG
}

BulletData Player::GetBulletData(float _shot_rad)
{
	BulletData _data;
	_data.damage = pBullet[UserData::get_bullet[UserData::now_bullet]].damage;
	_data.b_angle = _shot_rad;
	_data.delete_time = pBullet[UserData::get_bullet[UserData::now_bullet]].life_span;
	_data.h_count = pBullet[UserData::get_bullet[UserData::now_bullet]].h_count;
	_data.location = this->location;
	_data.radius = pBullet[UserData::get_bullet[UserData::now_bullet]].radius;
	_data.speed = pBullet[UserData::get_bullet[UserData::now_bullet]].speed;
	_data.who = this;
	_data.b_type = (BulletType)UserData::get_bullet[UserData::now_bullet];
	for (int i = 0; i < 3; i++)
	{
		_data.color[i] = pBullet[UserData::get_bullet[UserData::now_bullet]].color[i];
	}
	return _data;
}

void Player::ShotBullet()
{
	//一発以上撃つ弾種なら
	if (pBullet[UserData::get_bullet[UserData::now_bullet]].bullet_num > 1)
	{
		float t = (pBullet[UserData::get_bullet[UserData::now_bullet]].space * pBullet[UserData::get_bullet[UserData::now_bullet]].bullet_num) / 2;
		for (int i = 0; i < pBullet[UserData::get_bullet[UserData::now_bullet]].bullet_num; i++)
		{
			manager->CreateAttack(GetBulletData(shot_rad - t + i * pBullet[UserData::get_bullet[UserData::now_bullet]].space));
		}
	}
	//単発なら
	else
	{
		manager->CreateAttack(GetBulletData(shot_rad));
	}
}

void Player::DrawBulletOrbit(Vector2D _loc)const
{
	float span,start;
	switch (UserData::get_bullet[UserData::now_bullet])
	{
		//普通タイプの矢印
	case BulletType::bNormal:
	case BulletType::bStrong:
	case BulletType::bStrongest:
	case BulletType::bTracking:
		DrawRotaGraph(_loc.x + 40 * sinf(shot_rad + 1.5f), _loc.y - 40 * cosf(shot_rad + 1.5f), 1.0f, shot_rad + 1.55f, arrow_image, TRUE);

		break;
		//拡散弾の矢印
	case BulletType::bShotgun:
		span = (pBullet[UserData::get_bullet[UserData::now_bullet]].space * pBullet[UserData::get_bullet[UserData::now_bullet]].bullet_num);
		start = shot_rad + 1.55f - (span / 2);
		for (float i = start; i <= start + span; i += pBullet[UserData::get_bullet[UserData::now_bullet]].space)
		{
			DrawRotaGraph(_loc.x + 40 * sinf(i), _loc.y - 40 * cosf(i), 1.0f, i, arrow_image, TRUE);
		}
		break;

	case BulletType::bExplosion:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - (frame * 5) % 200);
		DrawRotaGraph(_loc.x + 40 * sinf(shot_rad + 1.5f), _loc.y - 40 * cosf(shot_rad + 1.5f), 1.0f, shot_rad + 1.55f, arrow_image, TRUE);
		DrawCircleAA(_loc.x + (pBullet[UserData::get_bullet[UserData::now_bullet]].speed*pBullet[UserData::get_bullet[UserData::now_bullet]].life_span) * sinf(shot_rad + 1.55f),
			_loc.y - (pBullet[UserData::get_bullet[UserData::now_bullet]].speed * pBullet[UserData::get_bullet[UserData::now_bullet]].life_span) * cosf(shot_rad + 1.55f),
			(frame*5) % 200, 100, 0xff0000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	default:
		break;
	}
}

void Player::CreateArrowImage()const
{
	SetDrawScreen(arrow_image);
	ClearDrawScreen();
	//現在の弾の種類に応じて見た目を変更
	switch (UserData::get_bullet[UserData::now_bullet])
	{
	case BulletType::bNormal:
		for (int i = 1; i < 5; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20) * 10));
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
					    35, 25 + (i * 20) - (frame % 20),
					    40, 30 + (i * 20) - (frame % 20),
					    50, 10 + (i * 20) - (frame % 20),
					    0xffffff, TRUE);				
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
						65, 25 + (i * 20) - (frame % 20),
						60, 30 + (i * 20) - (frame % 20),
						50, 10 + (i * 20) - (frame % 20),
						0xffffff, TRUE);
		}
		//文字透過リセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case BulletType::bShotgun:
		for (int i = 1; i < 4; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20) * 10));
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
						40, 25 + (i * 20) - (frame % 20),
						45, 30 + (i * 20) - (frame % 20),
						50, 10 + (i * 20) - (frame % 20),
						0x00ffff, TRUE);
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
						60, 25 + (i * 20) - (frame % 20),
						55, 30 + (i * 20) - (frame % 20),
						50, 10 + (i * 20) - (frame % 20),
						0x00ffff, TRUE);
		}
		//文字透過リセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case BulletType::bStrong:
		for (int i = 1; i < 5; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20) * 10));
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
						25, 25 + (i * 20) - (frame % 20),
						30, 30 + (i * 20) - (frame % 20),
						50, 10 + (i * 20) - (frame % 20),
						0xffaa00, TRUE);			
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
						75, 25 + (i * 20) - (frame % 20),
						70, 30 + (i * 20) - (frame % 20),
						50, 10 + (i * 20) - (frame % 20),
						0xffaa00, TRUE);
		}
		//文字透過リセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case BulletType::bTracking:
		for (int i = 1; i < 5; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20) * 10));
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
				35, 25 + (i * 20) - (frame % 20),
				30, 30 + (i * 20) - (frame % 20),
				50, 10 + (i * 20) - (frame % 20),
				0xff00ff, TRUE);
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
				65, 25 + (i * 20) - (frame % 20),
				60, 30 + (i * 20) - (frame % 20),
				50, 10 + (i * 20) - (frame % 20),
				0xff00ff, TRUE);
		}
		//文字透過リセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case BulletType::bExplosion:
		for (int i = 1; i < 5; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20) * 10));
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
				40, 25 + (i * 20) - (frame % 20),
				45, 30 + (i * 20) - (frame % 20),
				50, 10 + (i * 20) - (frame % 20),
				0xff0000, TRUE);
			DrawQuadrangleAA(50, (i * 20) - (frame % 20),
				60, 25 + (i * 20) - (frame % 20),
				55, 30 + (i * 20) - (frame % 20),
				50, 10 + (i * 20) - (frame % 20),
				0xff0000, TRUE);
		}
		//文字透過リセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	case BulletType::bStrongest:
		for (int i = 1; i < 5; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (i * 200) - ((frame % 20) * 10));
			DrawQuadrangleAA(50, (i * 20) - (frame % 20)*2,
						15, 25 + (i * 20) - (frame % 20)*2,
						20, 30 + (i * 20) - (frame % 20)*2,
						50, 10 + (i * 20) - (frame % 20)*2,
						0x000000, TRUE);			 	 
			DrawQuadrangleAA(50, (i * 20) - (frame % 20)*2,
						85, 25 + (i * 20) - (frame % 20)*2,
						80, 30 + (i * 20) - (frame % 20)*2,
						50, 10 + (i * 20) - (frame % 20)*2,
						0x000000, TRUE);
		}
		//文字透過リセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		break;
	default:
		break;
	}

	//文字透過リセット
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	SetDrawScreen(DX_SCREEN_BACK);
}

void Player::CreatePlayerImage()const
{
	//プレイヤー画像保存
	SetDrawScreen(player_image);
	ClearDrawScreen();
	//カーソルを動かせる状態なら表示
	if (aim_once_flg)
	{
		//弾の軌道描画
		DrawBulletOrbit(PLAYER_IMAGE_SIZE / 2);
	}

	if ((!damage_flg || (damage_flg && frame % 3 == 0)) && (!death_flg || death_timer > DEFAULT_DEATH_TIMER * 2))
	{
		//画像描画
		if (image != 0)
		{
			if (move_velocity.x > 0)
			{
				DrawRotaGraphF(PLAYER_IMAGE_SIZE/2, PLAYER_IMAGE_SIZE / 2, 1, 0, image, true, false);
			}
			else
			{
				DrawRotaGraphF(PLAYER_IMAGE_SIZE / 2, PLAYER_IMAGE_SIZE / 2, 1, 0, image, true, true);
			}
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);
}