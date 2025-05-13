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
	int change_result_delay;	//���U���g�J�ڂ܂ł̒x������
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
	/// ���U���g�J��
	/// </summary>
	/// <param name="_delay">�J�ڂ���܂ł̒x��</param>
	void ChangeResult(int _delay = 0);

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
	void CreatePopUp(Vector2D _location, string _text, int _font_size, int _text_color, float _move, int _life_span);

	/// <summary>
	/// ��ʒ[�̃����_���ȍ��W���󂯎��
	/// </summary>
	/// <returns>���W</returns>
	Vector2D GetRandLoc();

	/// <summary>
	/// �v���C���[�̃R�C���������ɉ����ēG�̏o���m����ς���
	/// </summary>
	/// <returns>�X�|�[��������G</returns>
	ObjectList GetRandEnemy();

	/// <summary>
	/// �w�肵���I�u�W�F�N�g���w�肵���m���ŕԂ�
	/// ��FGetEnemy(eEnemy1,100)���m����eEnemy1��Ԃ�
	/// ��FGetEnemy(eEnemy1,50,eEnemy2,50)���ܕ��ܕ���eEnemy1��eEnemy2��Ԃ�
	/// 
	/// �m���́A�S�Ă̒l�����v���āA�P�O�O���ɂȂ�悤�Ɋ������l�Œ��I�����
	/// ��FGetEnemy(eEnemy1,100,eEnemy2,50)�����v��150�Ȃ̂ŁA0~100��eEnemy1�A101~150��eEnemy2�@�܂�eEnemy1�͖�66%�̊m��
	/// ��FGetEnemy(eEnemy1,25,eEnemy2,25)��Enemy1��50%�̊m��
	/// 
	/// </summary>
	ObjectList GetEnemy(ObjectList _list1,int _prob1,
		ObjectList _list2 = eNULL, int _prob2 = 0,
		ObjectList _list3 = eNULL, int _prob3 = 0,
		ObjectList _list4 = eNULL, int _prob4 = 0, 
		ObjectList _list5 = eNULL, int _prob5 = 0);

	/// <summary>
	/// �G�̐����ɕK�v�ȏ����A���݂̏�Ԃɉ����Ď擾
	/// </summary>
	/// <returns>�G�̐����ɕK�v�ȏ��</returns>
	ObjectData EnemyRandSpawn();
};



