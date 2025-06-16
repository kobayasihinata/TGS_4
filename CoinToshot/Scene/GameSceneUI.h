#pragma once
#include "../Utility/UserData.h"
#include "Tutorial.h"
#include <vector>
#include <string>
using namespace std;

#define STRING_SPAN 10		//���ړ��m�̊Ԋu

struct UIData
{
	Vector2D location;		//�����\���ʒu
	string text;		//�\������
	int font_size;			//�����傫��
	int text_color;			//�����F
	float move_speed;		//UI�ړ����x�i��ɐi�ށj
	int life_span;			//����
	int life_count;			//���ԑ���

	bool operator == (const UIData _data)
	{
		if (this->location != _data.location)return false;
		if (this->text != _data.text)return false;
		if (this->text_color != _data.text_color)return false;
		if (this->move_speed != _data.move_speed)return false;
		if (this->life_span != _data.life_span)return false;
		if (this->life_count != _data.life_count)return false;
		return true;
	}
};

struct ConfettiData
{
	Vector2D location;	//�\�����W
	Vector2D velocity;	//�ړ���
	int image;			//�摜�i�[
	float radian;		//�`��p�x
	float move_radian;	//�p�x�ύX��̒l
	float speed;		//�ړ����x

	bool operator == (const ConfettiData _data)
	{
		if (this->location != _data.location)return false;
		if (this->velocity != _data.velocity)return false;
		if (this->image != _data.image)return false;
		if (this->radian != _data.radian)return false;
		if (this->move_radian != _data.move_radian)return false;
		if (this->speed != _data.speed)return false;
		return true;
	}
};
class GameSceneUI
{
private:
	int frame;				//�t���[���v��
	int max_attraction;		//�z���񂹃o�[�\���p

	class Tutorial* tutorial;	//�`���[�g���A���i�[

	int bullet_image;		//�e��ޕ\��
	int old_bullet_type;	//�e��ޕύX�p
	int bullet_change_timer;//�e�ύX�A�j���[�V�������ԑ���
	float change_anim_move;	//�A�j���[�V�����̈ړ��ʊi�[

	Vector2D player_ui_loc;	//�v���C���[���ʒu
	int now_coin_num;		//���݂̃R�C������
	int old_coin_num;		//�P�t���[���O�̃R�C������

	int con_spawn;			//�����ᐶ���܂ł̊Ԋu

	int lock_image;			//"���b�N����Ă���"�p�摜
	int confetti_image[8];	//8�F�̎�����i�[

	bool ex_anim_once;			//��񂾂��A�j���[�V����
	bool ex_se_once;			//��񂾂��Đ�
	int ex_anim_timer;			//�A�j���[�V�����p
	int now_image;				//�A�j���[�V�����p
	std::vector<int> ex_anim;	//�����A�j���[�V����
	int ex_se;					//������
	int lock_se;				//�g�p�s��SE

	//�|�b�v�A�b�v�\���p
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

	std::vector<ConfettiData> confetti_data;
	std::vector<ConfettiData> delete_confetti_data;

public:
	bool confetti_flg;	//������𐶐����邩���f
public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw()const;

	//��������
	void SetUIData(Vector2D _location, string _text, int _text_color, float _move, int _life_span, int _font_size);

	//������f�[�^����
	ConfettiData GetConfettiData()const;

	//�e��ޕ\��
	void CreateBulletTypeImage()const;

	//���b�N�摜����
	void CreateLockImage()const;

	//������W��ސ���
	void CreateConfettiImage();

	//�e���`��
	void DrawBullet(Vector2D _loc, int _type)const;

	//�v���C���[���`��
	void DrawPlayerUI()const;

	//�e��ނ̈ړ����E���������f true=�E
	bool CheckMoveDirection(int _now, int _old)const;
};
