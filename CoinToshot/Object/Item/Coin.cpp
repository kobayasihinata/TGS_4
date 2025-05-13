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
	//移動終了後にもう一回他のコインと結合しようとする
	if ((int)velocity.x == 0 && (int)velocity.y == 0)
	{
		check_flg = false;
	}
}

void Coin::Draw()const
{
	__super::Draw();
	//コイン仮
	DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xffff00, true);
#ifdef _DEBUG
	//自身の獲得枚数を表示
	DrawFormatStringF(local_location.x, local_location.y, 0xff0000, "%d", (int)add_num);
#endif
}

void Coin::Hit(ObjectBase* hit_object)
{
	//触れた対象が死んでいたら終了
	if (hit_object->GetDeathFlg())
	{
		return;
	}
	//触れた対象がコインで、フラグが立っているなら終了
	if (hit_object->GetObjectType() == eCOIN && check_flg)
	{
		return;
	}
	//触れた対象がプレイヤーとコイン以外なら終了
	if (hit_object->GetObjectType() != ePLAYER && hit_object->GetObjectType() != eCOIN)
	{
		return;
	}
	
	//コイン同士が引き寄せられる
	//if (hit_object->GetObjectType() == eCOIN && 
	//	COIN_FUSION_RANGE < sqrtf(powf(this->location.x - hit_object->GetLocation().x,2) + powf(this->location.y - hit_object->GetLocation().y,2)) &&
	//	this->add_num > 0 && 
	//	static_cast<Coin*>(hit_object)->add_num > 0)
	//{
	//	location.x += ((hit_object->GetLocation().x - this->location.x) / 200);
	//	location.y += ((hit_object->GetLocation().y - this->location.y) / 200);
	//}


	//コインに触れたらそのコインを消し、そのコインの獲得枚数を加算する
	//条件１:hit_objectがCoinである
	//条件２:自身とhit_object二点間の距離が指定した値以下
	//条件３:自身のコイン加算数が0以上
	//条件４:hit_object(coin)の加算数が0以上
	if (hit_object->GetObjectType() == eCOIN &&
		COIN_FUSION_RANGE > sqrtf(powf(this->location.x - hit_object->GetLocation().x, 2) + powf(this->location.y - hit_object->GetLocation().y, 2)) &&
		this->add_num > 0 &&
		static_cast<Coin*>(hit_object)->add_num > 0)
	{
		add_num += static_cast<Coin*>(hit_object)->add_num;
		static_cast<Coin*>(hit_object)->add_num = 0;
		manager->DeleteObject(hit_object);

		return;
	}

	//上の処理は生成後の一回だけ
	check_flg = true;

	//プレイヤーに触れたらコイン加算して消える
	if (this->add_num > 0 && hit_object->GetObjectType() == ePLAYER)
	{
		UserData::coin += add_num;
		std::string s = "+" + std::to_string((int)add_num);
		ingame->CreatePopUp(this->location, s, GetRand(100), 0xffff00, 1, 60);

		manager->DeleteObject(this);

		return;
	}
}

