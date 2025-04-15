#include "ShapeCollider.h"
#include "ObjectBase.h"
#include "ActorBase.h"

void ShapeCollider::Initialize()
{
	location = 0;
	local_location = 0;
	box_size = 0;
}

bool ShapeCollider::CheckHit(ShapeCollider* object1, ShapeCollider* object2)
{
	return CheckHit(object1->GetLocation(), object1->GetSize(), object2->GetLocation(), object2->GetSize());
}

bool ShapeCollider::CheckHit(Vector2D _loc, Vector2D _size, ShapeCollider* object2)
{
	return CheckHit(_loc, _size, object2->GetLocation(), object2->GetSize());
}

bool ShapeCollider::CheckHit(Vector2D _loc1, Vector2D _size1, Vector2D _loc2, Vector2D _size2)
{
	Vector2D loc1 = _loc1;
	Vector2D size1 = _size1;
	Vector2D loc2 = _loc2;
	Vector2D size2 = _size2;
	//“–‚½‚è”»’èŠm”F
	if (loc1.x < loc2.x + size2.x &&
		loc2.x < loc1.x + size1.x &&
		loc1.y < loc2.y + size2.y &&
		loc2.y < loc1.y + size1.y)
	{
		return true;
	}
	return false;
}
