#pragma once
#include "../../Utility/Vector2D.h"
#include "ObjectBase.h"
#include "../Player/PlayerBullet.h"

struct BulletData {
	Vector2D location;	//À•W
	float damage;		//—^ƒ_ƒ[ƒW
	float radius;		//”¼Œa
	float speed;		//‘¬“x
	ObjectBase* who;	//’N‚ª‘Å‚¿o‚µ‚½‚©
	float b_angle;		//Šp“x
	//int b_num;		 //’e‚ğ“¯‚É‰½”­Œ‚‚Â‚©
	int h_count;		//ŠÑ’Êo—ˆ‚é“G‚Ì”
	int delete_time;	//’e‚ğÁ‚·‚Ü‚Å‚ÌŠÔ
	BulletType b_type;		 //’e‚Ìí—Ş
	int color[3];			//’e‚ÌF
};
