#include "Block.h"

Block::Block()
{

}

Block::~Block()
{

}

void Block::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Block::Finalize()
{

}

void Block::Update()
{

}

void Block::Draw()const
{
	__super::Draw();
}

void Block::Hit(ObjectBase* hit_object)
{
	//当たらなくなるまで繰り返す
	while (this->CheckHit(hit_object))
	{
		//オブジェクトを押す
		hit_object->Push(this);
	}
}

void Block::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{

}
