#pragma once
#include "SceneBase.h"
#include "../Object/ObjectManager.h"
#include "Camera/Camera.h"
#include "GameSceneUI.h"
#include "../Object/Base/ShapeCollider.h"

class InGameScene : public SceneBase
{
private:
	class Camera* camera;		//�J�����i�[
	ObjectManager* objects;		//�I�u�W�F�N�g�Ǘ��N���X
	GameSceneUI* ui;			//�Q�[�����C��UI
	eSceneType change_scene;	//�J�ڐ�

	ShapeCollider* test;
public:
	InGameScene();
	virtual ~InGameScene();

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

	/// <summary>
	/// ���̃X�e�[�W�ɐ؂�ւ��� �Ȃ��Ȃ�N���A���U���g��ʂ�
	/// </summary>
	void ChangeNextStage();

	/// <summary>
	/// ���U���g�J��
	/// </summary>
	/// <param name="_is_clear">false=�Q�[���I�[�o�[ true=�Q�[���N���A</param>
	void ChangeResult();

	/// <summary>
	/// �����_���ŃA�C�e���𔭐�������
	/// </summary>
	void SpawnItem();

	/// <summary>
	/// �����_���ɓG�𐶐�
	/// </summary>
	void SpawnEnemy();

	/// <summary>
	/// ��ʏ�ɏo��UI����
	/// </summary>
	/// <param name="_location">�\��������W</param>
	/// <param name="_text">�\�����镶��</param>
	/// <param name="_text_color">�����F</param>
	/// <param name="_move">�ړ���</param>
	/// <param name="_life_span">��������</param>
	void CreatePopUp(Vector2D _location, const char* _text, int _font_size, int _text_color, float _move, int _life_span);
};



