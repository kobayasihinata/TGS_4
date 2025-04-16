#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"

ObjectData UserData::old_data;
int UserData::width_num[10];						//ステージの横幅
int UserData::height_num[10];						//ステージの縦幅
std::vector<ObjectData> UserData::stage_data[10];	//ステージのブロック情報
int UserData::stage_num = 0;								//ステージの数
int UserData::now_stage = 0;								//現在のステージ数

Vector2D UserData::spawn_loc[10] = { 0,0 };		//リスポーンする位置
float UserData::player_hp = DEFAULT_HP;			//体力
int UserData::coin = 0;						//スコア
int UserData::life = DEFAULT_LIFE;				//残機
int UserData::timer = DEFAULT_TIMELIMIT;		//残り時間
bool UserData::is_clear = true;					//false=ゲームオーバー true=ゲームクリア
int UserData::invincible = 0;					//この数値が０以上なら、その時間だけプレイヤーを無敵にして、0に戻す

void UserData::ReadData()
{
	//const char* a = "Resource/UserData/StageDataOne.txt";
	//char s;	//文字格納用
	//std::ifstream file(a);
	//std::streampos position;	//文字位置保存
	//bool seekg_flg = false;		//測定後の戻る処理をするか

	////ファイルが読み込めていたなら
	//if (file)
	//{
	//	do {
	//		//ステージの幅と高さを取得
	//		file >> width_num[stage_num];
	//		file >> height_num[stage_num];
	//		//仮格納用変数定義
	//		ObjectData i;
	//		do {
	//			//各ステージ一回目の読み込み以外は一つ戻す
	//			if (seekg_flg)
	//			{
	//				//一つ戻る
	//				file.seekg(position);
	//			}
	//			else
	//			{
	//				seekg_flg = true;
	//			}
	//			file >> i.type;
	//			file >> i.loc.x;
	//			file >> i.loc.y;
	//			file >> i.size.x;
	//			file >> i.size.y;
	//			//全く同じ情報を持つオブジェクトの追加をしない
	//			if (old_data.type != i.type ||
	//				old_data.loc != i.loc ||
	//				old_data.size != i.size)
	//			{
	//				stage_data[stage_num].push_back(i);
	//			}
	//			//プレイヤー初期位置を保存
	//			if (i.type == ObjectList::PLAYER)spawn_loc[stage_num] = i.loc;
	//			old_data = i;
	//			//読み込み位置を保存
	//			position = file.tellg();
	//			//次の文字を読み込み
	//			file >> s;
	//			//読み込んだ文字が区切りの合図以外なら続行
	//		} while (s != '-' && !file.eof());

	//		//各ステージ一回目の戻す処理リセット
	//		seekg_flg = false;
	//		//ステージの数に加算
	//		stage_num++;
	//		//ファイルが終わるまで続行
	//	} while (!file.eof());

	//}
}

void UserData::WriteData()
{
	//const char* a = "Resource/UserData/StageDataOne.txt";

	//std::ofstream file(a);

	////ファイルが読み込めていたなら
	//if (file)
	//{
	//	for (int i = 0; i < stage_num; i++)
	//	{
	//		//ステージの幅と高さを保存
	//		file << width_num[i] << "\n";
	//		file << height_num[i] << "\n";

	//		//ステージ情報を保存
	//		for (const auto stage_data : stage_data[i])
	//		{
	//			file << stage_data.type << " "
	//				<< (int)stage_data.loc.x << " "
	//				<< (int)stage_data.loc.y << " "
	//				<< (int)stage_data.size.x << " "
	//				<< (int)stage_data.size.y << "\n";
	//		}
	//		//最後の終了合図文字は保存しない
	//		if (i < stage_num - 1)
	//		{
	//			file << "-" << "\n";
	//		}
	//	}
	//}
}
