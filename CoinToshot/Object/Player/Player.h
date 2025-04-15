#pragma once
#include <vector>
#include "../../Utility/Vector2D.h"
#include "../Base/ActorBase.h"
#include "../../Scene/Camera/Camera.h"

class Player :
	public ActorBase
{
private:
	class Camera* camera;	//�J�����|�C���^�i�[(�v���C���[�̏���n�����߂����Ɏ擾)
	bool inv_flg;		//���G�����f
	int inv_timer;		//���G���ԑ���
	bool damage_flg;	//��e�㖳�G�����f
	int damage_timer;	//��e�㖳�G���ԑ���
public:

public:
	Player() = default;					//�R���X�g���N�^
	~Player() = default;					//�f�t�H���g�̃f�X�g���N�^���g�p

public:

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="init_location">�������W</param>
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = { PLAYER_WIDTH ,PLAYER_HEIGHT })override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw()const override;

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="hit_Object">���ׂ鑊��</param>
	void Hit(ObjectBase* hit_Object)override;

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="value">�󂯂��_���[�W��</param>
	void Damage(float _value, Vector2D _attack_loc)override;

	/// <summary>
	/// ���S����
	/// </summary>
	void Death();

public:

	/// <summary>
	/// �`��摜���擾
	/// </summary>
	/// <returns>�摜�n���h��</returns>
	int GetImages();
};


