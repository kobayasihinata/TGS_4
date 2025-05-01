#pragma once

#define BULLET_NUM 3	//弾の種類

//プレイヤーの弾に必要な情報
struct PlayerBullet
{
	int cost;			//発射にかかるコイン数
	float damage;		//ダメージ量
	float speed;		//弾速
	float radius;		//弾の半径
	int h_count;		//敵を何体貫通するか
	int life_span;		//弾の生存時間
};

//弾のデータ
static PlayerBullet pBullet[BULLET_NUM]
{
	//cos   dam  spe   rad   h_c  life
	{  1,	1.f, 10.f, 10.f,   1, 120},
	{ 10,   7.f, 15.f, 15.f,   1, 180},
	{100, 100.f, 20.f, 25.f, 100, 360}
};
