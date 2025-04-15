#include "Block.h"

Block::Block()
{

}

Block::~Block()
{

}

void Block::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
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

void Block::Hit(ObjectBase* hit_Object)
{

}
