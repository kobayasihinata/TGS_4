#pragma once
#include<vector>
#include<map>

#include "Player/Player.h"
#include "Attack.h"
#include "Enemy/Enemy1.h"
#include "Enemy/Enemy2.h"
#include "Enemy/Enemy3.h"
#include "Enemy/Enemy4.h"
#include "Enemy/Enemy5.h"
#include "Item/Coin.h"
#include "../Utility/UserData.h"
#include "../Scene/Camera/Camera.h"
#include "Base/ObjectList.h"
#include "Base/BulletData.h"

class InGameScene;

//�������ɕK�v�ȃf�[�^
struct InitData {
	ObjectBase* object;
	int object_num;
	Vector2D init_location;
	Vector2D init_size;
};

class ObjectManager
{
private:
	//�J�����|�C���^
	class Camera* camera;
	//�Q�[�����C���|�C���^
	InGameScene* ingame;
	//�I�u�W�F�N�g�ꗗ
	std::vector<ObjectBase*> object_list;
	//��������I�u�W�F�N�g
	std::vector<InitData>create_object;
	//��������I�u�W�F�N�g
	std::vector<ObjectBase*> delete_object;
public:
	//����������
	void Initialize(InGameScene* _ingame);
	//�I��������
	void Finalize();
	//�X�V
	void Update();
	//�`��
	void Draw()const;
	//�I�u�W�F�N�g�̐���
	void CreateObject(int object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f);
	//�I�u�W�F�N�g�̐���(�I�u�W�F�N�g�f�[�^�p�̃I�[�o�[���[�h)
	void CreateObject(ObjectData _data);
	//�I�u�W�F�N�g�̍폜
	void DeleteObject(ObjectBase* _delete_object);
	//�I�u�W�F�N�g�̍폜�i�S�����j
	void DeleteAllObject();
	//�U���̐��� inti_location=�������W size=�傫�� _object=���̍U���𐶐������I�u�W�F�N�g _time=���蔭������ _angle=�ړ�����
	void CreateAttack(BulletData _bullet_data);
	//�����蔻�菈��
	void ObjectHitCheck();
	//�I�u�W�F�N�g�ꗗ���擾����
	std::vector<ObjectBase*> GetObjectList()const;
	//���̃X�e�[�W�J��
	void ChangeNextStage();
	//���U���g��ʑJ��
	void Result();

};

