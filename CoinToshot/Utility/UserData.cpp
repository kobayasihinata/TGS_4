#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"

RankingData UserData::ranking_data[RANKING_DATA]{ 0 };	//�����L���O�f�[�^�i�[

float UserData::player_hp = DEFAULT_HP;			//�̗�
int UserData::coin = 10;						//�X�R�A
int UserData::timer = DEFAULT_TIMELIMIT;		//�c�莞��
bool UserData::is_clear = true;					//false=�Q�[���I�[�o�[ true=�Q�[���N���A
bool UserData::is_gamestop = false;				//false=�Q�[���� true=�Q�[����~
int UserData::invincible = 0;					//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�
int UserData::bullet_type = 0;				//�I�𒆂̒e�̎��
bool UserData::attraction_flg = false;
void UserData::ReadRankingData()
{
	const char* a = "Resource/Data/CTs_RankingData.txt";

	std::ifstream file(a);
	//�t�@�C�����ǂݍ��߂Ă����Ȃ�
	if (file)
	{
		RankingData data = { 0 };
		//�����L���O�f�[�^�ǂݍ���
		for (int i = 0; i < RANKING_DATA; i++)
		{
			file >> ranking_data[i].num;
			file >> ranking_data[i].name;
			file >> ranking_data[i].coin;
		}
	}
}

void UserData::WriteRankingData()
{
	const char* a = "Resource/Data/CTs_RankingData.txt";

	std::ofstream file(a);
	//�t�@�C�����ǂݍ��߂Ă����Ȃ�
	if (file)
	{
		//�����L���O�f�[�^�ǂݍ���
		for (int i = 0; i < RANKING_DATA; i++)
		{
			file << ranking_data[i].num << " ";
			file << ranking_data[i].name.c_str() << " ";
			file << ranking_data[i].coin << "\n";
		}
	}
}
