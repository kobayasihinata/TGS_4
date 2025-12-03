#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"
#include <vector>

#define BOSS_ANIM 180	//登場アニメーション時間

class BossBase :
	public ActorBase
{
protected:
	class Camera* camera;	//カメラポインタ格納
	float move_speed;		//移動速度
	int hit_damage;			//プレイヤーに与えるダメージ量

	bool anim_once;			//一回だけ実行
	bool anim_flg;			//登場アニメーション中か
	bool anim_impact_flg;	//登場演出を始めるか
	int boss_anim_timer;	//登場アニメーション時間測定

	std::vector<int> aura_image;	//ボスオーラ画像
	int now_aura_image;				//オーラ画像表示位置
	int waves_image;				//波動画像格納
	int boss_gauss_image;			//ボスぼかし画像保存

	int death_se;					//死亡時SE
	int item_spawn_se;				//アイテム生成SE

public:

	BossBase()
	{
		//カメラ取得
		camera = Camera::Get();
		move_speed = 0.f;
		hit_damage = 0;
		anim_once = false;
		anim_flg = true;	//生成された瞬間にアニメーションが開始するので真
		anim_impact_flg = false;
		boss_anim_timer = 0;
		drop_coin = 0;
		drop_coin_count = 0;
		death_se = 0;
		waves_image = MakeScreen(100, 100, true);
		boss_gauss_image = 0;
		MakeDrawWaves();
		//SE読み込み
		ResourceManager* rm = ResourceManager::GetInstance();
		std::vector<int>tmp;
		aura_image = rm->GetImages("Resource/Images/Effect/boss_aura.png", 8, 8, 1, 120, 120);
		death_se = rm->GetSounds("Resource/Sounds/Enemy/death.mp3");
		item_spawn_se = rm->GetSounds("Resource/Sounds/Direction/成功音.mp3");
	}

	virtual void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f, float init_radius = 0.f)override
	{
		__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);
	}

	virtual void Update()override
	{
		__super::Update();

		//登場アニメーション時間測定
		if (anim_flg && ++boss_anim_timer > BOSS_ANIM)
		{
			anim_flg = false;
			anim_impact_flg = false;
		}
		//オーラ更新
		if (frame % 5 == 0)
		{
			if (++now_aura_image >= aura_image.size())
			{
				now_aura_image = 0;
			}
		}
		boss_gauss_image = image;
	}

	virtual void Draw()const override
	{
		DrawRotaGraphF(local_location.x, local_location.y, box_size.x / 40, 0, aura_image[now_aura_image], TRUE);
		//画像描画
		if (image != 0)
		{
			if (move_velocity.x > 0)
			{
				DrawRotaGraphF(local_location.x, local_location.y, box_size.x / 40, 0, image, true, false);
			}
			else
			{
				DrawRotaGraphF(local_location.x, local_location.y, box_size.x / 40, 0, image, true, true);
			}
		}
		if (anim_impact_flg)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 - ((200.f/10.f)* (frame % 10)));
			DrawRotaGraphF(local_location.x, local_location.y, (box_size.x + pow(frame % 10,3)) / 40 , 0, boss_gauss_image, true, false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawWaves(local_location, powf(frame % 30, 2)*2);
			DrawWaves(local_location, powf((frame -10) % 30, 2)*2);
			DrawWaves(local_location, powf((frame -20) % 30, 2)*2);
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
		//hp描画
		DrawFormatStringF(local_location.x, local_location.y - (box_size.y / 2), 0xffffff, "HP:%f", this->hp);
#endif // _DEBUG

	}

	virtual void Hit(ObjectBase* hit_object)override
	{
		__super::Hit(hit_object);

		//自身が生きていて、プレイヤーに当たったらダメージを与える
		if (!this->death_flg &&
			hit_object->GetObjectType() == ePLAYER)
		{
			//ノックバック量を自身の速度に応じて増加させる
			hit_object->Damage(hit_damage, this->location ,10 + fabs(velocity.x) + fabs(velocity.y));
		}

	}

	//移動処理
	void Move()override
	{
		//減速の実行
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//移動前の座標格納
		old_location = location;
		//移動の実行
		location += velocity;
	}

	//プレイヤーに向かって移動する
	void MovetoPlayer()
	{
		//死亡していなければこの処理
		if (!this->death_flg)
		{
			float radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);

			//移動の上限値設定
			if (fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
			if (fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);

			//移動したい方向保存
			move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
		}
	}

	//波動生成
	void MakeDrawWaves()
	{
		SetDrawScreen(waves_image);
		ClearDrawScreen();
		DrawCircle(50, 50, 48, 0xffffff, false);
		SetDrawScreen(DX_SCREEN_BACK);
		GraphFilter(waves_image, DX_GRAPH_FILTER_GAUSS, 8, 300);
	}

	//波動描画 _loc=中心座標 _size=大きさ
	void DrawWaves(Vector2D _loc, float _size)const
	{
		DrawRotaGraphF(_loc.x, _loc.y, _size / 100, 0.f, waves_image, true);
	}
};