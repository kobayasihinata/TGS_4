#include "ShapeCollider.h"
#include "ObjectBase.h"
#include "ActorBase.h"

void ShapeCollider::Initialize(Vector2D init_location, Vector2D init_size)
{
	location = init_location;
	local_location = 0;
	box_size = init_size;
}

bool ShapeCollider::CheckHit(ShapeCollider* object2)
{
	return CheckHit(object2->GetLocation(), object2->GetSize());
}


bool ShapeCollider::CheckHit(Vector2D _loc2, Vector2D _size2)
{
	Vector2D loc1 = this->location;
	Vector2D size1 = this->box_size;
	Vector2D loc2 = _loc2;
	Vector2D size2 = _size2;
	//�����蔻��m�F
	if (loc1.x < loc2.x + size2.x &&
		loc2.x < loc1.x + size1.x &&
		loc1.y < loc2.y + size2.y &&
		loc2.y < loc1.y + size1.y)
	{
		return true;
	}
	return false;
}

void ShapeCollider::Push(ShapeCollider* hit_object)
{
	Vector2D saveloc = this->location;
	Vector2D savesize = this->box_size;

	Vector2D loc = saveloc;
	Vector2D size = savesize;

	bool stageHitFlg[2][4] = { false };
	float move[4] = { 0 };

	//�㉺����p�ɍ��W�ƃG���A�̒���
	loc = { saveloc.x + 10.f,saveloc.y };
	size = { savesize.x - 20.f, 1.f };

	//�v���C���[������̔���
	if (CheckHit(hit_object) && !stageHitFlg[1][top]) {
		stageHitFlg[0][top] = true;
		stageHitFlg[1][top] = true;
	}
	else {
		stageHitFlg[0][top] = false;
	}

	//�v���C���[�������̔���
	loc.y = saveloc.y + savesize.y + 1;

	if (CheckHit(hit_object) && !stageHitFlg[1][bottom]) {
		stageHitFlg[0][bottom] = true;
		stageHitFlg[1][bottom] = true;
	}
	else {
		stageHitFlg[0][bottom] = false;
	}

	//�ŏ��̒l�ɖ߂�
	loc = saveloc;
	size = savesize;

	//������ɖ��܂�Ȃ��悤�ɂ���
	if (stageHitFlg[0][top]) {//������ɖ��܂��Ă�����
		float t = (hit_object->GetLocation().y + hit_object->GetSize().y) - loc.y;
		if ((int)t != 0) {
			move[top] = t;
		}
	}

	//�������ɖ��܂�Ȃ��悤�ɂ���
	if (stageHitFlg[0][bottom]) {//�������ɖ��܂��Ă�����
		float t = hit_object->GetLocation().y - (loc.y + size.y);
		if ((int)t != 0) {
			move[bottom] = t;
		}
	}

	//���E����p�ɍ��W�ƃG���A�̒���
	size = { 1.f,savesize.y - 20.f };

	//�v���C���[�������̔���
	if (CheckHit(hit_object) && !stageHitFlg[1][left]) {
		stageHitFlg[0][left] = true;
		stageHitFlg[1][left] = true;
	}
	else {
		stageHitFlg[0][left] = false;
	}


	//�v���C���[�E�����̔���
	loc.x = saveloc.x + savesize.x + 1;

	if (CheckHit(hit_object) && !stageHitFlg[1][right]) {
		stageHitFlg[0][right] = true;
		stageHitFlg[1][right] = true;
	}
	else {
		stageHitFlg[0][right] = false;
	}

	//�ŏ��̒l�ɖ߂�
	loc = saveloc;
	size = savesize;

	//�������ɖ��܂�Ȃ��悤�ɂ���
	if (stageHitFlg[0][left]) {//�������ɖ��܂��Ă�����
		float t = (hit_object->GetLocation().x + hit_object->GetSize().x) - loc.x;
		if ((int)t != 0) {
			move[left] = t;
		}
	}

	//�E�����ɖ��܂�Ȃ��悤�ɂ���
	if (stageHitFlg[0][right]) {//�E�����ɖ��܂��Ă�����
		float t = hit_object->GetLocation().x - (loc.x + size.x);
		if ((int)t != 0) {
			move[right] = t;
		}
	}


	//�㉺���E�̈ړ��ʂ���ړ�������܂��Ă邩���ׂ�
	//���E�ړ������Ă݂Ă܂����܂��Ă���߂�
		//�㉺�̍��W�X�V
	loc.y += (move[top] + move[bottom]);
	loc.x += (move[left] + move[right]);

	if (loc.x +
		size.x <
		hit_object->GetLocation().x ||
		loc.x >
		hit_object->GetLocation().x +
		hit_object->GetSize().x) {
		if (stageHitFlg[1][top] || stageHitFlg[1][bottom]) {
			loc.x -= (move[left] + move[right]);
		}
	}

	//�ړ��ʂ����ۂ̃I�u�W�F�N�g�ɔ��f������
	this->location = loc;

	////���������ӂ�ۑ�����
	//for (int i = 0; i < 4; i++)
	//{
	//	myself->hit_flg[i] = stageHitFlg[0][i];
	//}
}