#pragma once

#define BULLET_NUM 5	//’e‚Ìí—Ş

//ƒvƒŒƒCƒ„[‚Ì’e‚É•K—v‚Èî•ñ
struct PlayerBullet
{
	char name[10];		//’e‚Ì–¼Ì
	int cost;			//”­Ë‚É‚©‚©‚éƒRƒCƒ“”
	float damage;		//ƒ_ƒ[ƒW—Ê
	float speed;		//’e‘¬
	float radius;		//’e‚Ì”¼Œa
	int h_count;		//“G‚ğ‰½‘ÌŠÑ’Ê‚·‚é‚©
	int life_span;		//’e‚Ì¶‘¶ŠÔ
	int cooldown;		//’e‚Ì”­ËŠÔŠu

	int bullet_num;		//’e‚ğ‚¢‚­‚Â”­Ë‚·‚é‚©
	float space;		//‚Ç‚ê‚­‚ç‚¢‚ÌŠÔŠu‚ğŠJ‚¯‚ÄŒ‚‚Â‚©
	int color[3];			//F
};

//’e‚Ìƒf[ƒ^
static PlayerBullet pBullet[BULLET_NUM]
{
	//name    cos   dam  spe   rad   h_c  life  cd  num space
	{"’Êí’e",   1,	  1.f, 10.f, 20.f,   1, 120,  1, 1,    0,{255,255,255}},
	{"ŠgU’e",   5,   2.f, 10.f, 20.f,   1,  30,  30, 5, 0.3f,{0,255,255}},
	{"‹­‰»’e",  10,   7.f, 15.f, 30.f,   5, 180, 10, 1,    0,{255,127,0}},
	{"”š”­’e",  30,   3.f, 10.f, 10.f, 100,  20, 10, 1,	   0,{255,0,0}},
	{"Å‹­’e", 100,  20.f, 25.f, 40.f, 100, 360,180, 1,    0,{0,0,0}},
};

//ƒf[ƒ^‚Æ“¯‚¶‡”Ô
static enum BulletType {
	bNormal=0,
	bShotgun,
	bStrong,
	bExplosion,
	bStrongest,

};