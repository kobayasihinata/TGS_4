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
		timer = _timer;
		anim_span = _anim_span;
	}

	//�I��������
	virtual void Finalize() = 0;

	//�X�V����
	virtual void Update() = 0;

	//�`�揈��(�摜�G�t�F�N�g�Ȃ�__super����@Dxlib�̊֐��ŃG�t�F�N�g�����Ȃ�__super���Ȃ�)
	virtual void Draw()const
	{
		//�摜�`��
		if (image != 0)
		{
			DrawRotaGraphF(local_location.x, local_location.y, 1.f, 1.f, image, TRUE);
		}
		else
		{
			DrawStringF(local_location.x, local_location.y, "�摜����", 0x00ff00);
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
