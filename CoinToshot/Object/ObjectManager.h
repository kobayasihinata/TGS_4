#pragma once
#include<vector>
#include<map>

#include "Player/Player.h"
#include "Attack.h"
#include "Enemy/Enemy1.h"
#include "Enemy/Enemy2.h"
#include "Enemy/Enemy3.h"
#include "Enemy/Enemy4.h"
#include "Enemy/Enemy5.h"
#include "Item/Coin.h"
#include "../Utility/UserData.h"
#include "../Scene/Camera/Camera.h"
#include "Base/ObjectList.h"
#include "Base/BulletData.h"

class InGameScene;

//初期化に必要なデータ
struct InitData {
	ObjectBase* object;
	int object_num;
	Vector2D init_location;
	Vector2D init_size;
};

class ObjectManager
{
private:
	//カメラポインタ
	class Camera* camera;
	//ゲームメインポインタ
	InGameScene* ingame;
	//オブジェクト一覧
	std::vector<ObjectBase*> object_list;
	//生成するオブジェクト
	std::vector<InitData>create_object;
	//消去するオブジェクト
	std::vector<ObjectBase*> delete_object;
public:
	//初期化処理
	void Initialize(InGameScene* _ingame);
	//終了時処理
	void Finalize();
	//更新
	void Update();
	//描画
	void Draw()const;
	//オブジェクトの生成
	void CreateObject(int object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f);
	//オブジェクトの生成(オブジェクトデータ用のオーバーロード)
	void CreateObject(ObjectData _data);
	//オブジェクトの削除
	void DeleteObject(ObjectBase* _delete_object);
	//オブジェクトの削除（全消し）
	void DeleteAllObject();
	//攻撃の生成 inti_location=初期座標 size=大きさ _object=この攻撃を生成したオブジェクト _time=判定発生時間 _angle=移動方向
	void CreateAttack(BulletData _bullet_data);
	//当たり判定処理
	void ObjectHitCheck();
	//オブジェクト一覧を取得する
	std::vector<ObjectBase*> GetObjectList()const;
	//次のステージ遷移
	void ChangeNextStage();
	//リザルト画面遷移
	void Result();

};

