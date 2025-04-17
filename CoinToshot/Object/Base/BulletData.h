#pragma once
#include "../../Utility/Vector2D.h"
#include "ObjectBase.h"

struct BulletData {
	Vector2D location;	//À•W
	float radius;		//”¼Œa
	float speed;		//‘¬“x
	ObjectBase* who;			//’N‚ª‘Å‚¿o‚µ‚½‚©
	float b_angle;		//Šp“x
	//int b_num;		 //’e‚ğ“¯‚É‰½”­Œ‚‚Â‚©
	int h_count;		//ŠÑ’Êo—ˆ‚é“G‚Ì”
	int delete_time;	//’e‚ğÁ‚·‚Ü‚Å‚ÌŠÔ
	//int b_type;		 //’e‚Ìí—Ş
};
