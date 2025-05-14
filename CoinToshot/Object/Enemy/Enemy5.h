#pragma once
#include "../Base/EnemyBase.h"
#include <vector>
using namespace std;

struct BlockData {
	int block_anim_timer;	//�h��G�t�F�N�g�A�j���[�V����
	Vector2D attack_loc;	//�A�j���[�V�����p �U���̍��W

	bool operator ==  (const BlockData _data)
	{
		if (this->block_anim_timer != _data.block_anim_timer)return false;
		if (this->attack_loc != _data.attack_loc)return false;
		return true;
	}
};
class Enemy5 :
	public EnemyBase
{
private:
	std::vector<BlockData> block_data;	//�U����h�������ɃG�t�F�N�g���o���p
	std::vector<BlockData> delete_block_data;	//�U����h�������ɃG�t�F�N�g���o���p

public:
	//�R���X�g���N�^
	Enemy5();
	//�f�X�g���N�^
	~Enemy5();
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
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;

};

