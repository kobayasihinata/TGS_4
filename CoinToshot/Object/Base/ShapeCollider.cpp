#include "ShapeCollider.h"
#include "ObjectBase.h"
#include "ActorBase.h"

void ShapeCollider::Initialize(Vector2D init_location, Vector2D init_size, float init_radius)
{
	location = init_location;
	local_location = 0;
	box_size = init_size;
	radius = init_radius;
	//半径が0より大きく指定されていたら、box_sizeを半径に合わせる
	if (radius > 0)
	{
		box_size = radius * 2;
	}
}

bool ShapeCollider::CheckHit(ShapeCollider* object)
{
	//自身も相手も四角なら
	if (this->radius <= 0.f && object->radius <= 0.f)
	{
		return CollisionCheckBtoB(object->GetLocation(), object->GetSize());
	}
	//自身が四角で、相手が円なら
	if (this->radius <= 0.f && object->radius > 0.f)
	{
		return CollisionCheckBtoC(this->location,this->box_size,object->GetLocation(), object->GetRadius());
	}
	//自身も相手も円なら
	if (this->radius > 0.f && object->radius > 0.f)
	{
		return CollisionCheckCtoC(object->GetLocation(), object->GetRadius());
	}
	//自身が円で、相手が四角なら
	if (this->radius > 0.f && object->radius <= 0.f)
	{
		return CollisionCheckBtoC(object->GetLocation(), object->GetSize(), this->location, this->radius);
	}
}


bool ShapeCollider::CollisionCheckBtoB(Vector2D _loc2, Vector2D _size2)const
{
	Vector2D loc1 = this->location;
	Vector2D size1 = this->box_size;
	Vector2D loc2 = _loc2;
	Vector2D size2 = _size2;
	//当たり判定確認
	if (loc1.x - (size1.x / 2) < loc2.x + (size2.x / 2) &&
		loc2.x - (size2.x / 2) < loc1.x + (size1.x / 2) &&
		loc1.y - (size1.y / 2) < loc2.y + (size2.y / 2) &&
		loc2.y - (size2.y / 2) < loc1.y + (size1.y / 2))
	{
		return true;
	}
	return false;
}

bool ShapeCollider::CollisionCheckBtoC(Vector2D _loc1, Vector2D _size1, Vector2D _loc2, float _radius2)const
{
	bool nResult = false;

	float box_left = _loc1.x - (_size1.x / 2);
	float box_right = _loc1.x + (_size1.x / 2);
	float box_up = _loc1.y + (_size1.y / 2);
	float box_down = _loc1.y - (_size1.y / 2);

	// 四角形の四辺に対して円の半径分だけ足したとき円が重なっていたら
	if ((_loc2.x > box_left - _radius2) &&
		(_loc2.x < box_right + _radius2) &&
		(_loc2.y > box_down - _radius2) &&
		(_loc2.y < box_up + _radius2))
	{
		nResult = true;
		float fl = _radius2 * _radius2;


		// 左
		if (_loc2.x < box_left)
		{
			// 左上
			if ((_loc2.y < box_down))
			{
				if ((DistanceSqrf(box_left, box_down, _loc2.x, _loc2.y) >= fl))
				{
					nResult = false;
				}
			}
			else
			{
				// 左下
				if (_loc2.y > box_up)
				{
					if ((DistanceSqrf(box_left, box_up, _loc2.x, _loc2.y) >= fl))
					{
						nResult = false;
					}
				}
			}
		}
		else
		{
			// 右
			if (_loc2.x > box_right)
			{
				// 右上
				if ((_loc2.y < box_down))
				{
					if ((DistanceSqrf(box_right, box_down, _loc2.x, _loc2.y) >= fl))
					{
						nResult = false;
					}
				}
				else
				{
					// 右下
					if (_loc2.y > box_up)
					{
						if ((DistanceSqrf(box_right, box_up, _loc2.x, _loc2.y) >= fl))
						{
							nResult = false;
						}
					}
				}
			}
		}
	}

	return nResult;
}

bool ShapeCollider::CollisionCheckCtoC(Vector2D _loc, float _radius)const
{
	return this->radius + _radius > sqrt(powf(location.x - _loc.x, 2) + pow(location.y - _loc.y, 2));
}

float ShapeCollider::DistanceSqrf(const float t_x1, const float t_y1, const float t_x2, const float t_y2)const
{
	float dx = t_x2 - t_x1;
	float dy = t_y2 - t_y1;

	return (dx * dx) + (dy * dy);
}

void ShapeCollider::Push(ShapeCollider* hit_object)
{
	float shot_rad = atan2f(hit_object->GetLocation().y - this->location.y, hit_object->GetLocation().x - this->location.x);
	this->location.x -= cosf(shot_rad);
	this->location.y -= sinf(shot_rad);
}