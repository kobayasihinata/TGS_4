#pragma once
#include "DxLib.h"

class Vector2D
{
public:
	float x;	//x��
	float y;	//y��

public:
	Vector2D();			//�R���X�g���N�^
	Vector2D(float scalar);			//�R���X�g���N�^�i�I�[�o�[���[�h�j
	Vector2D(float mx, float my);	//�R���X�g���N�^�i�I�[�o�[���[�h�j

	~Vector2D();					//�f�X�g���N�^

public:
	//����I�y���[�^�[	
	Vector2D& operator = (const Vector2D& location);

	//�������Z�I�y���[�^�[
	bool operator == (const Vector2D& location);

	//�s�������Z�I�y���[�^�[
	bool operator != (const Vector2D& location);

	//���Z�I�y���[�^�[
	const Vector2D operator + (const Vector2D& location)const;
	//���Z����I�y���[�^�[
	Vector2D& operator += (const Vector2D& location);

	//���Z�I�y���[�^�[
	const Vector2D operator - (const Vector2D& location)const;
	//���Z����I�y���[�^�[
	Vector2D& operator -= (const Vector2D& location);

	//��Z�I�y���[�^�[(scalar)
	const Vector2D operator * (const float& scalar)const;
	//��Z�I�y���[�^�[(location)
	const Vector2D operator * (const Vector2D& location)const;
	//��Z����I�y���[�^�[(scalar)
	Vector2D& operator *=(const float& scalar);
	//��Z����I�y���[�^�[(location)
	Vector2D& operator *=(const Vector2D& location);

	//���Z�I�y���[�^�[(scalar)
	const Vector2D operator / (const float& scalar)const;
	//���Z�I�y���[�^�[(location)
	const Vector2D operator / (const Vector2D& location)const;
	//���Z����I�y���[�^�[(scalar)
	Vector2D& operator /= (const float& scalar);
	//���Z����I�y���[�^�[(location)
	Vector2D& operator /= (const Vector2D& location);

public:
	//���W��Int�^�ɃL���X�g����
	void ToInt(int* x, int* y)const;

	/// <summary>
	/// Vector2D�^�̈�����DrawBox������
	/// </summary>
	/// <param name="_loc1">�`����W����</param>
	/// <param name="_loc2">�`����W�E��</param>
	/// <param name="_color">�`��F</param>
	/// <param name="_fill_flg">�h��Ԃ����ǂ���</param>
	static void DrawBoxV2(Vector2D _loc1, Vector2D _loc2, unsigned int _color, bool _fill_flg)
	{
		DrawBoxAA(_loc1.x, _loc1.y, _loc2.x, _loc2.y, _color, _fill_flg);
	}
};







