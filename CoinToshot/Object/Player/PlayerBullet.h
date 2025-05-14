#pragma once

#define BULLET_NUM 3	//’e‚Ìí—Ş

//ƒvƒŒƒCƒ„[‚Ì’e‚É•K—v‚Èî•ñ
struct PlayerBullet
{
	char name[10];			//’e‚Ì–¼Ì
	int cost;			//”­Ë‚É‚©‚©‚éƒRƒCƒ“”
	float damage;		//ƒ_ƒ[ƒW—Ê
	float speed;		//’e‘¬
	float radius;		//’e‚Ì”¼Œa
	int h_count;		//“G‚ğ‰½‘ÌŠÑ’Ê‚·‚é‚©
	int life_span;		//’e‚Ì¶‘¶ŠÔ
};

//’e‚Ìƒf[ƒ^
static PlayerBullet pBullet[BULLET_NUM]
{
	//cos   dam  spe   rad   h_c  life
	{"’Êí’e",  1,	1.f, 10.f, 20.f,   1, 120},
	{"‹­‰»’e", 10,   7.f, 15.f, 30.f,   5, 180},
	{"Å‹­’e", 50,  50.f, 25.f, 40.f, 100, 360}
};
