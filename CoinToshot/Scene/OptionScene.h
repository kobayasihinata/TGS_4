#pragma once
#include "SceneBase.h"
#define OPTION_NUM 3	//項目の数
#define VOLUME 5000		//音量0から上げる値

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
	"操作タイプ",
	"戻る",
};

class OptionScene : public SceneBase
{
private:
	eSceneType change_scene;	//シーン遷移用

	int cursor;		//大項目選択
	int current_item;	//選択中の項目

	int volume_cursor;	//音量調整のカーソル
	int se_volume[10];		//SE音量の段階
	int bgm_volume[10];		//BGM音量の段階

	int end_cursor;		//オプション終了の確認画面のカーソル

	int cont_type;	//操作タイプ一時保存用
	bool check_change_flg;	//操作タイプ変更確認

	int option_bgm; //BGM
	int cursor_se;	//カーソルSE
	int button_se;	//ボタン効果音
	int back_se;	//戻るボタン効果音
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
};