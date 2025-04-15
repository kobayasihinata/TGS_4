#pragma once
#include "../../Utility/Vector2D.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/DebugInformation.h"
#include "ObjectList.h"
#include "ShapeCollider.h"
#include <vector>

class ObjectManager;

#define DEFAULT_ANIM_SPAN 20	//�A�j���[�V��������

class ObjectBase : public ShapeCollider
{
protected:
	ObjectManager* manager;
	int object_type;					//�I�u�W�F�N�g�̎��(���Ԃ̓}�l�[�W���[�N���X��ObjectList)

	std::vector<int> animation_image;	//�A�j���[�V�����摜�i�[
	int image;							//���ݕ`�悷��摜
	int anim_timer;						//�A�j���[�V�����摜�؂�ւ��p�^�C�}�[
	int anim_span;						//�A�j���[�V���������ۑ��p
	int image_num;						//�`��摜�̔z���̈ʒu�i�[
	Vector2D image_shift;				//�摜�̈ʒu�����p

public:
	bool hit_flg[4];					//�ӂ��Ƃ̓����蔻��i�[

public:
	//�R���X�g���N�^
	ObjectBase() = default;
	//�f�X�g���N�^	
	virtual ~ObjectBase() = default;
	//����������
	virtual void Initialize(ObjectManager* _manager, int _object_type, Vector2D init_location = 0.0f, Vector2D init_size = 40.0f) {
		
		__super::Initialize(init_location ,init_size);

		manager = _manager;
		object_type = _object_type;

		//������
		anim_span = DEFAULT_ANIM_SPAN;	//�A�j���[�V���������ۑ��p
		image_num = 0;					//�`��摜�̔z���̈ʒu�i�[
	}
	//�I��������
	virtual void Finalize() = 0;
	//�X�V����
	virtual void Update() = 0;
	//�`�揈��
	virtual void Draw()const
	{
		//�摜�`��
		DrawGraphF(local_location.x, local_location.y, image, false);
#ifdef _DEBUG
		//�����蔻��̕`��
		Vector2D::DrawBoxV2(local_location, local_location + box_size, 0xff0000, false);
#endif // _DEBUG
	}
	//���[�J�����W�̐ݒ�
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
	//�����蔻�肪��������̏���
	virtual void Hit(ObjectBase* hit_Object) = 0;
	//�_���[�W����
	virtual void Damage(float _value, Vector2D _attack_loc) = 0;
	//�I�u�W�F�N�g�̎�ގ擾
	int GetObjectType()const { return object_type; }

	//�A�j���[�V��������(��{�I�ȃ��[�v�p)
	void Animation()
	{
		//�w��̎��ԂɒB������
		if (++anim_timer > anim_span)
		{
			//�^�C�}�[���Z�b�g
			anim_timer = 0;
			//�摜�̑�������������ŏ��̉摜�ɖ߂�
			if (++image_num > (animation_image.size() - 1))
			{
				image_num = 0;
			}
			//�A�j���[�V�����摜��؂�ւ���
			image = animation_image[image_num];
		}
	}
};
