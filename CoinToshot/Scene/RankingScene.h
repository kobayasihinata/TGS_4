#pragma once
#include "SceneBase.h"

static int rank_color[10]
{
	0xffff00,
	0xdddd00,
	0xbbbb00,
	0x666600,
	0x666600,
	0x666600,
	0x666600,
	0x666600,
	0x666600,
	0x666600,
};
class RankingScene : public SceneBase
{
private:
	int ranking_bgm;	
public:
	RankingScene();
	virtual ~RankingScene();

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
};



