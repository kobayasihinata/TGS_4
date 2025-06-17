#pragma once
#include "SceneBase.h"
#include "../Utility/Vector2D.h"
#include <string>
#include <vector>
using namespace std;

#define KEY_WIDTH 10	//���O���̓L�[�{�[�h���v�f��
#define KEY_HEIGHT 4	//���O���̓L�[�{�[�h�c�v�f��

#define START_ANIM_TIME 180		//�J�ډ��o����	
#define RESULT_ANIM_TIME 300	//���U���g�\�����o����
#define BONUS_ANIM_TIME 300		//�{�[�i�X�_�����Z���o����

#define SKIP_TIME 30		//�c�艽�t���[���܂ŃX�L�b�v�o���邩

#define GRAVITY 1	//��t���[���Ԃɉ��Z�����d�͗�
#define DECELERATION 0.1f	//������

struct StringData {
	char text;		//�\������
	Vector2D location;	//���W
	Vector2D goal_location;	//�S�[�����W
	Vector2D goal_size;	//�S�[���傫��
	Vector2D velocity;	//�ړ�����
	float speed;		//���x
	float angle;		//�p�x
	bool is_add;		//�ǉ����镶�����폜���镶����
	bool operator == (const StringData _data)
	{
		if (this->text != _data.text)return false;
		if (this->location != _data.location)return false;
		if (this->goal_location != _data.goal_location)return false;
		if (this->goal_size != _data.goal_size)return false;
		if (this->velocity != _data.velocity)return false;
		if (this->speed != _data.speed)return false;
		if (this->angle != _data.angle)return false;
		if (this->is_add != _data.is_add)return false;
		return true;
	}
};

//���͂ł��镶�������Ƃ��̔z�u
static char key[KEY_HEIGHT][KEY_WIDTH]{
	{'0','1','2','3','4','5','6','7','8','9',},
	{'a','b','c','d','e','f','g','h','i','j',},
	{'k','l','m','n','o','p','q','r','s','t',},
	{'u','v','w','x','y','z',' ',' ',' ',' ',},
};

enum DispScene
{
	dIsClear = 0,
	dDispResult,
	dBonusPoint,
	dEnterName,
};

class ResultScene : public SceneBase
{
private:
	int now_disp;		//�\�����Ă�����


	int start_anim_timer;	//�J�ډ��o���ԑ���
	int result_anim_timer;	//���U���g�\�����o���ԑ���
	int bonus_anim_timer;	//�{�[�i�X�_�����Z���o���ԑ���

	int add_anim_num;		//���Z�A�j���[�V�����p1�t���[���ӂ�̉��Z����
	int add_anim_coin;		//���Z�A�j���[�V�����p�R�C������

	bool add_coin_once;		//�{�[�i�X���Z����񂾂��s��
	//���O���͊֘A
	Vector2D name_string_loc;	//���O�\���ʒu
	Vector2D key_box_loc;		//���O���͔��ʒu
	int current_x;		//�J�[�\��X�ʒu
	int current_y;		//�J�[�\��Y�ʒu
	string name;		//���O�i�[
	std::vector<StringData> string_data;	//���O���͎��̃A�j���[�V�����p
	std::vector<StringData> delete_string_data;	//���O���͎��̃A�j���[�V�����p

	bool disp_se_once;	//���ڕ\��SE�Đ�
	int result_bgm;		//���U���gBGM
	int button_se;		//�{�^��SE
	int erase_se;		//�����폜SE
	int cursor_se;		//�J�[�\��SE
	int disp_se;		//���ڕ\��SE
	int coin_se;		//�R�C�����ZSE
public:
	ResultScene();
	virtual ~ResultScene();

	//����������
	virtual void Initialize()override;

	//�X�V����
	//�����F�P�t���[��������̎���
	//�߂�l�F���̃V�[���^�C�v
	virtual eSceneType Update(float _delta)override;

	//�`�揈��
	virtual void Draw()const override;

	//�I��������
	virtual void Finalize() override;

	//���݂̃V�[���^�C�v(�I�[�o�[���C�h�K�{)
	virtual eSceneType GetNowSceneType()const override;

	//���O����
	eSceneType EnterName();

	//���O���͎���Draw
	void EnterNameDraw()const;

	//�����L���O���בւ�
	void SortRanking();

	//�����ړ�����
	StringData MoveString(StringData _data);

	//������������
	void CreateMoveString(const char _c, Vector2D _loc, Vector2D _goal_loc, Vector2D _goal_size, bool _is_add, Vector2D _velocity = 0.f);
};



