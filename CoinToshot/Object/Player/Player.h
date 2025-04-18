#pragma once
#include <vector>
#include "../../Utility/Vector2D.h"
#include "../Base/ActorBase.h"
#include "../../Scene/Camera/Camera.h"
#include "../Base/BulletData.h"

class Player :
	public ActorBase
{
private:

	InputKey* k_input;			//入力機能の取得(キーボード)
	class Camera* camera;		//カメラポインタ格納(プレイヤーの情報を渡すためだけに取得)

	//コイン発射関連
	float shot_rad;				//発射角度

	//無敵処理関連
	bool inv_flg;				//無敵か判断
	int inv_timer;				//無敵時間測定
	bool damage_flg;			//被弾後無敵か判断
	int damage_timer;			//被弾後無敵時間測定
public:

public:
	Player() = default;					//コンストラクタ
	~Player() = default;					//デフォルトのデストラクタを使用

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="init_location">初期座標</param>
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = { PLAYER_WIDTH ,PLAYER_HEIGHT }, float init_radius = 0.f)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()const override;

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="hit_Object">調べる相手</param>
	void Hit(ObjectBase* hit_Object)override;

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="value">受けたダメージ量</param>
	void Damage(float _value, Vector2D _attack_loc)override;

	/// <summary>
	/// 死亡処理
	/// </summary>
	void Death();

	/// <summary>
	/// プレイヤー操作処理
	/// </summary>
	void Control();

	/// <summary>
	/// 弾を発射するための情報を纏める
	/// </summary>
	/// <returns>弾を発射するための情報</returns>
	BulletData GetBulletData();

public:

	/// <summary>
	/// 描画画像を取得
	/// </summary>
	/// <returns>画像ハンドル</returns>
	int GetImages();
};


