#include "UserData.h"
#include <fstream>
//#include <iostream>
#include <string>
#include "../Object/Base/ObjectList.h"
#include "common.h"

ObjectData UserData::old_data;
int UserData::width_num[10];						//�X�e�[�W�̉���
int UserData::height_num[10];						//�X�e�[�W�̏c��
std::vector<ObjectData> UserData::stage_data[10];	//�X�e�[�W�̃u���b�N���
int UserData::stage_num = 0;								//�X�e�[�W�̐�
int UserData::now_stage = 0;								//���݂̃X�e�[�W��

Vector2D UserData::spawn_loc[10] = { 0,0 };		//���X�|�[������ʒu
float UserData::player_hp = DEFAULT_HP;			//�̗�
int UserData::coin = 0;						//�X�R�A
int UserData::life = DEFAULT_LIFE;				//�c�@
int UserData::timer = DEFAULT_TIMELIMIT;		//�c�莞��
bool UserData::is_clear = true;					//false=�Q�[���I�[�o�[ true=�Q�[���N���A
int UserData::invincible = 0;					//���̐��l���O�ȏ�Ȃ�A���̎��Ԃ����v���C���[�𖳓G�ɂ��āA0�ɖ߂�

void UserData::ReadData()
{
	//const char* a = "Resource/UserData/StageDataOne.txt";
	//char s;	//�����i�[�p
	//std::ifstream file(a);
	//std::streampos position;	//�����ʒu�ۑ�
	//bool seekg_flg = false;		//�����̖߂鏈�������邩

	////�t�@�C�����ǂݍ��߂Ă����Ȃ�
	//if (file)
	//{
	//	do {
	//		//�X�e�[�W�̕��ƍ������擾
	//		file >> width_num[stage_num];
	//		file >> height_num[stage_num];
	//		//���i�[�p�ϐ���`
	//		ObjectData i;
	//		do {
	//			//�e�X�e�[�W���ڂ̓ǂݍ��݈ȊO�͈�߂�
	//			if (seekg_flg)
	//			{
	//				//��߂�
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
	//			//�S�������������I�u�W�F�N�g�̒ǉ������Ȃ�
	//			if (old_data.type != i.type ||
	//				old_data.loc != i.loc ||
	//				old_data.size != i.size)
	//			{
	//				stage_data[stage_num].push_back(i);
	//			}
	//			//�v���C���[�����ʒu��ۑ�
	//			if (i.type == ObjectList::PLAYER)spawn_loc[stage_num] = i.loc;
	//			old_data = i;
	//			//�ǂݍ��݈ʒu��ۑ�
	//			position = file.tellg();
	//			//���̕�����ǂݍ���
	//			file >> s;
	//			//�ǂݍ��񂾕�������؂�̍��}�ȊO�Ȃ瑱�s
	//		} while (s != '-' && !file.eof());

	//		//�e�X�e�[�W���ڂ̖߂��������Z�b�g
	//		seekg_flg = false;
	//		//�X�e�[�W�̐��ɉ��Z
	//		stage_num++;
	//		//�t�@�C�����I���܂ő��s
	//	} while (!file.eof());

	//}
}

void UserData::WriteData()
{
	//const char* a = "Resource/UserData/StageDataOne.txt";

	//std::ofstream file(a);

	////�t�@�C�����ǂݍ��߂Ă����Ȃ�
	//if (file)
	//{
	//	for (int i = 0; i < stage_num; i++)
	//	{
	//		//�X�e�[�W�̕��ƍ�����ۑ�
	//		file << width_num[i] << "\n";
	//		file << height_num[i] << "\n";

	//		//�X�e�[�W����ۑ�
	//		for (const auto stage_data : stage_data[i])
	//		{
	//			file << stage_data.type << " "
	//				<< (int)stage_data.loc.x << " "
	//				<< (int)stage_data.loc.y << " "
	//				<< (int)stage_data.size.x << " "
	//				<< (int)stage_data.size.y << "\n";
	//		}
	//		//�Ō�̏I�����}�����͕ۑ����Ȃ�
	//		if (i < stage_num - 1)
	//		{
	//			file << "-" << "\n";
	//		}
	//	}
	//}
}
