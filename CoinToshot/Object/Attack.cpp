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

void Attack::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size)
{
	__super::Initialize(_manager, _object_type, init_location, init_size);
}

void Attack::Finalize()
{

}

void Attack::Update()
{
	//ˆê’èŽžŠÔŒo‰ß‚µ‚½‚çŽ©g‚ðíœ‚·‚é
	if (++count_up > time)
	{
		//UŒ‚‚ÌÁ‹Ž
		manager->DeleteObject(this);
	}


	//’e‚ÌˆÚ“®
	this->location += move_velocity;
}

void Attack::Draw()const
{
	__super::Draw();
	//UŒ‚
	DrawString(local_location.x, local_location.y, "attack", 0xffffff);
}

void Attack::Hit(ObjectBase* hit_object)
{
	//UŒ‚“¯Žm‚ª“–‚½‚Á‚½ê‡A–³Ž‹
	if (hit_object->GetObjectType() == eATTACK)
	{
		return;
	}
	//UŒ‚‚µ‚½‚Ì‚ªƒvƒŒƒCƒ„[‚Å‚Í‚È‚­AƒvƒŒƒCƒ„[‚É‚±‚ÌUŒ‚‚ª“–‚½‚Á‚Ä‚¢‚é‚È‚ç
	if (object->GetObjectType() != ePLAYER && hit_object->GetObjectType() == ePLAYER)
	{
		//ƒ_ƒ[ƒW
		hit_object->Damage(1, this->location + (this->box_size / 2));
		//UŒ‚‚ÌÁ‹Ž
		manager->DeleteObject(this);
	}

	//UŒ‚‚µ‚½‚Ì‚ªƒvƒŒƒCƒ„[‚ÅAƒvƒŒƒCƒ„[ˆÈŠO‚ÉUŒ‚‚ª“–‚½‚Á‚Ä‚¢‚é‚È‚ç
	if (object->GetObjectType() == ePLAYER && hit_object->GetObjectType() != ePLAYER)
	{
		//ƒ_ƒ[ƒW
		hit_object->Damage(1, this->location + (this->box_size / 2));
		//UŒ‚‚ÌÁ‹Ž
		manager->DeleteObject(this);
	}
}
