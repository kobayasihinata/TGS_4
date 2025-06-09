#pragma once
#include "../Utility/common.h"
#include "../Utility/Vector2D.h"
#include "TutoType.h"
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

	int text_box[3];	//�e�L�X�g�{�b�N�X �ӁA�p�A�����łR��ފi�[
	int generate_text_box;	//���������e�L�X�g�{�b�N�X���i�[����ꏊ
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

	//�I�u�W�F�N�g��~��Ԃ��擾
	bool GetTutoStopFlg()const { return tuto_stop_flg; }

	//�`���[�g���A���J�n���N�G�X�g�𑗂�
	bool StartTutoRequest(TutoType _type);

	//�`���[�g���A�����̏�����
	void InitTuto(TutoType _type);


	//���[�������`��
	void DrawRule()const;

	//�e�L�X�g�\���p������
	void CreateTextBox()const;

	//�p�[�c����e�L�X�g�{�b�N�X�𐶐�
	void GenerateTextBox(Vector2D _loc, Vector2D _size)const;
};
