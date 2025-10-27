#include "Coin.h"
#include "../ObjectManager.h"
#include "../../Utility/UserData.h"
#include "../../Scene/InGameScene.h"

Coin::Coin(InGameScene* _ingame, Vector2D _init_velocity)
{
	camera = Camera::Get();

	ingame = _ingame;
	velocity = _init_velocity;
	add_num = 1;
	check_flg = false;

	//SE読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	get_se = rm->GetSounds("Resource/Sounds/Coin/Get.mp3");
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

	//獲得枚数に応じてデカくなる
	radius = 20.f + add_num;
	box_size = radius * 2;
	//吸い寄せのフラグが立っていたら、プレイヤーに向けて移動する
	if (UserData::attraction_flg)
	{
		velocity.x += ((camera->player_location.x - this->location.x) / 200);
		velocity.y += ((camera->player_location.y - this->location.y) / 200);
	}
	if (++frame % 20 == 0)
	{
		manager->CreateEffect(elShine, { location.x+GetRand((int)radius)-(radius/2),location.y + GetRand((int)radius) - (radius / 2) }, TRUE, 0xffff00);
	}
}

void Coin::Draw()const
{
	__super::Draw();

	float shift = abs((frame % 60) - 30)/5;	
	//大きさによって軽量描画か普通描画か変える
	if (radius < 100)
	{
		//小さいコインを軽量描画
		UserData::DrawDefaultCoin({ local_location.x,local_location.y + shift }, radius);
	}
	else
	{
		//大きいコインを普通描画
		UserData::DrawCoin(local_location, radius);
	}
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
	//if (hit_object->GetObjectType() == eCOIN && check_flg)
	//{
	//	return;
	//}
	//触れた対象がプレイヤーとコイン以外なら終了
	if (hit_object->GetObjectType() != ePLAYER && hit_object->GetObjectType() != eCOIN)
	{
		return;
	}
	
	//コイン同士が引き寄せられる
	if (hit_object->GetObjectType() == eCOIN && 
		UserData::player_hp > 0 &&
		COIN_FUSION_RANGE < sqrtf(powf(this->location.x - hit_object->GetLocation().x,2) + powf(this->location.y - hit_object->GetLocation().y,2)) &&
		this->add_num > 0 && 
		static_cast<Coin*>(hit_object)->add_num > 0)
	{
		location.x += ((hit_object->GetLocation().x - this->location.x) / 200);
		location.y += ((hit_object->GetLocation().y - this->location.y) / 200);
	}


	//コインに触れたらそのコインを消し、そのコインの獲得枚数を加算する
	//条件１、２:velocityが0(移動していない) or 座標が完全に一致
	//条件３:hit_objectがCoinである
	//条件４:プレイヤーが死んでいない
	//条件５:自身とhit_object二点間の距離が指定した値以下
	//条件６:自身のコイン加算数が0以上
	//条件７:hit_object(coin)の加算数が0以上
	if ((((int)velocity.x == 0 && (int)velocity.y == 0)||
		(location == hit_object->GetLocation()))
		&&
		hit_object->GetObjectType() == eCOIN &&
		UserData::player_hp > 0 &&
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
		//上の処理は移動停止後の一回だけ
		check_flg = true;
	}

	//プレイヤーに触れたらコイン加算して消える
	if (this->add_num > 0 && hit_object->GetObjectType() == ePLAYER)
	{
		UserData::coin += add_num;
		std::string s = "+" + std::to_string((int)add_num);
		ingame->CreatePopUp(this->location, s,0xffff33);

		manager->DeleteObject(this);
		manager->CreateEffect(elCoin, { location.x + GetRand((int)radius) - (radius / 2),location.y + GetRand((int)radius) - (radius / 2) }, TRUE, 0xffff00);
		ResourceManager::rPlaySound(get_se, DX_PLAYTYPE_BACK);
		return;
	}
}

