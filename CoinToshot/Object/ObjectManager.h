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
#include "Block.h"
#include "Slot.h"
#include "../Utility/UserData.h"
#include "../Scene/Camera/Camera.h"
#include "Base/ObjectList.h"
#include "Base/BulletData.h"

#include "Base/EffectBase.h"
#include "Effect/Ripples.h"
#include "Effect/Smoke.h"
#include "Base/EffectList.h"

class InGameScene;

//オブジェクトの初期化に必要なデータ
struct ObjectInitData {
	ObjectBase* object;			//生成するオブジェクト
	int object_num;				//自身がeNum上の何番目か
	Vector2D init_location;		//初期絶対座標
	Vector2D init_size;			//初期サイズ
	float init_radius = 0.f;	//四角の場合使わない
};

//エフェクトの初期化に必要なデータ
struct EffectInitData {
	EffectBase* effect;			//生成するエフェクト	
	Vector2D init_location;		//初期絶対座標
	bool front_flg;				//オブジェクトより前に描画するか
	int timer;					//表示時間
	int anim_span;				//アニメーション切り替え間隔
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
	std::vector<ObjectInitData>create_object;
	//消去するオブジェクト
	std::vector<ObjectBase*> delete_object;

	//エフェクト一覧
	std::vector<EffectBase*> effect_list;
	//生成するエフェクト
	std::vector<EffectInitData> create_effect;
	//消去するエフェクト
	std::vector<EffectBase*> delete_effect;
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
	void CreateObject(int object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f, float init_radius = 0.f,Vector2D init_velocity = 0.f);
	//オブジェクトの生成(オブジェクトデータ用のオーバーロード)
	void CreateObject(ObjectData _data);
	//オブジェクトの削除
	void DeleteObject(ObjectBase* _delete_object);
	//オブジェクトの削除（全消し）
	void DeleteAllObject();

	//エフェクトの生成
	void CreateEffect(int object_type, Vector2D init_location = 0.0f, bool _front_flg = true, int _timer = 60, int _anim_span = 10);
	//エフェクトの削除
	void DeleteEffect(EffectBase* _delete_effect);

	//攻撃の生成 inti_location=初期座標 size=大きさ _object=この攻撃を生成したオブジェクト _time=判定発生時間 _angle=移動方向
	void CreateAttack(BulletData _bullet_data);
	//当たり判定処理
	void ObjectHitCheck();
	//オブジェクト一覧を取得する
	std::vector<ObjectBase*> GetObjectList()const;
	//リザルト画面遷移
	void Result(int _delay = 0);
	//画面内に居るか判断(引数は画面外のどこまでを許すか)
	bool CheckInScreen(ObjectBase* _object, int space)const;
};

