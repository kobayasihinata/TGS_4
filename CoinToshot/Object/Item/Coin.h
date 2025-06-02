#pragma once
#include "../Base/ActorBase.h"
#include "../../Scene/Camera/Camera.h"
class InGameScene;

class Coin :
	public ActorBase
{
private:
	InGameScene* ingame;		//���݂̃V�[���̃|�C���^��ۑ�
	class Camera* camera;		//�J�����|�C���^�i�[(�v���C���[�̏����󂯎�邽��)

	int color_r;
	int color_g;
	int color_b;
	//����
	int get_se;				//�R�C���l��SE

public:
	float add_num;	//�R�C���ǉ���
	bool check_flg;	//�R�C�������̂��邩����x�������f����p	

public:
	//�R���X�g���N�^
	Coin(InGameScene* _ingame,Vector2D _init_velocity = 0.f);
	//�f�X�g���N�^
	~Coin();
	//����������
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//�I��������
	void Finalize()override;
	//�X�V����
	void Update()override;
	//�`�揈��
	void Draw()const override;
	//�����蔻�肪��������̏���
	void Hit(ObjectBase* hit_Object)override;
	//�_���[�W����
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override {};
};

