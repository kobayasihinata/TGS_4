#pragma once
#include "SceneBase.h"
#define OPTION_NUM 3	//項目の数

enum OptionItem
{
	oVolume = 0,
	oControl,
	oEnd,
	oNull
};

static char option_text[OPTION_NUM][256] =
{
	"音量設定",
	"操作方法",
	"戻る",
};

class OptionScene : public SceneBase
{
private:
	eSceneType change_scene;	//シーン遷移用

	int cursor;		//大項目選択
	int current_item;	//選択中の項目

	int volume_cursor;	//音量調整のカーソル
	int end_cursor;		//オプション終了の確認画面のカーソル

	int option_bgm; //BGM
	int cursor_se;	//カーソルSE
	int button_se;	//戻るボタン効果音
public:
	OptionScene();
	virtual ~OptionScene();

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

	//音量調整の更新
	void VolumeUpdate();
	//音量調整の描画
	void VolumeDraw()const;

	//操作変更の更新
	void ControlUpdate();
	//操作変更の描画
	void ControlDraw()const;

	//設定終了の更新
	void EndUpdate();
	//設定終了の描画
	void EndDraw()const;
};