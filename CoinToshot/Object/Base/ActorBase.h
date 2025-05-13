#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

class ActorBase :
	public ObjectBase
{
protected:
	int frame;	//フレーム測定
	float hp;
	Vector2D old_location = { 0.0f,0.0f };	//1フレーム前の座標

	Vector2D last_velocity;				//停止する（値がMOVE_LOWER_LIMITを下回る）直前のvelocity
	Vector2D move_velocity;				//移動したい方向の保存(実際の移動量とは異なる)

	int drop_coin;			//ドロップするコインの量
	int drop_coin_count;	//ドロップするコインの計測
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

		//死亡時透明になっていく（仮演出）
		if (death_flg)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, death_timer * 4);
		}

		//画像描画
		if (image != 0)
		{
			if (move_velocity.x > 0)
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, true, false);
			}
			else
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, true, true);
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

		//死亡時透明を解除（仮演出）
		if (death_flg)
		{
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		}
	}

	//当たり判定が被った時の処理
	virtual void Hit(ObjectBase* hit_object)override
	{
		//自身が生きてる且つプレイヤーではなく、攻撃とアイテム以外なら
		if (!(this->death_flg) && 
			this->object_type != ePLAYER &&
			hit_object->GetObjectType() != ObjectList::eATTACK &&
			hit_object->GetObjectType() != ObjectList::eCOIN &&
			hit_object->GetObjectType() != ObjectList::eHEAL)
		{
			//オブジェクトを押す
			Push(hit_object);
		}
	}

	virtual void Damage(float _value, Vector2D _attack_loc)override
	{
		//自身のHPが1以上ならこの処理
		if (hp > 0)
		{

			hp -= _value;
			//ノックバック処理

			//ダメージ源の中心座標からノックバック方向を求める
			double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
			velocity.x -= (KNOCK_BACK * cos(radian));
			velocity.y -= (KNOCK_BACK * sin(radian));

			//死
			if (hp <= 0)
			{
				this->death_flg = true;
				//アニメーション位置をリセット
				image_num = 0;
			}
		}
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

