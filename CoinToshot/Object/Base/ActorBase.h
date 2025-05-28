#pragma once
#include "ObjectBase.h"
#include "ObjectList.h"
#include "../../Utility/common.h"

class ActorBase :
	public ObjectBase
{
protected:
	int frame;		//フレーム測定
	float max_hp;	//最大HP記憶
	float hp;		//HP格納
	float hpbar_move;	//HPゲージ減少アニメーション
	Vector2D old_location = { 0.0f,0.0f };	//1フレーム前の座標

	Vector2D last_velocity;				//停止する（値がMOVE_LOWER_LIMITを下回る）直前のvelocity
	Vector2D move_velocity;				//移動したい方向の保存(実際の移動量とは異なる)

	int drop_coin;			//ドロップするコインの量
	int drop_coin_count;	//ドロップするコインの計測

	int damage_se;			//ダメージ音
public:
	Vector2D velocity = { 0.0f,0.0f };

public:
	virtual void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f, float init_radius = 0.f)override
	{
		__super::Initialize(_manager, _object_type,init_location, init_size, init_radius);

		//初期化
		frame = 0;
		hpbar_move = 0.f;
		old_location = 0.f;
		last_velocity = 0.f;				
		move_velocity = 0.f;
		drop_coin_count = 0;

		//SE読込
		ResourceManager* rm = ResourceManager::GetInstance();
		//SE読み込み
		damage_se = rm->GetSounds("Resource/Sounds/damage.mp3");
	}

	virtual void Update()override
	{
		frame++;

		//停止前に移動してた方向を保存する
		if (fabsf(velocity.x) > MOVE_LOWER_LIMIT)
		{
			last_velocity.x = velocity.x;
		}
		if (fabsf(velocity.y) > MOVE_LOWER_LIMIT)
		{
			last_velocity.y = velocity.y;
		}
	}

	virtual void Draw()const override
	{
		//画像描画
		if (image != 0)
		{
			if (move_velocity.x > 0)
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, true, false);
			}
			else
			{
				DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0, image, true, true);
			}
		}
#ifdef _DEBUG
		//当たり判定の描画

		//四角
		if (this->radius <= 0.f)
		{
			Vector2D::DrawBoxV2(local_location - (box_size / 2), local_location + (box_size / 2), 0xff0000, false);
		}
		//円
		else
		{
			DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xff0000, false);
		}
#endif // _DEBUG

	}

	//当たり判定が被った時の処理
	virtual void Hit(ObjectBase* hit_object)override
	{
		//自身が生きてる且つプレイヤーではなく、相手が生きていて、攻撃とアイテム以外なら
		if (!(this->death_flg) && 
			this->object_type != ePLAYER &&
			!hit_object->GetDeathFlg() &&
			hit_object->GetObjectType() != ObjectList::eATTACK &&
			hit_object->GetObjectType() != ObjectList::eCOIN &&
			hit_object->GetObjectType() != ObjectList::eHEAL &&
			hit_object->GetObjectType() != ObjectList::eMAGNET)
		{
			//当たらなくなるまで繰り返す
			while (this->CheckHit(hit_object))
			{
				//オブジェクトを押す
				Push(hit_object);
			}
		}
	}

	virtual void Damage(float _value, Vector2D _attack_loc,int _knock_back = KNOCK_BACK)override
	{
		//自身のHPが1以上ならこの処理
		if (hp > 0)
		{

			hp -= _value;
			//HPが0より小さくなったら0にする
			if (hp < 0)hp = 0;

			//ダメージ量が自身の最大HPより大きければ、ダメージは最大HPとする
			if (_value > max_hp)
			{
				_value = max_hp;
			}
			//減少アニメーション
			hpbar_move += _value * (HPBAR_SIZE / max_hp);

			//ダメージSE
			PlaySoundMem(damage_se, DX_PLAYTYPE_BACK);

			//ノックバック処理

			//ダメージ源の中心座標からノックバック方向を求める
			double radian = atan2(_attack_loc.y - this->location.y, _attack_loc.x - this->location.x);
			velocity.x -= (_knock_back * cos(radian));
			velocity.y -= (_knock_back * sin(radian));

			//死
			if (hp <= 0)
			{
				this->death_flg = true;
				//アニメーション位置をリセット
				image_num = 0;
				anim_end_flg = false;
			}
		}
	}

	//移動処理
	void Move()
	{
		//減速の実行
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//移動前の座標格納
		old_location = location;
		//移動の実行
		location += velocity;

		//壁に当たっていたら前の座標に戻す
		//左端
		if (location.x - (box_size.x/2) <= -STAGE_SIZE)
		{
			location.x = -STAGE_SIZE + (box_size.x / 2);
		}
		//右端
		if (location.x + (box_size.x / 2) > STAGE_SIZE)
		{
			location.x = STAGE_SIZE - (box_size.x / 2);
		}
		//上端
		if (location.y - (box_size.y / 2) <= -STAGE_SIZE)
		{
			location.y = -STAGE_SIZE + (box_size.y / 2);
		}
		//下端
		if (location.y + (box_size.y / 2) > STAGE_SIZE)
		{
			location.y = STAGE_SIZE - (box_size.y / 2);
		}
	}

	//１フレーム前の座標を取得する
	Vector2D GetOldLocation()const { return old_location; }
};

