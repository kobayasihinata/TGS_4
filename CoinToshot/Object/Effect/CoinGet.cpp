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

	animation_image = rm->GetImages("Resource/Images/Effect/shine.png", 25, 5, 5, 64, 56);

	image = animation_image[0];

}

//終了時処理
void CoinGet::Finalize()
{

}

//更新処理
void CoinGet::Update()
{
	//アニメーションの切り替え速度を、画像枚数とエフェクト時間から割り出す
	int r = init_timer / (animation_image.size() - 1);
	if (r <= 0)r = 1;
	if (timer % r == 0)
	{
		manager->CreateEffect(elShine, { location.x + GetRand(30) - 15,location.y + GetRand(30) - 15 }, TRUE, 0xffff00);
		if (now_anim >= (animation_image.size() - 1))
		{
			image = 0;
		}
		else
		{
			image = animation_image[now_anim];
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