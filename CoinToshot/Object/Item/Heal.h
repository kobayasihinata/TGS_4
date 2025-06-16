#pragma once
#include "../Base/ActorBase.h"
#include <vector>

class InGameScene;

class Heal :
	public ActorBase
{
private:
	InGameScene* ingame;		//���݂̃V�[���̃|�C���^��ۑ�

	float heal_num;	//�񕜗�

	std::vector<int> shine_image;	//�I�[���摜�i�[
	int now_shine_image;			//���ݕ`��I�[���摜

	int heal_se;					//��SE
public:
	//�R���X�g���N�^
	Heal(InGameScene* _ingame, Vector2D _init_velocity);
	//�f�X�g���N�^
	~Heal();
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

