#pragma once

static enum TutoType
{
	tRule=0,		//ルール説明
	tMove,			//移動方法説明
	tAim,			//照準カーソル説明
	tAttack,		//攻撃説明
	tBulletChange,	//弾変更説明
	tEnemyBullet,	//敵の攻撃説明
	tSlot,			//スロット説明
	tNone			//チュートリアルをしていない時用
};

#define TUTO_NUM 8