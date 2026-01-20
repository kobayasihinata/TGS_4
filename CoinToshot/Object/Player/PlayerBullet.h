#pragma once

#define BULLET_NUM 6	//弾の種類

//プレイヤーの弾に必要な情報
struct PlayerBullet
{
	char name[10];		//弾の名称
	int cost;			//発射にかかるコイン数
	float damage;		//ダメージ量
	int knockback;		//ノックバック強度
	float speed;		//弾速
	float radius;		//弾の半径
	int h_count;		//敵を何体貫通するか
	int life_span;		//弾の生存時間
	int cooldown;		//弾の発射間隔

	int bullet_num;		//弾をいくつ発射するか
	float space;		//どれくらいの間隔を開けて撃つか
	int color[3];			//色
};

//弾のデータ
static PlayerBullet pBullet[BULLET_NUM]
{
	//name     cos   dam  knock spe   rad   h_c  life  cd  num space color
	{"通常弾",   1,	 1.f,   10	,10.f, 20.f,   1,  120,  2,  1,    0,{255,255,255}},
	{"拡散弾",   5,  2.f,   10	,10.f, 20.f,   1,   30, 20,  5, 0.3f,{0,255,255}},
	{"強化弾",  10,  7.f,   15	,13.f, 30.f,   5,  180, 10,  1,    0,{255,127,0}},
	{"追尾弾",  10,  1.f,    7	, 7.f, 15.f,  13, 1200, 30,  1,    0,{255,0,255}},
	{"爆発弾",  30,  1.f,   20	,10.f, 10.f, 100,   20, 40,  1,    0,{255,0,0}},
	{"最強弾",	 1,  1.f,    0	,40.f, 20.f,   1,  100,  0 , 1,    0,{0,0,0}},
};

//データと同じ順番
static enum BulletType {
	bNormal=0,
	bShotgun,
	bStrong,
	bTracking,
	bExplosion,
	bStrongest,
};