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
#include "Block.h"
#include "Slot.h"
#include "../Utility/UserData.h"
#include "../Scene/Camera/Camera.h"
#include "Base/ObjectList.h"
#include "Base/BulletData.h"

#include "Base/EffectBase.h"
#include "Effect/Ripples.h"
#include "Effect/Smoke.h"
#include "Base/EffectList.h"

class InGameScene;

//�I�u�W�F�N�g�̏������ɕK�v�ȃf�[�^
struct ObjectInitData {
	ObjectBase* object;			//��������I�u�W�F�N�g
	int object_num;				//���g��eNum��̉��Ԗڂ�
	Vector2D init_location;		//������΍��W
	Vector2D init_size;			//�����T�C�Y
	float init_radius = 0.f;	//�l�p�̏ꍇ�g��Ȃ�
};

//�G�t�F�N�g�̏������ɕK�v�ȃf�[�^
struct EffectInitData {
	EffectBase* effect;			//��������G�t�F�N�g	
	Vector2D init_location;		//������΍��W
	bool front_flg;				//�I�u�W�F�N�g���O�ɕ`�悷�邩
	int timer;					//�\������
	int anim_span;				//�A�j���[�V�����؂�ւ��Ԋu
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
	std::vector<ObjectInitData>create_object;
	//��������I�u�W�F�N�g
	std::vector<ObjectBase*> delete_object;

	//�G�t�F�N�g�ꗗ
	std::vector<EffectBase*> effect_list;
	//��������G�t�F�N�g
	std::vector<EffectInitData> create_effect;
	//��������G�t�F�N�g
	std::vector<EffectBase*> delete_effect;
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
	void CreateObject(int object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f, float init_radius = 0.f,Vector2D init_velocity = 0.f);
	//�I�u�W�F�N�g�̐���(�I�u�W�F�N�g�f�[�^�p�̃I�[�o�[���[�h)
	void CreateObject(ObjectData _data);
	//�I�u�W�F�N�g�̍폜
	void DeleteObject(ObjectBase* _delete_object);
	//�I�u�W�F�N�g�̍폜�i�S�����j
	void DeleteAllObject();

	//�G�t�F�N�g�̐���
	void CreateEffect(int object_type, Vector2D init_location = 0.0f, bool _front_flg = true, int _timer = 60, int _anim_span = 10);
	//�G�t�F�N�g�̍폜
	void DeleteEffect(EffectBase* _delete_effect);

	//�U���̐��� inti_location=�������W size=�傫�� _object=���̍U���𐶐������I�u�W�F�N�g _time=���蔭������ _angle=�ړ�����
	void CreateAttack(BulletData _bullet_data);
	//�����蔻�菈��
	void ObjectHitCheck();
	//�I�u�W�F�N�g�ꗗ���擾����
	std::vector<ObjectBase*> GetObjectList()const;
	//���U���g��ʑJ��
	void Result(int _delay = 0);
	//��ʓ��ɋ��邩���f(�����͉�ʊO�̂ǂ��܂ł�������)
	bool CheckInScreen(ObjectBase* _object, int space)const;
};

