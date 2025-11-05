#include "Box.h"
#include "ObjectManager.h"

Box::Box()
{
	hp = 3;
	drop_coin = GetRand(20) + 10;
	drop_coin_count = 0;

	impact = 0;

	//読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Item/Box.png", 1, 1, 1, 48, 43);
	animation_image.push_back(tmp);

	image = animation_image[0][0];
	//SE読み込み
	death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
	item_spawn_se = rm->GetSounds("Resource/Sounds/Direction/成功音.mp3");
}

Box::~Box()
{

}

void Box::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
}

void Box::Finalize()
{

}

void Box::Update()
{
	__super::Update();

	//箱振動
	if (impact > 0)
	{
		impact--;
	}
	//移動
	Move();

	//アニメーション
	Animation();

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
			manager->DeleteObject(this);
			//演出中に出せなかったコインをまとめてドロップ
			for (int i = drop_coin_count; i < drop_coin; i++)
			{
				Vector2D rand = { (float)(GetRand(20) - 10),(float)(GetRand(20) - 10) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{ 40, 40 },
					20.f,
					rand);
			}
			//エフェクト
			manager->CreateEffect(elExplosion, this->location,{0,0});

			//一定確率でアイテム生成＆アイテムSE再生
			if (GetRand(ITEM_DROP) == 0)
			{
				//回復か磁石
				if (GetRand(50 - UserData::player_hp) > 10)
				{
					manager->CreateObject(
						eHEAL,
						this->location,
						Vector2D{40, 40});
				}
				else
				{
					manager->CreateObject(
						eMAGNET,
						this->location,
						Vector2D{40, 40});
				}
				//SE再生
				ResourceManager::rPlaySound(item_spawn_se, DX_PLAYTYPE_BACK);
			}
			//レアでないなら通常のSE再生
			else
			{
				//SE再生
				ResourceManager::rPlaySound(death_se, DX_PLAYTYPE_BACK);
			}
	}
}

void Box::Draw()const
{
	Vector2D loc{ 0,10 };	//振動用の移動＋画像の上下のずれ修正

	loc.x = (float)GetRand(impact) - (float)(impact / 2);
	//画像描画
	if (image != 0)
	{
		if (move_velocity.x > 0)
		{
			DrawRotaGraphF(local_location.x + loc.x, local_location.y + loc.y, 1, 0, image, true, false);
		}
		else
		{
			DrawRotaGraphF(local_location.x + loc.x, local_location.y + loc.y, 1, 0, image, true, true);
		}
	}
}

void Box::Hit(ObjectBase* hit_object)
{
	//プレイヤーと敵だけ弾く
	if (hit_object->GetObjectType() == ePLAYER || hit_object->IsEnemy())
	{
		//当たらなくなるまで繰り返す
		while (this->CheckHit(hit_object))
		{
			//オブジェクトを押す
			hit_object->Push(this);
		}
	}
}

void Box::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{

	if (hp > 0)
	{
		hp -= _value;
		impact = 10;
		//ダメージSE
		ResourceManager::rPlaySound(damage_se, DX_PLAYTYPE_BACK);

	}
	//HPが０以下なら死亡処理
	if (hp <= 0)
	{
		death_flg = true;
	}
}
