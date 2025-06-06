#include "Attack.h"
#include "ObjectManager.h"
#include "../Utility/common.h"

Attack::Attack(BulletData _bullet_data)
{
	damage = _bullet_data.damage;
	object = _bullet_data.who;
	time = _bullet_data.delete_time;
	count_up = 0;
	move_velocity.x = _bullet_data.speed * cosf(_bullet_data.b_angle);
	move_velocity.y = _bullet_data.speed * sinf(_bullet_data.b_angle);
	angle = _bullet_data.b_angle;
	hit_count = 0;
	hit_max = _bullet_data.h_count;
	old_hit_object = nullptr;
	bullet_type = _bullet_data.b_type;

	//プレイヤーなら波紋を黄色、敵なら赤にする
	if (object->GetObjectType() == ePLAYER)
	{
		ripple_color = 0xffff00;
	}
	else
	{
		ripple_color = 0xff0000;
	}

	//画像読込
	ResourceManager* rm = ResourceManager::GetInstance();
	//SE読み込み
	shot_se = rm->GetSounds("Resource/Sounds/shot.mp3");
}

Attack::~Attack()
{

}

void Attack::Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location, Vector2D init_size, float init_radius)
{
	__super::Initialize(_manager, _object_type, init_location, init_size, init_radius);

	gauss_image = MakeScreen(box_size.x, box_size.y,TRUE);
}

void Attack::Finalize()
{

}

void Attack::Update()
{
	//一定時間経過したら自身を削除する
	if (++count_up > time)
	{
		//攻撃の消去
		manager->DeleteObject(this);

		//爆発弾処理
		if (bullet_type == BulletType::bExplosion)
		{
			for (float i = 0; i < 6.28; i+=0.53f)
			{
				manager->CreateAttack(GetBulletData(i));
			}
			PlaySoundMem(shot_se, DX_PLAYTYPE_BACK);
		}

		//消滅エフェクト
		manager->CreateEffect(elRipples, this->location, true, ripple_color, 45);
	}

	//波紋を生成
	if (count_up % 5 == 0)
	{
		manager->CreateEffect(elRipples, this->location, false, ripple_color,30);
	}
	//弾の移動
	this->location += move_velocity;
}

void Attack::Draw()const
{
	__super::Draw();

	SetDrawScreen(gauss_image);
	ClearDrawScreen();
	//コイン仮(プレイヤー)
	if (object->GetObjectType() == ePLAYER)
	{
		UserData::DrawCoin({ radius,radius }, radius);
	}
	//コイン仮(敵)
	else
	{
		UserData::DrawCoin({ radius,radius }, radius,255,0,0);
	}
	SetDrawScreen(DX_SCREEN_BACK);

	DrawRotaGraphF(local_location.x, local_location.y,1.f, angle, gauss_image, true);
}

void Attack::Hit(ObjectBase* hit_object)
{
	//当たったオブジェクトがひとつ前の同じものならスキップ
	if (old_hit_object == hit_object)
	{
		return;
	}
	//当たったオブジェクトが死亡演出中ならスキップ
	if (hit_object->GetDeathFlg())
	{
		return;
	}

	//攻撃同士が当たった場合か、落ちてるコインと当たった場合、無視
	if (hit_object->GetObjectType() == eATTACK || hit_object->GetObjectType() == eCOIN)
	{
		return;
	}

	//攻撃したのがプレイヤーではなく、プレイヤーにこの攻撃が当たっているなら
	if (object->GetObjectType() != ePLAYER && hit_object->GetObjectType() == ePLAYER)
	{
		//ダメージ
		hit_object->Damage(damage, this->location);
		//当たった事を保存する
		old_hit_object = hit_object;
		//指定した回数オブジェクトに当たっていれば、攻撃の消去
		if (++hit_count >= hit_max)
		{
			manager->DeleteObject(this);
		}
	}

	//攻撃したのがプレイヤーで、敵に攻撃が当たっているなら
	if (object->GetObjectType() == ePLAYER && hit_object->IsEnemy())
	{
		//ダメージ
		hit_object->Damage(damage, this->location);
		//当たった事を保存する
		old_hit_object = hit_object;
		//指定した回数オブジェクトに当たっていれば、もしくは生きてる敵５に弾が当たったら、攻撃の消去
		if (++hit_count >= hit_max || (hit_object->GetObjectType() == eENEMY5 && !hit_object->GetDeathFlg()))
		{
			manager->DeleteObject(this);
		}
	}
}

BulletData Attack::GetBulletData(float _shot_rad)
{
	BulletData _data;
	_data.damage = damage;
	_data.b_angle = _shot_rad;
	_data.delete_time = time;
	_data.h_count = 100;
	_data.location = this->location;
	_data.radius = 5;
	_data.speed = 10;
	_data.who = object;
	_data.b_type = BulletType::bNormal;

	return _data;
}