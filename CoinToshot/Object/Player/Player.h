#pragma once
#include <vector>
#include "../../Utility/Vector2D.h"
#include "../Base/ActorBase.h"
#include "../../Scene/Camera/Camera.h"
#include "../Base/BulletData.h"

class InGameScene;

class Player :
	public ActorBase
{
private:

	InputKey* k_input;			//���͋@�\�̎擾(�L�[�{�[�h)
	class Camera* camera;		//�J�����|�C���^�i�[(�v���C���[�̏���n�����߂����Ɏ擾)

	//�e�ύX
	int bullet_change_cd;		//�e��ޕύX�N�[���_�E������
	int arrow_image;			//���摜�쐬

	//�R�C�����ˊ֘A
	float shot_rad;				//���ˊp�x
	float old_shot_rad;			//�P�t���[���O�̔��ˊp�x

	//���G�����֘A
	bool inv_flg;				//���G�����f
	int inv_timer;				//���G���ԑ���
	bool damage_flg;			//��e�㖳�G�����f
	int damage_timer;			//��e�㖳�G���ԑ���
	bool damage_stop;			//��e��̓����Ȃ���Ԕ��f

	//����
	int shot_se;				//�e����SE
	int walk_se;				//�ړ�SE
	int cursor_se;				//�Ə�SE
	int not_shoot_se;			//���ˎ��sSE
	int death_se;				//���SSE
public:
	static InGameScene* ingame;		//���݂̃V�[���̃|�C���^��ۑ�

private:
	//�R���X�g���N�^��private�ɂ��邱�ƂŁA
//���N���X�̃����o�֐��ŃC���X�^���X�𐶐��ł��Ȃ��悤�ɂ���
	Player() = default;

	//�R�s�[�K�[�h
	//�N���X�O�ŃC���X�^���X�𐶐����ēn�����Ƃ��ł��Ȃ��悤�ɂ���
	Player(Player& v) = default;
	Player& operator = (const Player& v) = delete;
	~Player() = default;
public:

	//�C���X�^���X���擾���鏈��
	static Player* Get(InGameScene* _ingame);

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="init_location">�������W</param>
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = { PLAYER_WIDTH ,PLAYER_HEIGHT }, float init_radius = 0.f)override;

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
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;

	/// <summary>
	/// ���S����
	/// </summary>
	void Death();

	/// <summary>
	/// �v���C���[���쏈��
	/// </summary>
	void Control();

	/// <summary>
	/// �e�𔭎˂��邽�߂̏����܂Ƃ߂�
	/// </summary>
	/// <returns>�e�𔭎˂��邽�߂̏��</returns>
	BulletData GetBulletData(float _shot_rad);

private:
	/// <summary>
	/// �e��������
	/// </summary>
	/// <param name="_bullet_type">���ʂ̎��</param>
	void ShotBullet();

	/// <summary>
	/// �v���C���[�̒e�̋O����`��
	/// </summary>
	void DrawBulletOrbit()const;

	/// <summary>
	/// �v���C���[�̋O�����`��
	/// </summary>
	void CreateArrowImage()const;
};


