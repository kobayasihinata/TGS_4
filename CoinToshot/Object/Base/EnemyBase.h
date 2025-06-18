#pragma once
#include "ActorBase.h"
#include "../../Scene/Camera/Camera.h"
#include <vector>

class EnemyBase :
	public ActorBase
{
protected:
	class Camera* camera;	//�J�����|�C���^�i�[(�v���C���[�̏���n�����߂����Ɏ擾)
	float move_speed;		//�ړ����x
	int hit_damage;			//�v���C���[�ɗ^����_���[�W��

	bool rare_flg;					//���A�̃t���O
	std::vector<int> shine_image;	//�I�[���摜�i�[
	int now_shine_image;			//���ݕ`��I�[���摜

	int death_se;				//���S��SE
public:

	EnemyBase()
	{
		//�J�����擾
		camera = Camera::Get();
		move_speed = 0.f;		
		hit_damage = 0;			
		drop_coin = 0;	
		drop_coin_count = 0;

		//���A�̒��I
		rare_flg = (GetRand(30) == 0) ? true : false;

		if (rare_flg)
		{
			//�摜�Ǎ�
			ResourceManager* rm = ResourceManager::GetInstance();
			std::vector<int>tmp;
			shine_image = rm->GetImages("Resource/Images/Effect/yellow_shine.png", 40, 8, 5, 96, 96);
			now_shine_image = 0;
		}
	}

	virtual void Update()override
	{
		__super::Update();

		MovetoPlayer();

		//���A�̂Ȃ�I�[���A�j���[�V�����̍X�V
		if (rare_flg)
		{
			if (++now_shine_image > 39)
			{
				now_shine_image = 0;
			}
		}
	}

	virtual void Draw()const override
	{
		//���A�̂Ȃ�A�I�[���̕`�������
		if (rare_flg)
		{
			DrawRotaGraphF(local_location.x, local_location.y, box_size.x/40, 0, shine_image[now_shine_image], TRUE);
		}

		__super::Draw();

		//HP�Q�[�W����
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0x000000,
			true
		);
		//HP�Q�[�W�{��
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)) + hpbar_move,
			local_location.y - (box_size.y / 2),
			0xffff00,
			true
		);
		//HP�Q�[�W�����A�j���[�V����
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x - (HPBAR_SIZE / 2) + (hp * (HPBAR_SIZE / max_hp)) + hpbar_move,
			local_location.y - (box_size.y / 2),
			0xff0000,
			true
		);
		//HP�Q�[�W�O�g
		DrawBoxAA(local_location.x - (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2) - 10,
			local_location.x + (HPBAR_SIZE / 2),
			local_location.y - (box_size.y / 2),
			0xffffff,
			false
		);

#ifdef _DEBUG
		//hp�`��
		DrawFormatStringF(local_location.x, local_location.y - (box_size.y / 2), 0xffffff, "HP:%f", this->hp);
#endif // _DEBUG

	}

	virtual void Hit(ObjectBase* hit_object)override
	{
		__super::Hit(hit_object);

		//���g�������Ă��āA�v���C���[�ɓ���������_���[�W��^����
		if (!this->death_flg &&
			hit_object->GetObjectType() == ePLAYER)
		{
			hit_object->Damage(hit_damage, this->location);
		}

	}

	void Move()override
	{
		//�����̎��s
		velocity.x -= velocity.x / 10;
		velocity.y -= velocity.y / 10;
		//�ړ��O�̍��W�i�[
		old_location = location;
		//�ړ��̎��s
		location += velocity;
	}
	//�v���C���[�Ɍ������Ĉړ�����
	void MovetoPlayer()
	{
		//���S���Ă��Ȃ���΂��̏���
		if (!this->death_flg)
		{
			double radian = atan2(camera->player_location.y - this->location.y, camera->player_location.x - this->location.x);
	
			//�ړ��̏���l�ݒ�
			if(fabsf(velocity.x) < move_speed)velocity.x += move_speed * cos(radian);
			if(fabsf(velocity.y) < move_speed)velocity.y += move_speed * sin(radian);

			//�ړ������������ۑ�
			move_velocity = { move_speed * cosf(radian) ,move_speed * sinf(radian) };
		}
	}

};