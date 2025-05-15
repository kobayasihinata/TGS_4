#pragma once
#include "../../Utility/Vector2D.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/DebugInformation.h"
#include <vector>
#include "../../Utility/common.h"

class ObjectManager;

class EffectBase
{
protected:
	ObjectManager* manager;

	Vector2D location;					//絶対座標
	Vector2D local_location;			//相対座標

	std::vector<std::vector<int>> animation_image;	//アニメーション画像格納
	int image;							//現在描画する画像
	Vector2D image_shift;				//画像の位置調整用

	int timer;							//時間測定
	int anim_span;						//アニメーション間隔

public:
	//コンストラクタ
	EffectBase() = default;

	//デストラクタ	
	virtual ~EffectBase() = default;

	//初期化処理
	virtual void Initialize(ObjectManager* _manager, Vector2D init_location,int _timer, int _anim_span) {

		location = init_location;
		local_location = 0;
		manager = _manager;
		//初期化				
		image = 0;
		image_shift = 0;
		timer = _timer;
		anim_span = _anim_span;
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
			DrawRotaGraphF(local_location.x, local_location.y, 1.f, 1.f, image, TRUE);
		}
		else
		{
			DrawStringF(local_location.x, local_location.y, "画像無し", 0x00ff00);
		}
	}
};
