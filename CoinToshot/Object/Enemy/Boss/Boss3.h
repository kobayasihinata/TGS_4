#pragma once
#include "../../Base/BossBase.h"
#include "../../Base/BulletData.h"
#include "../../Player/PlayerBullet.h"

#define BOSS_POWERUP_ANIM 180	//登場アニメーション時間

#define MAX_LEVEL 2		//最大レベル

//弾のデータ
static PlayerBullet bBullet[BULLET_NUM]
{
	//name     cos   dam   kno  spe   rad   h_c  life  cd  num space color
	{"通常弾",   1,	 1.f,	1, 10.f, 20.f,   1,  120,  1,  1,    0,{100,100,100}},
	{"拡散弾",   5,  2.f,	1, 10.f, 20.f,   1,   30, 20,  5, 0.3f,{  0,100,100}},
	{"強化弾",  10,  3.f,	1, 15.f, 30.f,   5,  180, 10,  1,    0,{100, 60,  0}},
	{"追尾弾",  10,  1.f,	1,  4.f, 15.f,  13,  600, 30,  1,    0,{100,  0,100}},
	{"爆発弾",  30,  3.f,	1, 10.f, 10.f, 100,   20, 40,  1,	0,{100,  0,  0}},
	{"最強弾", 100, 20.f,	1, 25.f, 40.f, 100,  360,180,  1,    0,{255,255,255}},
};


struct BOSS3_BULLET
{
	int correct_coin;	//攻撃に移るのに必要なコイン枚数
	int attack_span;	//攻撃頻度
	int shot_num;		//攻撃回数
};
static BOSS3_BULLET boss3_bullet[BULLET_NUM]
{
	{10,  30,10},
	{30,  90, 5},
	{30,  70, 5},
	{60,  60, 3},
	{60, 180, 2},
	{100,180, 1},
};

class Boss3 :
	public BossBase
{
private:

	int coin_num;		//コイン所持数
	int move_mode;		//移動タイプ格納
	int collect_timer;	//コイン収集時間測定
	bool boss_stop_flg;	//歩くのをやめるか

	bool time_count_flg;	//生きている時間の測定をするかどうか
	int survival_time;		//ボスの生きている時間（レベルが上がったらリセット）
	int strength_level;		//ボスの強さ
	float boss_speed[3];	//強さごとのボスの速さ
	int player_distance;	//プレイヤーとの距離の基準値
	bool strength_up_anim;	//敵強化演出
	int strength_up_timer; //敵強化測定

	//攻撃発射関連
	int bullet_type;	//弾種類

	bool can_shot;		//弾を撃っていい状態か
	Vector2D target_loc;//攻撃対象の座標
	int shot_count;		//発射回数
	float shot_rad;		//発射角度
	bool shot_once;		//一回だけ撃つ
	float shot_speed;	//弾速度

	int shot_se;		//弾発射SE
	int walk_se;		//歩行SE

public:
	static InGameScene* ingame;		//現在のシーンのポインタを保存
	static bool boss3_once;		//プレイヤー生成を一回だけにする

private:
	//コンストラクタをprivateにすることで、
//自クラスのメンバ関数でインスタンスを生成できないようにする
	Boss3() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	Boss3(Boss3& v) = default;
	Boss3& operator = (const Boss3& v) = delete;
	~Boss3() = default;
public:

	//インスタンスを取得する処理
	static Boss3* Get(InGameScene* _ingame);

	//初期化処理
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//終了時処理
	void Finalize()override;
	//更新処理
	void Update()override;
	//描画処理
	void Draw()const override;
	//当たり判定が被った時の処理
	void Hit(ObjectBase* hit_Object)override;
	//ダメージ処理
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;

	//弾を撃つ関連の処理
	void Bullet();

	// 弾を発射するための情報をまとめる
	BulletData GetBulletData(float _shot_rad);

	//挙動切り替え
	void ChangeMove();

	//移動方向管理
	void MoveBoss3();

	//弾を撃つ
	void ShotBullet();

	///指定の座標の周りを回るように移動 _loc=軸 _distance=対象との距離 _direction=回転方向(false=左)
	double MoveAround(Vector2D _loc, int _distance, bool _direction = false);

	//強さの更新
	void UpdateStrength();

	//自身の強さを変更する
	void SetStrength(int _level);
};

