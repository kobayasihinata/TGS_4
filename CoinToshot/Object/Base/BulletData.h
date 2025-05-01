#pragma once
#include "../../Utility/Vector2D.h"
#include "ObjectBase.h"

struct BulletData {
	Vector2D location;	//座標
	float damage;		//与ダメージ
	float radius;		//半径
	float speed;		//速度
	ObjectBase* who;			//誰が打ち出したか
	float b_angle;		//角度
	//int b_num;		 //弾を同時に何発撃つか
	int h_count;		//貫通出来る敵の数
	int delete_time;	//弾を消すまでの時間
	//int b_type;		 //弾の種類
};
