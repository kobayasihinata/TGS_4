#pragma once
#include "../../Utility/Vector2D.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/DebugInformation.h"
#include <vector>
#include "../../Utility/common.h"

class ObjectManager;

class EffectBase
{
protected:
	ObjectManager* manager;

	Vector2D location;					//��΍��W
	Vector2D local_location;			//���΍��W

	std::vector<int> animation_image;	//�A�j���[�V�����摜�i�[
	int image;							//���ݕ`�悷��摜
	Vector2D image_shift;				//�摜�̈ʒu�����p
	int now_anim;						//���ݕ`�撆�̉摜�ʒu

	int init_timer;						//�^�C�}�[�����l
	int timer;							//���ԑ���
	int anim_span;						//�A�j���[�V�����Ԋu
	bool front_flg;						//�I�u�W�F�N�g���O�ɕ`�悷�邩���f

public:
	//�R���X�g���N�^
	EffectBase() = default;

	//�f�X�g���N�^	
	virtual ~EffectBase() = default;

	//����������
	virtual void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span) {

		location = init_location;
		local_location = 0;
		manager = _manager;
		//������				
		image = 0;
		image_shift = 0;
		now_anim = 0;
		timer = _timer;
		anim_span = _anim_span;
		front_flg = _front_flg;
	}

	//�I��������
	virtual void Finalize() = 0;

	//�X�V����(�摜�G�t�F�N�g�Ȃ�__super����@Dxlib�̊֐��ŃG�t�F�N�g�����Ȃ�__super���Ȃ�)
	virtual void Update()
	{
		//�A�j���[�V�����̐؂�ւ����x���A�摜�����ƃG�t�F�N�g���Ԃ��犄��o��
		int r = init_timer / (animation_image.size() - 1);
		if (r <= 0)r = 1;
		if (timer % r == 0)
		{
			if (now_anim >= animation_image.size())
			{
				image = 0;
			}
			else
			{
				image = animation_image[now_anim++];
			}
		}

	}

	//�`�揈��(�摜�G�t�F�N�g�Ȃ�__super����@Dxlib�̊֐��ŃG�t�F�N�g�����Ȃ�__super���Ȃ�)
	virtual void Draw()const
	{
		//�摜�`��
		if (image != 0)
		{
			DrawRotaGraphF(local_location.x, local_location.y, 1.f, 0.f, image, TRUE);
		}
	}

	//�I�u�W�F�N�g�̌��ɕ`�悷�邩�擾
	bool GetFrontFlg()const
	{
		return front_flg;
	}

	//���[�J�����W�̐ݒ�
	void SetLocalLocation(Vector2D camera_location)
	{
		local_location = this->location - camera_location;
	}
};
