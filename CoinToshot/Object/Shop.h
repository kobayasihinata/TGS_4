#pragma once
#include "Base/ObjectBase.h"

#define SHOP_COOLDOWN    300 //再びショップを使えるようになる時間
#define SHOP_ITEM_WIDTH	 195 //ショップUIのアイテム一つ辺りの幅
#define SHOP_ITEM_HEIGHT 195 //ショップUIのアイテム一つ辺りの高さ
#define ITEM_NUM 2			 //購入できるアイテムの総数

struct ItemList
{
	char name[256];	//アイテム名
	char sub_text[256];	//説明
	int price;						//値段
};

static ItemList item_list[ITEM_NUM]
{
	{"回復","HPを回復できる",5},
	{"磁石","コインを引き寄せる",10},
};

class InGameScene;

class Shop :
	public ObjectBase
{
private:
	class Camera* camera;		//カメラ格納
	InGameScene* ingame;		//現在のシーンのポインタを保存

	bool shop_flg;				//ショップ画面を表示するか判断
	int shop_cd;				//再びショップを使えるようになる時間測定

	int shop_cursor;			//購入アイテムを選ぶ

	int item_image[ITEM_NUM][2];	//表示画像格納
	int cursor_se;	//カーソルSE
	int button_se;	//ボタン効果音
	int back_se;	//戻るボタン効果音


public:
	//コンストラクタ
	Shop(InGameScene* _ingame);
	//デストラクタ
	~Shop();
	//初期化処理
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//終了時処理
	void Finalize()override;
	//更新処理
	void Update()override;
	//描画処理
	void Draw()const override;
	//当たり判定が被った時の処理
	void Hit(ObjectBase* hit_object)override;
	//ダメージ処理
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;

	//画像生成
	void CreateItemImage();
	//アイテム購入処理
	void BuyItem(int cursor);
	//画面端のランダムな座標を渡す
	Vector2D GetRandLoc();

	//ショップ開始or終了処理
	void SetShop(bool _flg);
};

