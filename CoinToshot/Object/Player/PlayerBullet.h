#pragma once

#define BULLET_NUM 5	//弾の種類

//プレイヤーの弾に必要な情報
struct PlayerBullet
{
	char name[10];		//弾の名称
	int cost;			//発射にかかるコイン数
	float damage;		//ダメージ量
	float speed;		//弾速
	float radius;		//弾の半径
	int h_count;		//敵を何体貫通するか
	int life_span;		//弾の生存時間

	int bullet_num;		//弾をいくつ発射するか
	float space;		//どれくらいの間隔を開けて撃つか
};

//弾のデータ
static PlayerBullet pBullet[BULLET_NUM]
{
	//name    cos   dam  spe   rad   h_c  life num space
	{"通常弾",  1,	 1.f, 10.f, 20.f,   1, 120,1,    0},
	{"拡散弾",  5,   2.f, 10.f, 20.f,   1,  30,5, 0.2f},
	{"強化弾", 10,   7.f, 15.f, 30.f,   5, 180,1,    0},
	{"爆発弾", 10,   3.f, 10.f, 30.f,  10,  20,1,	 0},
	{"最強弾", 50,  50.f, 25.f, 40.f, 100, 360,1,    0},
};

//データと同じ順番
static enum BulletType {
	bNormal=0,
	bShotgun,
	bStrong,
	bExplosion,
	bStrongest,

};