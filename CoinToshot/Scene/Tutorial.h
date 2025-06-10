#pragma once
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"
#include "TutoType.h"
#include "../Utility/InputPad.h"
#include "Dxlib.h"

#define FADE_TIME	 20	//�`���[�g���A���e�L�X�g�̃t�F�[�h�C���A�A�E�g�ɂ�����t���[����
#define TEXT_BOX	 32		//�ЂƂ��̉摜�傫��

class Tutorial
{
private:
	bool tutorial_flg;	//�`���[�g���A���������f
	bool tuto_stop_flg;	//�`���[�g���A���ׂ̈ɁA�I�u�W�F�N�g���~�߂�K�v�����邩

	TutoType now_tuto;	//���ݎ��s���̃`���[�g���A�����i�[

	int timer;			//�`���[�g���A�����s���ԑ���
	float text_alpha;	//�t�F�[�h�C���A�A�E�g�̐���
	int stick_anim;		//�X�e�B�b�N���񂷃A�j���[�V�����p
	int button_anim;		//�{�^���A�j���[�V�����p

	int l_stick[4] = { L_STICK_UP,L_STICK_RIGHT,L_STICK_DOWN,L_STICK_LEFT };//�\����
	int r_stick[4] = { R_STICK_UP,R_STICK_RIGHT,R_STICK_DOWN,R_STICK_LEFT };//�\����
	int text_box[3];	//�e�L�X�g�{�b�N�X �ӁA�p�A�����łR��ފi�[
	int generate_text_box;	//���������e�L�X�g�{�b�N�X���i�[����ꏊ
	Vector2D text_box_loc;		//�e�L�X�g�{�b�N�X�\���ʒu
	Vector2D text_box_size;		//�e�L�X�g�{�b�N�X�傫��

	bool tuto_executed_flg[TUTO_NUM];	//�`���[�g���A�������ɍs���������f
public:


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

	//�`���[�g���A���J�n���N�G�X�g�𑗂� _loc=�`���[�g���A�����Ăяo���������W
	bool StartTutoRequest(TutoType _type,Vector2D _loc = 0);

	//�`���[�g���A�����̏�����
	void InitTuto(TutoType _type, Vector2D _loc);


	//�e�L�X�g�\���p������
	void CreateTextBox()const;

	//�p�[�c����e�L�X�g�{�b�N�X�𐶐�
	void GenerateTextBox(Vector2D _size)const;


	//���Ԍo�߂ŏI������^�C�v�̃`���[�g���A���̊�b�X�V
	void UpdateTimeTuto();
	//����̃A�N�V�����ŏI������^�C�v�̃`���[�g���A���̊�b�X�V
	void UpdatePracticeTuto();

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
};
