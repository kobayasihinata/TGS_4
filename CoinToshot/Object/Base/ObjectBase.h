#pragma once
#include "../../Utility/Vector2D.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/DebugInformation.h"
#include "ObjectList.h"
#include "ShapeCollider.h"
#include <vector>
#include "../../Utility/common.h"

class ObjectManager;


class ObjectBase : public ShapeCollider
{
protected:
	ObjectManager* manager;
	int object_type;					//オブジェクトの種類(順番はマネージャークラスのObjectList)

	std::vector<std::vector<int>> animation_image;	//アニメーション画像格納
	int image;							//現在描画する画像
	int image_line;						//現在描画する画像の行
	int anim_timer;						//アニメーション画像切り替え用タイマー
	int anim_span;						//アニメーション周期保存用
	bool anim_end_flg;					//アニメーション一周終了
	int image_num;						//描画画像の配列上の位置格納
	Vector2D image_shift;				//画像の位置調整用

	bool death_flg;						//死亡時の演出をしているか判断
	int death_timer;					//死亡演出の時間測定
public:
	//コンストラクタ
	ObjectBase() = default;
	//デストラクタ	
	virtual ~ObjectBase() = default;
	//初期化処理
	virtual void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f, float init_radius = 0.f) {
		
		__super::Initialize(init_location ,init_size, init_radius);

		manager = _manager;
		object_type = _object_type;

		//初期化
		anim_span = DEFAULT_ANIM_SPAN;	//アニメーション周期保存用
		anim_end_flg = false;
		image_num = 0;					//描画画像の配列上の位置格納
		image_shift = 0;
		death_flg = false;					
		death_timer = 0;
	}
	//終了時処理
	virtual void Finalize() = 0;
	//更新処理
	virtual void Update() = 0;

	//描画処理
	virtual void Draw()const
	{

		//画像描画
		if (image != 0)
		{
			DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0.f, image, TRUE);
		}
#ifdef _DEBUG
		//当たり判定の描画

		//四角
		if (this->radius <= 0.f)
		{
			Vector2D::DrawBoxV2(local_location-(box_size/2), local_location + (box_size/2), 0xff0000, false);
		}
		//円
		else
		{
			DrawCircleAA(local_location.x, local_location.y, radius, 20, 0xff0000, false);
		}
#endif // _DEBUG
	}

	//当たり判定が被った時の処理
	virtual void Hit(ObjectBase* hit_Object) = 0;
	//ダメージ処理
	virtual void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK) = 0;
	//オブジェクトの種類取得
	int GetObjectType()const { return object_type; }
	//死亡演出中か取得
	bool GetDeathFlg()const { return this->death_flg; }
	//敵オブジェクトか判断
	bool IsEnemy()const
	{
		return object_type == eENEMY1 ||
			   object_type == eENEMY2 ||
			   object_type == eENEMY3 ||
			   object_type == eENEMY4 ||
			   object_type == eENEMY5;
	}
	//ボスオブジェクトか判断
	bool IsBoss()const
	{
		return object_type == eBOSS1 ||
			   object_type == eBOSS2;
	}
	//アニメーション処理(基本的なループ用)
	virtual void Animation()
	{
		//画像が無い場合スキップ
		if (image == 0)return;
		//指定の時間に達したら
		if (++anim_timer > anim_span)
		{
			//タイマーリセット
			anim_timer = 0;

			//アニメーション一周フラグをさげる
			anim_end_flg = false;

			//画像の総数を上回ったら最初の画像に戻す
			if (++image_num > (animation_image[image_line].size() - 1))
			{
				image_num = 0;
				//アニメーションが一周した事を検知
				anim_end_flg = true;
			}

			//アニメーション画像を切り替える
			image = animation_image[image_line][image_num];
		}
	}
};
