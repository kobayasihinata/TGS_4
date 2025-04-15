#pragma once
#include "../../Utility/Vector2D.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/DebugInformation.h"
#include "ObjectList.h"
#include "ShapeCollider.h"
#include <vector>

class ObjectManager;

#define DEFAULT_ANIM_SPAN 20	//アニメーション周期

class ObjectBase : public ShapeCollider
{
protected:
	ObjectManager* manager;
	int object_type;					//オブジェクトの種類(順番はマネージャークラスのObjectList)

	std::vector<int> animation_image;	//アニメーション画像格納
	int image;							//現在描画する画像
	int anim_timer;						//アニメーション画像切り替え用タイマー
	int anim_span;						//アニメーション周期保存用
	int image_num;						//描画画像の配列上の位置格納
	Vector2D image_shift;				//画像の位置調整用

public:
	bool hit_flg[4];					//辺ごとの当たり判定格納

public:
	//コンストラクタ
	ObjectBase() = default;
	//デストラクタ	
	virtual ~ObjectBase() = default;
	//初期化処理
	virtual void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f) {
		
		__super::Initialize(init_location ,init_size);

		manager = _manager;
		object_type = _object_type;

		//初期化
		anim_span = DEFAULT_ANIM_SPAN;	//アニメーション周期保存用
		image_num = 0;					//描画画像の配列上の位置格納
	}
	//終了時処理
	virtual void Finalize() = 0;
	//更新処理
	virtual void Update() = 0;
	//描画処理
	virtual void Draw()const
	{
		//画像描画
		DrawGraphF(local_location.x, local_location.y, image, false);
#ifdef _DEBUG
		//当たり判定の描画
		Vector2D::DrawBoxV2(local_location, local_location + box_size, 0xff0000, false);
#endif // _DEBUG
	}
	//ローカル座標の設定
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
	//当たり判定が被った時の処理
	virtual void Hit(ObjectBase* hit_Object) = 0;
	//ダメージ処理
	virtual void Damage(float _value, Vector2D _attack_loc) = 0;
	//オブジェクトの種類取得
	int GetObjectType()const { return object_type; }

	//アニメーション処理(基本的なループ用)
	void Animation()
	{
		//指定の時間に達したら
		if (++anim_timer > anim_span)
		{
			//タイマーリセット
			anim_timer = 0;
			//画像の総数を上回ったら最初の画像に戻す
			if (++image_num > (animation_image.size() - 1))
			{
				image_num = 0;
			}
			//アニメーション画像を切り替える
			image = animation_image[image_num];
		}
	}
};
