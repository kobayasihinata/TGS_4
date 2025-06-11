#pragma once
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"
#include "TutoType.h"
#include "Camera/Camera.h"
#include "../Utility/InputPad.h"
#include "../Object/Base/ObjectBase.h"
#include "Dxlib.h"

#define FADE_TIME	 20	//�`���[�g���A���e�L�X�g�̃t�F�[�h�C���A�A�E�g�ɂ�����t���[����
#define TEXT_BOX	 32		//�ЂƂ��̉摜�傫��

class Tutorial
{
private:
	class Camera* camera;		//�J�����|�C���^�i�[(�v���C���[�̏���n�����߂����Ɏ擾)

	bool tutorial_flg;	//�`���[�g���A���������f
	bool tuto_stop_flg;	//�`���[�g���A���ׂ̈ɁA�I�u�W�F�N�g���~�߂�K�v�����邩

	TutoType now_tuto;	//���ݎ��s���̃`���[�g���A�����i�[
	ObjectBase* tuto_object;	//�`���[�g���A�����Ăяo�����I�u�W�F�N�g��ۑ�

	int timer;			//�`���[�g���A�����s���ԑ���
	float text_alpha;	//�t�F�[�h�C���A�A�E�g�̐���
	int stick_anim;		//�X�e�B�b�N���񂷃A�j���[�V�����p
	int button_anim;	//�{�^���A�j���[�V�����p
	int aim_timer;		//�Ə��`���[�g���A���p�@�G�ɏƏ��������Ă��鎞�ԑ���
	int aim_success_flg;		//�Ə�����
	int aim_success_timer;		//�Ə��������ԑ���
	int attack_sequence;		//�U���`���[�g���A���菇
	Vector2D enemy_loc;			//�G���W�ۊ�
	int attack_success_timer;	//�U���������ԑ���
	int tuto_end_timer;			//�U��������e�L�X�g���ԑ���

	int l_stick[4] = { L_STICK_UP,L_STICK_RIGHT,L_STICK_DOWN,L_STICK_LEFT };//�\����
	int r_stick[4] = { R_STICK_UP,R_STICK_RIGHT,R_STICK_DOWN,R_STICK_LEFT };//�\����
	int text_box[3];	//�e�L�X�g�{�b�N�X �ӁA�p�A�����łR��ފi�[
	int generate_text_box;	//���������e�L�X�g�{�b�N�X���i�[����ꏊ
	Vector2D text_box_loc;		//�e�L�X�g�{�b�N�X�\���ʒu
	Vector2D text_box_size;		//�e�L�X�g�{�b�N�X�傫��

	bool ex_anim_flg;			//�A�j���[�V���������邩
	int ex_anim_timer;			//�A�j���[�V�����p
	int now_image;				//�A�j���[�V�����p
	std::vector<int> ex_anim;	//�����A�j���[�V����
	int ex_se;					//������

	bool tuto_executed_flg[TUTO_NUM];	//�`���[�g���A�������ɍs���������f
public:
	bool tuto_flg = false;			//���Ƀ`���[�g���A�����󂯂����Ƃ���������^
	float player_aim_rad = 0.f;		//���݂̏Ə��ʒu
private:
	//�R���X�g���N�^��private�ɂ��邱�ƂŁA
//���N���X�̃����o�֐��ŃC���X�^���X�𐶐��ł��Ȃ��悤�ɂ���
	Tutorial() = default;

	//�R�s�[�K�[�h
	//�N���X�O�ŃC���X�^���X�𐶐����ēn�����Ƃ��ł��Ȃ��悤�ɂ���
	Tutorial(Tutorial& v) = default;
	Tutorial& operator = (const Tutorial& v) = delete;
	~Tutorial() = default;
public:
	//�C���X�^���X���擾���鏈��
	static Tutorial* Get();
public:
	//������(���Z�b�g)
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw()const;

	//�`���[�g���A�������擾
	bool GetTutorialFlg()const { return tutorial_flg; }
	//���݂̃`���[�g���A�����擾
	TutoType GetNowTutorial()const { return now_tuto; }
	//�I�u�W�F�N�g��~��Ԃ��擾
	bool GetTutoStopFlg()const { return tuto_stop_flg; }
	//�w�肵���`���[�g���A�����I����Ă��邩�擾
	bool GetIsEndTutorial(TutoType _type)const { return tuto_executed_flg[_type]; }
	//�w�肵���`���[�g���A�����I����Ă���A�܂��͌��݂��̃`���[�g���A�������擾
	bool GetTutoNowEnd(TutoType _type)const {
		return GetIsEndTutorial(_type) || GetNowTutorial() == _type;
	}
	//��{�`���[�g���A�����I����Ă��邩�擾
	bool GetBasicTuto()const
	{
		return (tuto_executed_flg[TutoType::tRule] &&
			tuto_executed_flg[TutoType::tMove] &&
			tuto_executed_flg[TutoType::tAim] &&
			tuto_executed_flg[TutoType::tAttack]);
	}

	//�`���[�g���A���J�n���N�G�X�g�𑗂� _obj=�`���[�g���A�����Ăяo�����I�u�W�F�N�g
	bool StartTutoRequest(TutoType _type, ObjectBase* _obj = NULL);

	//�`���[�g���A�����̏�����
	void InitTuto(TutoType _type);

	//�`���[�g���A���I������
	void TutoEnd();

	//�Ə����G�ɍ����Ă��邩���f
	bool CheckAim();

	//�e�L�X�g�\���p������
	void CreateTextBox()const;

	//�p�[�c����e�L�X�g�{�b�N�X�𐶐�
	void GenerateTextBox(Vector2D _size)const;


	//���Ԍo�߂ŏI������^�C�v�̃`���[�g���A���̊�b�X�V
	void UpdateTimeTuto();

	//���[�������`��
	void DrawRule()const;

	//�ړ������X�V
	void UpdateMove();
	//�ړ������`��
	void DrawMove()const;

	//�Ə������X�V
	void UpdateAim();
	//�Ə������`��
	void DrawAim()const;

	//�U�������X�V
	void UpdateAttack();
	//�U�������`��
	void DrawAttack()const;

	//�e�ύX�����X�V
	void UpdateBulletChange();
	//�e�ύX�����`��
	void DrawBulletChange()const;
};
