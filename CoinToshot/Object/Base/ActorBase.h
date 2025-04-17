#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

#define KNOCK_BACK 1.5f;		//ノックバック時の倍率

class ActorBase :
	public ObjectBase
{
protected:
	int frame;	//フレーム測定
	float hp;
	Vector2D old_location = { 0.0f,0.0f };	//1フレーム前の座標
public:
	Vector2D velocity = { 0.0f,0.0f };

public:
	virtual void Update()override
	{
		frame++;
	}

	//当たり判定が被った時の処理
	virtual void Hit(ObjectBase* hit_object)override
	{
		//攻撃とアイテム以外なら
		if (hit_object->GetObjectType() != ObjectList::eATTACK &&
			hit_object->GetObjectType() != ObjectList::eCOIN &&
			hit_object->GetObjectType() != ObjectList::eHEAL)
		{
			//押す前の座標を保存
			float old = location.y;
			//オブジェクトを押す
			Push(hit_object);
		}
	}

	virtual void Damage(float _value, Vector2D _attack_loc)override
	{
		hp -= _value;
		//ノックバック処理

		//プレイヤーの中心座標
		Vector2D player_center = this->location + (this->box_size / 2);
		//プレイヤーの中心座標が攻撃の中心座標より大きければ
		if (player_center.x > _attack_loc.x)
		{
			velocity.x = 5;
		}
		//プレイヤーの中心座標が攻撃の中心座標より小さければ
		else
		{
			velocity.x = -5;
		}
		velocity.y = -5;
	}
	//移動処理
	void Move()
	{
		//減速の実行
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//移動前の座標格納
		old_location = location;
		//移動の実行
		location += velocity;
		//壁に当たっていたら前の座標に戻す

		//左端
		if (location.x <= -STAGE_SIZE)
		{
			location.x = -STAGE_SIZE;
		}
		//右端
		if (location.x + box_size.x > STAGE_SIZE)
		{
			location.x = STAGE_SIZE - box_size.x;
		}
		//上端
		if (location.y <= -STAGE_SIZE)
		{
			location.y = -STAGE_SIZE;
		}
		//下端
		if (location.y + box_size.y > STAGE_SIZE)
		{
			location.y = STAGE_SIZE - box_size.y;
		}
	}

	//１フレーム前の座標を取得する
	Vector2D GetOldLocation()const { return old_location; }
};

