#pragma once
#include "Base/ObjectBase.h"

#define REEL_NUM 9		//���[���̗v�f��
#define PEKA 50			//����m�� �Z���̂P
#define REEL_WAIT 120	//�҂�����
#define BIG_BONUS 240	//�h���b�v�R�C����

class InGameScene;

//���[���z��
static int ReelArray[3][REEL_NUM]
{
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{9,8,7,6,5,4,3,2,1},
};

class Slot :
	public ObjectBase
{
private:
	InGameScene* ingame;		//���݂̃V�[���̃|�C���^��ۑ�

	int frame;		//�t���[������
	bool can_stop;	//�v���C���[���G��Ă���Ԃ������[���ɐG���
	bool peka_flg;	//�ډ������\�ȏ�Ԃ����f
	bool bet_once;	//�R�C������p
	int reel[3];	//���~�܂��Ă��郊�[��
	int reel_wait;	//���[���̎���
	int stop_reel_num;	//���~�܂��Ă��郊�[���̐�
	int timer;		//�t���[������
	int now_reel[3];	//���w���Ă��郊�[��

	int drop_coin;			//�h���b�v����R�C���̗�
	int drop_coin_count;	//�h���b�v����R�C���̌v��

	Vector2D real_location;	//���[���`��ʒu
	Vector2D real_size;		//���[���`�攠�傫��
	Vector2D hana_location;	//�ؕ`��ʒu

	int reel_se;			//���[����]��SE
	int button_se;			//�{�^����~SE
	int bonus_se;			//�{�[�i�XSE
	bool bonus_se_play_once;//�{�[�i�XSE����񂾂��Đ�
public:
	//�R���X�g���N�^
	Slot(InGameScene* _ingame);
	//�f�X�g���N�^
	~Slot();
	//����������
	void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = Vector2D(0.0f), Vector2D init_size = 40.0f, float init_radius = 0.f)override;
	//�I��������
	void Finalize()override;
	//�X�V����
	void Update()override;
	//�`�揈��
	void Draw()const override;
	//�����蔻�肪��������̏���
	void Hit(ObjectBase* hit_object)override;
	//�_���[�W����
	void Damage(float _value, Vector2D _attack_loc, int _knock_back = KNOCK_BACK)override;
	//������������
	void AutoPlay();
	//�ډ����̏���
	void BonusStop();
	//���̃X���b�g�Ɏg���{�^����������Ă��邩���ׂ�(_button = �������Ƃ��Ă���{�^��)
	bool CheckButton(int _button);
	//�V�������Ă��邩���ׂ�
	bool CheckBigBonus()const;
};

