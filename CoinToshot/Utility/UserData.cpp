#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"


std::vector<RankingData> UserData::ranking_data{ 0 };	//�����L���O�f�[�^�i�[

Vector2D UserData::spawn_loc[10] = { 0,0 };		//���X�|�[������ʒu
float UserData::player_hp = DEFAULT_HP;			//�̗�
int UserData::coin = 10;						//�X�R�A
int UserData::timer = DEFAULT_TIMELIMIT;		//�c�莞��
bool UserData::is_clear = true;					//false=�Q�[���I�[�o�[ true=�Q�[���N���A
int UserData::invincible = 0;					//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�

void UserData::ReadRankingData()
{
	const char* a = "Resource/UserData/CTs_RankingData.txt";

	std::ifstream file(a);
	//�t�@�C�����ǂݍ��߂Ă����Ȃ�
	if (file)
	{
		RankingData data = { 0 };
		//�t�@�C���̍Ō�܂œǂݍ���
		while(!file.eof()){
			file >> data.name;
			file >> data.coin;
			//�i�[
			ranking_data.push_back(data);
		}
	}
}

void UserData::WriteRankingData()
{
	const char* a = "Resource/UserData/CTs_RankingData.txt";

	std::ofstream file(a);
	//�t�@�C�����ǂݍ��߂Ă����Ȃ�
	if (file)
	{
		//�����L���O�f�[�^�z����f�[�^����������
		for (const auto& ranking_data : ranking_data)
		{
			file << ranking_data.name;
			file << ranking_data.coin;
			file << "\n";
		}
	}
}
