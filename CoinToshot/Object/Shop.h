#pragma once
#include "Base/ObjectBase.h"

#define SHOP_COOLDOWN    600	//再びショップを使えるようになる時間
#define SHOP_ITEM_WIDTH	 240	//ショップUIのアイテム一つ辺りの幅
#define SHOP_ITEM_HEIGHT 240	//ショップUIのアイテム一つ辺りの高さ
#define START_ANIM_TIME  30		//アニメーション時間
#define END_ANIM_TIME    30		//アニメーション時間
#define ITEM_NUM         6		//購入できるアイテムの総数

struct ItemList
{
	char name[256];		//アイテム名
	int text_color;		//文字色
	char sub_text[256];	//説明
	int subtext_color;	//説明の文字色
	int price;			//値段
	int buy_max;		//購入上限
};

static ItemList item_list[ITEM_NUM]
{
	{"回復",  0x00ff00,"HPを回復できる",    0x0aa00,  50,5},
	{"磁石",  0x00ffff,"コインを引き寄せる",0x00aaaa, 50,3},
	{"強化弾",0xffaa00,"ほとんどの敵を一撃",0xaa5500, 50,1}, 
	{"追尾弾",0xff00ff,"オートエイム",		0xaa00aa, 50,1}, 
	{"爆発弾",0xff2222,"広範囲の爆発",		0xaa0000, 80,1},
	{"最強弾",0x000000,"つよい",			0x000000,150,1},
};

//一フレーム毎の位置
static int anim_move[START_ANIM_TIME]
{
	1150,1100,1050,1000, 950, 900, 860, 820, 780, 740,
	 700, 670, 640, 610, 580, 550, 530, 510, 490, 470,
	 450, 440, 430, 410, 390, 385, 380, 385, 390, 400
};

class InGameScene;

class Shop :
	public ObjectBase
{
private:
	class Camera* camera;		//カメラ格納
	InGameScene* ingame;		//現在のシーンのポインタを保存

	bool can_shop_flg;			//プレイヤーがショップと被って居るか確認
	bool shop_flg;				//ショップ画面を表示するか判断
	int shop_cd;				//再びショップを使えるようになる時間測定

	int shop_cursor;			//購入アイテムを選ぶ
	int old_shop_cursor;		//ひとつ前に選ばれていた購入アイテム
	Vector2D shop_ui_loc;		//ショップUIの座標
	Vector2D shop_ui_size;		//ショップUI一項目辺りの大きさ
	int shop_ui_span;			//ショップUI一項目間の間隔
	int buy_count[ITEM_NUM];	//購入回数を記録
	int item_impact[ITEM_NUM];	//購入不可時の振動

	bool start_anim;			 //ショップを開いたアニメーション
	int start_anim_timer;		 //アニメーション測定
	bool end_anim;				 //ショップを閉じたアニメーション
	int end_anim_timer;			 //アニメーション測定

	int item_image[ITEM_NUM][2];	//表示画像格納
	std::vector<int> shop_image;	//ショップ画像

	int start_se;					//入店時SE
	int cursor_se;					//カーソルSE
	int button_se;					//ボタン効果音
	int cancel_se;					//キャンセル音
	int back_se;					//戻る音
	int throw_se;					//投げるSE

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

