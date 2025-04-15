#include "ObjectManager.h"
#include "Base/ShapeCollider.h"
#include "../Scene/InGameScene.h"

void ObjectManager::Initialize(InGameScene* _ingame)
{
	camera = Camera::Get();
	ingame = _ingame;
}

void ObjectManager::Finalize()
{

}

void ObjectManager::Update()
{

	//オブジェクト配列に追加する処理
	for (const auto& create_object : create_object)
	{
		create_object.object->Initialize(this, create_object.object_num, create_object.init_location, create_object.init_size);
		object_list.push_back(create_object.object);
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

	//更新処理
	for (const auto& object_list : object_list)
	{
		object_list->SetLocalLocation(camera->GetCameraLocation());	//ここでカメラ座標を渡せるようにする
		object_list->Update();
	}

	//当たり判定処理
	ObjectHitCheck();
}

void ObjectManager::Draw()const
{
	//描画
	for (const auto& object_list : object_list)
	{
		object_list->Draw();
	}
}

void ObjectManager::CreateObject(int object_type, Vector2D init_location, Vector2D init_size)
{
	switch (object_type)
	{
	case ObjectList::ePLAYER:
		create_object.push_back(InitData{ new Player(),object_type,init_location,init_size });
		break;
	case ObjectList::eENEMY1:
		create_object.push_back(InitData{ new Enemy1(),object_type,init_location,init_size });
		break;
	case ObjectList::eENEMY2:
		create_object.push_back(InitData{ new Enemy2(),object_type,init_location,init_size });
		break;
	case ObjectList::eENEMY3:
		create_object.push_back(InitData{ new Enemy3(),object_type,init_location,init_size });
		break;
	case ObjectList::eENEMY4:
		create_object.push_back(InitData{ new Enemy4(),object_type,init_location,init_size });
		break;
	case ObjectList::eENEMY5:
		create_object.push_back(InitData{ new Enemy5(),object_type,init_location,init_size });
		break;
	case ObjectList::eCOIN:
		create_object.push_back(InitData{ new Coin(),object_type,init_location,init_size });
		break;
	//case ObjectList::eHEAL:
	//	create_object.push_back(InitData{ new Heal(),object_type,init_location,init_size });
	//	break;
	//case ObjectList::eBLOCK:
	//	create_object.push_back(InitData{ new Block(),object_type,init_location,init_size });
	//	break;
	//case ObjectList::eSLOT:
	//	create_object.push_back(InitData{ new Slot(),object_type,init_location,init_size });
	//	break;
	default:
		break;
	}
}
void ObjectManager::CreateObject(ObjectData _data)
{
	CreateObject(_data.type, _data.loc, _data.size);
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

void ObjectManager::CreateAttack(Vector2D init_location, Vector2D init_size, ObjectBase* _object, int _time)
{
	create_object.push_back(InitData{ new Attack(_object,_time),eATTACK,init_location,init_size });
}

void ObjectManager::ObjectHitCheck()
{
	for (const auto& object_list1 : object_list)
	{
		for (const auto& object_list2 : object_list)
		{
			if (object_list1 != object_list2 && object_list1->CheckHit(object_list2))
			{
				object_list1->Hit(object_list2);
			}
		}
	}
}

std::vector<ObjectBase*> ObjectManager::GetObjectList()const
{
	return object_list;
}

//次のステージ遷移
void ObjectManager::ChangeNextStage()
{
	ingame->ChangeNextStage();
}

void ObjectManager::Result()
{
	ingame->ChangeResult();
}