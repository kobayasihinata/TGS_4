#include "CoinGet.h"
#include "../ObjectManager.h"

CoinGet::CoinGet()
{

}

CoinGet::~CoinGet()
{

}

void CoinGet::Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span)
{
	__super::Initialize(_manager, init_location, _front_flg, _timer, _anim_span);
	init_timer = _timer;

	ResourceManager* rm = ResourceManager::GetInstance();

	animation_image = rm->GetImages("Resource/Images/Effect/coin_get.png", 99, 11, 9, 64, 64);

	color = 4;//0~8のランダム　一回色固定

	image = animation_image[color*11];

}

//終了時処理
void CoinGet::Finalize()
{

}

//更新処理
void CoinGet::Update()
{

	if (timer % 6 == 0)
	{
		manager->CreateEffect(elShine, { location.x + GetRand(30) - 15,location.y + GetRand(30) - 15 }, TRUE, 0xffff00);
		if (now_anim >= 11)
		{
			image = 0;
		}
		else
		{
			image = animation_image[(color*11) + now_anim];
			now_anim++;
		}
	}

	//時間が過ぎたら自身を消す
	if (--timer <= 0)
	{
		manager->DeleteEffect(this);
	}
}

//描画処理
void CoinGet::Draw()const
{
	__super::Draw();
}