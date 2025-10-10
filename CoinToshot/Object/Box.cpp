#include "Box.h"
#include "ObjectManager.h"

Box::Box()
{
	hp = 3;
	drop_coin = GetRand(20) + 10;
	drop_coin_count = 0;

	//読込
	ResourceManager* rm = ResourceManager::GetInstance();
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

	//移動
	Move();

	//アニメーション
	Animation();

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡時アニメーションに遷移
		image_line = 1;
		anim_span = 5;

		//死にながらコインをまき散らす
		if (++death_timer % 10 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand / 3);
			drop_coin_count++;
		}

		//死亡演出時間を過ぎたら自身を削除
		//if (anim_end_flg)アニメーションがある時用
		if (death_timer > 60)
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
			manager->CreateEffect(elExplosion, this->location);

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
}

void Box::Draw()const
{
	__super::Draw();
	
	if (death_flg)
	{
		DrawBoxAA(local_location.x - (box_size.x / 2) + GetRand(6) - 3,
			local_location.y - (box_size.y / 2) + GetRand(6) - 3,
			local_location.x + (box_size.x / 2) + GetRand(6) - 3,
			local_location.y + (box_size.y / 2) + GetRand(6) - 3,
			0x7b4d28, true);
		DrawString(local_location.x, local_location.y, "はこ", 0xffffff);
	}
	else
	{
		Vector2D::DrawBoxV2(local_location - (box_size / 2), local_location + (box_size / 2), 0x7b4d28, true);
		DrawString(local_location.x, local_location.y, "はこ", 0xffffff);
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
		//ダメージSE
		ResourceManager::rPlaySound(damage_se, DX_PLAYTYPE_BACK);
	}
	//HPが０以下なら死亡処理
	if (hp <= 0)
	{
		death_flg = true;

		//ノックバック処理
		double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
		velocity.x -= (_knock_back * cos(radian));
		velocity.y -= (_knock_back * sin(radian));

		//アニメーション位置をリセット
		image_num = 0;
		anim_end_flg = false;
	}
}
