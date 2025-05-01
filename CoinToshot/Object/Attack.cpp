#include "Attack.h"
#include "ObjectManager.h"
#include "../Utility/common.h"

Attack::Attack(BulletData _bullet_data)
{
	object = _bullet_data.who;
	time = _bullet_data.delete_time;
	count_up = 0;
	move_velocity.x = _bullet_data.speed * cosf(_bullet_data.b_angle);
	move_velocity.y = _bullet_data.speed * sinf(_bullet_data.b_angle);

	//location.x += ((b_speed + acceleration * 0.08) * cosf(rad));
	//location.y += ((b_speed + acceleration * 0.08) * sinf(rad));
}

Attack::~Attack()
{

}

void Attack::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Attack::Finalize()
{

}

void Attack::Update()
{
	//一定時間経過したら自身を削除する
	if (++count_up > time)
	{
		//攻撃の消去
		manager->DeleteObject(this);
	}


	//弾の移動
	this->location += move_velocity;
}

void Attack::Draw()const
{
	__super::Draw();
	//コイン仮(プレイヤー)
	if (object->GetObjectType() == ePLAYER)
	{
		DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xffaa00, true);
	}
	//コイン仮(敵)
	else
	{
		DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xff0000, true);
	}
}

void Attack::Hit(ObjectBase* hit_object)
{
	//当たったオブジェクトが死亡演出中ならスキップ
	if (hit_object->GetDeathFlg())
	{
		return;
	}

	//攻撃同士が当たった場合か、落ちてるコインと当たった場合、無視
	if (hit_object->GetObjectType() == eATTACK || hit_object->GetObjectType() == eCOIN)
	{
		return;
	}

	//攻撃したのがプレイヤーではなく、プレイヤーにこの攻撃が当たっているなら
	if (object->GetObjectType() != ePLAYER && hit_object->GetObjectType() == ePLAYER)
	{
		//ダメージ
		hit_object->Damage(1, this->location);
		//攻撃の消去
		manager->DeleteObject(this);
	}

	//攻撃したのがプレイヤーで、プレイヤー以外に攻撃が当たっているなら
	if (object->GetObjectType() == ePLAYER && hit_object->GetObjectType() != ePLAYER)
	{
		//ダメージ
		hit_object->Damage(1, this->location);
		//攻撃の消去
		manager->DeleteObject(this);
	}
}
