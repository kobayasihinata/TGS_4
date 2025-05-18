#include "ShapeCollider.h"
#include "ObjectBase.h"
#include "ActorBase.h"

void ShapeCollider::Initialize(Vector2D init_location, Vector2D init_size, float init_radius)
{
	location = init_location;
	local_location = 0;
	box_size = init_size;
	radius = init_radius;
	//���a��0���傫���w�肳��Ă�����Abox_size�𔼌a�ɍ��킹��
	if (radius > 0)
	{
		box_size = radius * 2;
	}
}

bool ShapeCollider::CheckHit(ShapeCollider* object)
{
	//���g��������l�p�Ȃ�
	if (this->radius <= 0.f && object->radius <= 0.f)
	{
		return CollisionCheckBtoB(object->GetLocation(), object->GetSize());
	}
	//���g���l�p�ŁA���肪�~�Ȃ�
	if (this->radius <= 0.f && object->radius > 0.f)
	{
		return CollisionCheckBtoC(this->location,this->box_size,object->GetLocation(), object->GetRadius());
	}
	//���g��������~�Ȃ�
	if (this->radius > 0.f && object->radius > 0.f)
	{
		return CollisionCheckCtoC(object->GetLocation(), object->GetRadius());
	}
	//���g���~�ŁA���肪�l�p�Ȃ�
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
	//�����蔻��m�F
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

	// �l�p�`�̎l�ӂɑ΂��ĉ~�̔��a�������������Ƃ��~���d�Ȃ��Ă�����
	if ((_loc2.x > box_left - _radius2) &&
		(_loc2.x < box_right + _radius2) &&
		(_loc2.y > box_down - _radius2) &&
		(_loc2.y < box_up + _radius2))
	{
		nResult = true;
		float fl = _radius2 * _radius2;


		// ��
		if (_loc2.x < box_left)
		{
			// ����
			if ((_loc2.y < box_down))
			{
				if ((DistanceSqrf(box_left, box_down, _loc2.x, _loc2.y) >= fl))
				{
					nResult = false;
				}
			}
			else
			{
				// ����
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
			// �E
			if (_loc2.x > box_right)
			{
				// �E��
				if ((_loc2.y < box_down))
				{
					if ((DistanceSqrf(box_right, box_down, _loc2.x, _loc2.y) >= fl))
					{
						nResult = false;
					}
				}
				else
				{
					// �E��
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