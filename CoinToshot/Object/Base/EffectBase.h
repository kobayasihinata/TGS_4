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

	std::vector<int> animation_image;	//アニメーション画像格納
	int image;							//現在描画する画像
	Vector2D image_shift;				//画像の位置調整用
	int now_anim;						//現在描画中の画像位置

	int init_timer;						//タイマー初期値
	int timer;							//時間測定
	int anim_span;						//アニメーション間隔
	bool front_flg;						//オブジェクトより前に描画するか判断

public:
	//コンストラクタ
	EffectBase() = default;

	//デストラクタ	
	virtual ~EffectBase() = default;

	//初期化処理
	virtual void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span,Vector2D init_velocity) {

		location = init_location;
		local_location = 0;
		manager = _manager;
		//初期化				
		image = 0;
		image_shift = 0;
		now_anim = 0;
		timer = _timer;
		anim_span = _anim_span;
		front_flg = _front_flg;
	}

	//終了時処理
	virtual void Finalize() = 0;

	//更新処理(画像エフェクトなら__superする　Dxlibの関数でエフェクトを作るなら__superしない)
	virtual void Update()
	{
		//アニメーションの切り替え速度を、画像枚数とエフェクト時間から割り出す
		int r = init_timer / (animation_image.size() - 1);
		if (r <= 0)r = 1;
		if (timer % r == 0)
		{
			if (now_anim >= animation_image.size())
			{
				image = 0;
			}
			else
			{
				image = animation_image[now_anim++];
			}
		}

	}

	//描画処理(画像エフェクトなら__superする　Dxlibの関数でエフェクトを作るなら__superしない)
	virtual void Draw()const
	{
		//画像描画
		if (image != 0)
		{
			DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0.f, image, TRUE);
		}
	}

	//オブジェクトの後ろに描画するか取得
	bool GetFrontFlg()const
	{
		return front_flg;
	}

	//ローカル座標の設定
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
};
