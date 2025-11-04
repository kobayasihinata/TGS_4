#pragma once

#include "SceneBase.h"
#define END_TIMER 1200	//何秒でゲームが終了するか
#define IMAGE_LINE_NUM 14
#define SOUND_LINE_NUM 10

//表示文字
static char image_cledit[IMAGE_LINE_NUM][256]
{
	"画像素材",
	"プレイヤー、敵画像：craftpix.net(https://craftpix.net/)"
	" ",
	"Free Golem Chibi 2D Game Sprites",
	" ",
	"Free Chibi Skeleton Crusader Character Sprites",
	" ",
	"Free Chibi Dark Oracle Character Sprites",
	" ",
	"Free Simple Platformer Game Kit Pixel Art",
	" ",
	"背景、アイテム素材 : ichi.io(https://itch.io/) ",
	"craftpix.net(https://craftpix.net/)",
	" ",
	"Basic Tileset and Asset Pack 32x32 Pixels",
};
static char sound_cledit[SOUND_LINE_NUM][256]
{
	"音声素材",
	"BGM : DOVA-SYNDROME  (https://dova-s.jp/)",
	" ",
	"タイトル & クレジットBGM  : 秦暁「Colorful Cheerful Jelly Beans」",
	"ゲーム中BGM  : Tinymemory「Rail_train」",
	"リザルト & ランキングBGM : Tinymemory「Sweet smelling flower」",
	" ",
	"効果音: OtoLogic (https://otologic.jp/)",
	"ポケットサウンド (https://pocket-se.info/)",
	"効果音ラボ (https://soundeffect-lab.info/)",
};
class EndScene : public SceneBase
{
private:
	bool release_button;	//Aボタンを離したか判断

	int button_se;			//ボタンSE
public:
	EndScene();
	virtual ~EndScene();

	//初期化処理
	virtual void Initialize()override;

	//更新処理
	//引数：１フレーム当たりの時間
	//戻り値：次のシーンタイプ
	virtual eSceneType Update(float _delta)override;

	//描画処理
	virtual void Draw()const override;


	//終了時処理
	virtual void Finalize() override;

	//現在のシーンタイプ(オーバーライド必須)
	virtual eSceneType GetNowSceneType()const override;

	//クレジット描画
	void DrawCledit(float _loc_y)const;
};

