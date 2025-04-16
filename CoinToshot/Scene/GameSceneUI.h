#pragma once
#include "../Utility/UserData.h"
#include <vector>

#define STRING_SPAN 10		//���ړ��m�̊Ԋu

struct UIData
{
	Vector2D local_location;	//�����\���ʒu
	const char* text;			//�\������
	int text_color;				//�����F
	float move_speed;			//UI�ړ����x�i��ɐi�ށj
	int life_span;		//����
	int life_count;		//���ԑ���
};
class GameSceneUI
{
private:
	std::vector<UIData> ui_data;
	std::vector<UIData> delete_ui_data;

public:
	//�X�V
	void Update();
	//�`��
	void Draw()const;

	//��������
	void SetUIData(Vector2D _location, const char* _text,int _text_color, float _move, int _life_span);
};
