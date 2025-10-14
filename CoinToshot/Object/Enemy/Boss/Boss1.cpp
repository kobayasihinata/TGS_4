#include "Boss1.h"
#include "../../ObjectManager.h"
#include "../../../Scene/InGameScene.h"
#include "../../../Utility/common.h"

Boss1::Boss1(InGameScene* _ingame)
{
	ingame = _ingame;
	move_speed = BOSS1_SPEED;
	max_hp = hp = BOSS1_HP;
	hit_damage = BOSS1_DAMAGE;
	//指定したドロップ量から±1の間でランダムにコインをドロップ
	drop_coin = BOSS1_DROPCOIN + (GetRand(2) - 1);


	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/Enemy1/Enemy1_Walk.png", 24, 5, 5, 96, 96);
	animation_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/Enemy1/Enemy1_Death.png", 15, 5, 3, 96, 96);
	animation_image.push_back(tmp);
	//tmp = rm->GetImages("Resource/Images/Player/Run2.png", 12, 12, 1, 64, 64);
	//animation_image.push_back(tmp);
	image = animation_image[0][0];

	//SE読み込み
	death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
}

Boss1::~Boss1()
{

}

void Boss1::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	anim_span = 1;
}

void Boss1::Finalize()
{

}

void Boss1::Update()
{
	__super::Update();

	//移動
	Move();

	//アニメーション
	Animation();
	
	//HPゲージ表示
	ingame->SetBossHp("Boss1", hp, max_hp, hpbar_move);

	//死亡演出フラグが立っているなら
	if (death_flg)
	{
		//死亡アニメーションを表示
		image_line = 1;
		anim_span = 3;

		//死にながらコインをまき散らす
		if (++death_timer % 20 == 0 && drop_coin_count < drop_coin)
		{
			Vector2D rand = { (float)(GetRand(this->GetSize().x) - this->GetSize().x / 2),(float)(GetRand(this->GetSize().y) - this->GetSize().y / 2) };
			manager->CreateObject(
				eCOIN,
				this->location + rand,
				Vector2D{ 40, 40 },
				20.f,
				rand);
			drop_coin_count++;
		}

		//死亡演出時間を過ぎたら自身を削除
		if (anim_end_flg)
		{
			manager->DeleteObject(this);
			//演出中に出せなかったコインをまとめてドロップ
			for (int i = drop_coin_count; i < drop_coin; i++)
			{
				Vector2D rand = { (float)(GetRand(25) - 12),(float)(GetRand(25) - 12) };
				manager->CreateObject(
					eCOIN,
					this->location + rand,
					Vector2D{ 40, 40 },
					20.f,
					rand);
			}
			//エフェクト
			manager->CreateEffect(elExplosion, this->location);

			//SE再生
			ResourceManager::rPlaySound(death_se, DX_PLAYTYPE_BACK);
		}
	}
}

void Boss1::Draw()const
{
	__super::Draw();
	Vector2D::DrawBoxV2(local_location - (box_size / 2), local_location + (box_size / 2), 0xff0000, false);
}

void Boss1::Hit(ObjectBase* hit_Object)
{
	__super::Hit(hit_Object);
}

void Boss1::Damage(float _value, Vector2D _attack_loc, int _knock_back)
{
	__super::Damage(_value, _attack_loc, _knock_back);
	//減少アニメーション
	hpbar_move += _value * (BOSS_BAR_SIZE / max_hp);

}

