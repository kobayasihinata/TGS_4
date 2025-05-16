#include "Coin.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Coin::Coin(InGameScene* _ingame, Vector2D _init_velocity)
{
	ingame = _ingame;
	velocity = _init_velocity;
	add_num = 1;
	check_flg = false;
}

Coin::~Coin()
{

}

void Coin::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Coin::Finalize()
{
	
}

void Coin::Update()
{
	Move();
}

void Coin::Draw()const
{
	__super::Draw();
	//ƒRƒCƒ“‰¼
	DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xffff00, true);
#ifdef _DEBUG
	//Ž©g‚ÌŠl“¾–‡”‚ð•\Ž¦
	DrawFormatStringF(local_location.x, local_location.y, 0xff0000, "%d", (int)add_num);
#endif
}

void Coin::Hit(ObjectBase* hit_object)
{
	//G‚ê‚½‘ÎÛ‚ªŽ€‚ñ‚Å‚¢‚½‚çI—¹
	if (hit_object->GetDeathFlg())
	{
		return;
	}
	//G‚ê‚½‘ÎÛ‚ªƒRƒCƒ“‚ÅAƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é‚È‚çI—¹
	//if (hit_object->GetObjectType() == eCOIN && check_flg)
	//{
	//	return;
	//}
	//G‚ê‚½‘ÎÛ‚ªƒvƒŒƒCƒ„[‚ÆƒRƒCƒ“ˆÈŠO‚È‚çI—¹
	if (hit_object->GetObjectType() != ePLAYER && hit_object->GetObjectType() != eCOIN)
	{
		return;
	}
	
	//ƒRƒCƒ““¯Žm‚ªˆø‚«Šñ‚¹‚ç‚ê‚é
	if (hit_object->GetObjectType() == eCOIN && 
		COIN_FUSION_RANGE < sqrtf(powf(this->location.x - hit_object->GetLocation().x,2) + powf(this->location.y - hit_object->GetLocation().y,2)) &&
		this->add_num > 0 && 
		static_cast<Coin*>(hit_object)->add_num > 0)
	{
		location.x += ((hit_object->GetLocation().x - this->location.x) / 200);
		location.y += ((hit_object->GetLocation().y - this->location.y) / 200);
	}


	//ƒRƒCƒ“‚ÉG‚ê‚½‚ç‚»‚ÌƒRƒCƒ“‚ðÁ‚µA‚»‚ÌƒRƒCƒ“‚ÌŠl“¾–‡”‚ð‰ÁŽZ‚·‚é
	//ðŒ‚PA‚Q:velocity‚ª0(ˆÚ“®‚µ‚Ä‚¢‚È‚¢)
	//ðŒ‚R:hit_object‚ªCoin‚Å‚ ‚é
	//ðŒ‚S:Ž©g‚Æhit_object“ñ“_ŠÔ‚Ì‹——£‚ªŽw’è‚µ‚½’lˆÈ‰º
	//ðŒ‚T:Ž©g‚ÌƒRƒCƒ“‰ÁŽZ”‚ª0ˆÈã
	//ðŒ‚U:hit_object(coin)‚Ì‰ÁŽZ”‚ª0ˆÈã
	if ((int)velocity.x == 0 &&
		(int)velocity.y == 0 &&
		hit_object->GetObjectType() == eCOIN &&
		COIN_FUSION_RANGE > sqrtf(powf(this->location.x - hit_object->GetLocation().x, 2) + powf(this->location.y - hit_object->GetLocation().y, 2)) &&
		this->add_num > 0 &&
		static_cast<Coin*>(hit_object)->add_num > 0
		)
	{
		add_num += static_cast<Coin*>(hit_object)->add_num;
		static_cast<Coin*>(hit_object)->add_num = 0;
		manager->DeleteObject(hit_object);

		return;
	}

	if ((int)velocity.x == 0 &&
		(int)velocity.y == 0)
	{
		//ã‚Ìˆ—‚ÍˆÚ“®’âŽ~Œã‚Ìˆê‰ñ‚¾‚¯
		check_flg = true;
	}

	//ƒvƒŒƒCƒ„[‚ÉG‚ê‚½‚çƒRƒCƒ“‰ÁŽZ‚µ‚ÄÁ‚¦‚é
	if (this->add_num > 0 && hit_object->GetObjectType() == ePLAYER)
	{
		UserData::coin += add_num;
		std::string s = "+" + std::to_string((int)add_num);
		ingame->CreatePopUp(this->location, s, GetRand(100), 0xffff00, 1, 60);

		manager->DeleteObject(this);

		return;
	}
}

