#include "Player.h"

#include <cmath>

#include "DxLib.h"
#include "../../Utility/UserData.h"
#include "../ObjectManager.h"

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="init_location">初期座標</param>
void Player::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);

	camera = Camera::Get();

	g_velocity = 0.0f;
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

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	//HP同期
	UserData::player_hp = hp;

	__super::Update();

	//カメラに座標を渡す
	camera->player_location = this->location;

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

/// <summary>
/// 描画処理
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
/// 当たり判定処理
/// </summary>
/// <param name="hit_Object">調べる相手</param>
void Player::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);

}

/// <summary>
/// ダメージ処理
/// </summary>
/// <param name="value">受けたダメージ量</param>
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

/// <summary>
/// 死亡処理
/// </summary>
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

/// <summary>
/// 描画画像を取得
/// </summary>
/// <returns>画像ハンドル</returns>
int Player::GetImages()
{
	return image;
}
