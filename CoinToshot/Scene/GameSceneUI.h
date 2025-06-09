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

	//�|�b�v�A�b�v�\���p
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

	//�{�^���摜�i�[
	std::vector<std::vector<int>> button_image;

public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw()const;

	//��������
	void SetUIData(Vector2D _location, string _text, int _text_color, float _move, int _life_span, int _font_size);

	//�e��ޕ\��
	void CreateBulletTypeImage()const;

	//�e���`��
	void DrawBullet(Vector2D _loc, int _type)const;

	//�v���C���[���`��
	void DrawPlayerUI()const;

	//�e��ނ̈ړ����E���������f true=�E
	bool CheckMoveDirection(int _now, int _old)const;
};
