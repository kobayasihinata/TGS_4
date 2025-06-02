#include "ObjectManager.h"
#include "Base/ShapeCollider.h"
#include "../Scene/InGameScene.h"

void ObjectManager::Initialize(InGameScene* _ingame)
{
	camera = Camera::Get();
	ingame = _ingame;

	ResourceManager* rm = ResourceManager::GetInstance();
	std::vector<int>tmp;
	tmp = rm->GetImages("Resource/Images/UI/button1.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);
	tmp = rm->GetImages("Resource/Images/UI/button2.png", 26, 7, 4, 40, 40);
	button_image.push_back(tmp);

	frame = 0;
	b_anim = 0;
}

void ObjectManager::Finalize()
{

}

void ObjectManager::Update()
{
	//フレーム測定
	if (++frame > 6000)
	{
		frame = 0;
	}
	//アニメーション更新
	if (frame % 10 == 0)
	{
		if (++b_anim > 1)
		{
			b_anim = 0;
		}
	}

	//オブジェクト配列に追加する処理
	for (const auto& create_object : create_object)
	{
		create_object.object->Initialize(this, create_object.object_num, create_object.init_location, create_object.init_size,create_object.init_radius);
		object_list.push_back(create_object.object);
		//煙エフェクト生成
		CreateEffect(elSmoke, create_object.init_location, true,0x000000, 30);
	}

	//追加したオブジェクトは消去
	create_object.clear();

	//オブジェクト配列から削除する処理
	for (const auto& delete_object : delete_object)
	{
		for (auto it = object_list.begin(); it != object_list.end();)
		{
			if (*it == delete_object)
			{

				it = object_list.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	//削除したオブジェクトは消去
	delete_object.clear();

	//オブジェクト更新処理
	for (const auto& object_list : object_list)
	{
		object_list->SetLocalLocation(camera->GetCameraLocation());	//ここでカメラ座標を渡せるようにする
		object_list->Update();
	}


	//エフェクト配列に追加する処理
	for (const auto& create_effect : create_effect)
	{
		create_effect.effect->Initialize(this, create_effect.init_location, create_effect.front_flg, create_effect.timer, create_effect.anim_span);
		effect_list.push_back(create_effect.effect);
	}

	//追加したオブジェクトは消去
	create_effect.clear();

	//エフェクト配列から削除する処理
	for (const auto& delete_effect : delete_effect)
	{
		for (auto it = effect_list.begin(); it != effect_list.end();)
		{
			if (*it == delete_effect)
			{

				it = effect_list.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	//削除したオブジェクトは消去
	delete_effect.clear();

	//エフェクト更新処理
	for (const auto& effect_list : effect_list)
	{
		effect_list->SetLocalLocation(camera->GetCameraLocation());	//ここでカメラ座標を渡せるようにする
		effect_list->Update();
	}


	//当たり判定処理
	ObjectHitCheck();
}

void ObjectManager::Draw()const
{
	std::vector<ObjectBase*> enemy_list;	//敵格納
	std::vector<ObjectBase*> slot_list;		//スロット格納
	std::vector<EffectBase*> front_effect;	//オブジェクトの後ろに描画するエフェクト格納

	//オブジェクトの後ろに描画するエフェクト
	for (const auto& effect : effect_list)
	{
		if (!effect->GetFrontFlg())
		{
			effect->Draw();
		}
		else
		{
			front_effect.push_back(effect);
		}
	}

	//描画
	for (const auto& object : object_list)
	{
		//画面内にあるオブジェクトだけ描画
		if (CheckInScreen(object, 50))
		{
			//敵だけ後から描画（見やすさのために）
			if (object->IsEnemy())
			{
				enemy_list.push_back(object);
			}
			else if (object->GetObjectType() == eSLOT)
			{
				slot_list.push_back(object);
			}
			else
			{
				object->Draw();
			}
		}
	}

	//敵描画
	for (const auto& enemy : enemy_list)
	{
		enemy->Draw();
	}

	//スロット描画
	for (const auto& slot : slot_list)
	{
		slot->Draw();
	}

	//オブジェクトの前に描画するエフェクト
	for (const auto& effect : front_effect)
	{
		effect->Draw();
	}
}

void ObjectManager::CreateObject(int object_type, Vector2D init_location, Vector2D init_size, float init_radius, Vector2D init_velocity)
{
	switch (object_type)
	{
	case ObjectList::ePLAYER:
		create_object.push_back(ObjectInitData{ new Player(),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eENEMY1:
		create_object.push_back(ObjectInitData{ new Enemy1(),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eENEMY2:
		create_object.push_back(ObjectInitData{ new Enemy2(),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eENEMY3:
		create_object.push_back(ObjectInitData{ new Enemy3(),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eENEMY4:
		create_object.push_back(ObjectInitData{ new Enemy4(ingame),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eENEMY5:
		create_object.push_back(ObjectInitData{ new Enemy5(),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eCOIN:
		create_object.push_back(ObjectInitData{ new Coin(ingame,init_velocity),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eHEAL:
		create_object.push_back(ObjectInitData{ new Heal(ingame),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eBLOCK:
		create_object.push_back(ObjectInitData{ new Block(),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eSLOT:
		create_object.push_back(ObjectInitData{ new Slot(ingame),object_type,init_location,init_size,init_radius });
		break;
	case ObjectList::eMAGNET:
		create_object.push_back(ObjectInitData{ new Magnet(),object_type,init_location,init_size,init_radius });
		break;
	default:
		break;
	}
}
void ObjectManager::CreateObject(ObjectData _data)
{
	CreateObject(_data.type, _data.loc, _data.size, _data.radius);
}

void ObjectManager::DeleteObject(ObjectBase* _delete_object)
{
	delete_object.push_back(_delete_object);
}

void ObjectManager::DeleteAllObject()
{
	//全オブジェクトを消去
	for (const auto object : object_list)
	{
		delete_object.push_back(object);
	}
}

void ObjectManager::CreateEffect(int object_type, Vector2D init_location, bool _front_flg, int _color, int _timer, int _anim_span)
{
	switch (object_type)
	{
	case EffectList::elRipples:
		create_effect.push_back(EffectInitData{ new Ripples(_color),init_location, _front_flg,_timer, _anim_span});
		break;
	case EffectList::elSmoke:
		create_effect.push_back(EffectInitData{ new Smoke(),init_location, _front_flg,_timer, _anim_span });
		break;
	case EffectList::elShine:
		create_effect.push_back(EffectInitData{ new Shine(_color),init_location, _front_flg,_timer, _anim_span });
		break;
	default:
		break;
	}
}

void ObjectManager::DeleteEffect(EffectBase* _delete_effect)
{
	delete_effect.push_back(_delete_effect);
}

void ObjectManager::CreateAttack(BulletData _bullet_data)
{
	create_object.push_back(
		ObjectInitData{
			new Attack(_bullet_data),
			eATTACK,
			_bullet_data.location,
			Vector2D{_bullet_data.radius * 2,_bullet_data.radius * 2},
			_bullet_data.radius
		}
	);
}

void ObjectManager::ObjectHitCheck()
{
	for (const auto& object1 : object_list)
	{
		for (const auto& object2 : object_list)
		{
			//オブジェクトのどちらかが画面外にあればスキップ
			if (CheckInScreen(object2, 50) && CheckInScreen(object1,50) && object1 != object2 && object1->CheckHit(object2))
			{
				object1->Hit(object2);
			}
		}
	}
}

std::vector<ObjectBase*> ObjectManager::GetObjectList()const
{
	return object_list;
}

void ObjectManager::Result(int _delay)
{
	ingame->ChangeResult(_delay);
}

bool ObjectManager::CheckInScreen(ObjectBase* _object, int space)const
{
	//カメラ座標ーゆとりより左側に居たらアウト
	if (_object->GetLocation().x + (_object->GetSize().x / 2) < camera->GetCameraLocation().x - space)return false;
	//カメラ座標＋スクリーン幅＋ゆとりより右側に居たらアウト
	if (_object->GetLocation().x - (_object->GetSize().x / 2) > camera->GetCameraLocation().x + SCREEN_WIDTH + space)return false;
	//カメラ座標ーゆとりより上側に居たらアウト
	if (_object->GetLocation().y + (_object->GetSize().y / 2) < camera->GetCameraLocation().y - space)return false;
	//カメラ座標＋スクリーン高さ＋ゆとりより下側に居たらアウト
	if (_object->GetLocation().y - (_object->GetSize().y / 2) > camera->GetCameraLocation().y + SCREEN_HEIGHT + space)return false;
	//画面内
	return true;
}

void ObjectManager::DrawButton(Vector2D _location,int _button)const
{
	DrawGraphF(_location.x, _location.y, button_image[b_anim][_button], true);
}