#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

#define KNOCK_BACK 10		//ノックバック時の倍率

class ActorBase :
	public ObjectBase
{
protected:
	int frame;	//フレーム測定
	float hp;
	Vector2D old_location = { 0.0f,0.0f };	//1フレーム前の座標

	Vector2D last_velocity;				//停止する（値がMOVE_LOWER_LIMITを下回る）直前のvelocity

public:
	Vector2D velocity = { 0.0f,0.0f };

public:
	virtual void Update()override
	{
		frame++;
		//停止前に移動してた方向を保存する
		if (fabsf(velocity.x) > MOVE_LOWER_LIMIT)
		{
			last_velocity.x = velocity.x;
		}
		if (fabsf(velocity.y) > MOVE_LOWER_LIMIT)
		{
			last_velocity.y = velocity.y;
		}
	}

	virtual void Draw()const override
	{
		//画像描画
		if (image != 0)
		{
			if (last_velocity.x > 0)
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, false, false);
			}
			else
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, false, true);
			}
		}
#ifdef _DEBUG
		//当たり判定の描画

		//四角
		if (this->radius <= 0.f)
		{
			Vector2D::DrawBoxV2(local_location - (box_size / 2), local_location + (box_size / 2), 0xff0000, false);
		}
		//円
		else
		{
			DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xff0000, false);
		}
#endif // _DEBUG
	}

	//当たり判定が被った時の処理
	virtual void Hit(ObjectBase* hit_object)override
	{
		//自身がプレイヤーではなく、攻撃とアイテム以外なら
		if (this->object_type != ePLAYER &&
			hit_object->GetObjectType() != ObjectList::eATTACK &&
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

		//ダメージ源の中心座標からノックバック方向を求める
		double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
		location.x -= (KNOCK_BACK * cos(radian));
		location.y -= (KNOCK_BACK * sin(radian));
	}

	//移動処理
	void Move()
	{

		DebugInfomation::Add("velox", last_velocity.x);
		DebugInfomation::Add("veloy", last_velocity.y);
		//減速の実行
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//移動前の座標格納
		old_location = location;
		//移動の実行
		location += velocity;
		//壁に当たっていたら前の座標に戻す

		//左端
		if (location.x - (box_size.x/2) <= -STAGE_SIZE)
		{
			location.x = -STAGE_SIZE + (box_size.x / 2);
		}
		//右端
		if (location.x + (box_size.x / 2) > STAGE_SIZE)
		{
			location.x = STAGE_SIZE - (box_size.x / 2);
		}
		//上端
		if (location.y - (box_size.y / 2) <= -STAGE_SIZE)
		{
			location.y = -STAGE_SIZE + (box_size.y / 2);
		}
		//下端
		if (location.y + (box_size.y / 2) > STAGE_SIZE)
		{
			location.y = STAGE_SIZE - (box_size.y / 2);
		}
	}

	//１フレーム前の座標を取得する
	Vector2D GetOldLocation()const { return old_location; }
};

