#include "DxLib.h"

#include "InGameScene.h"
#include "../Object/Player/Player.h"
#include "../Utility/InputKey.h"
#include "../Utility/UserData.h"

InGameScene::InGameScene() : objects(nullptr)
{
}

InGameScene::~InGameScene()
{

}

void InGameScene::Initialize()
{
	__super::Initialize();

	//�e�������Z�b�g
	UserData::timer = DEFAULT_TIMELIMIT;
	UserData::now_stage = 0;
	UserData::player_hp = DEFAULT_HP;
	UserData::life = DEFAULT_LIFE;
	UserData::score = 0;

	camera = Camera::Get();
	camera->SetStageSize({ (float)UserData::width_num[UserData::now_stage],(float)UserData::height_num[UserData::now_stage] });

	//�I�u�W�F�N�g�Ǘ��N���X����
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI����
	ui = new GameSceneUI();

	//�v���C���[����
	objects->CreateObject({ Vector2D{0,0},Vector2D{0,0},ePLAYER });
}

void InGameScene::Finalize()
{
	//�I�u�W�F�N�g�Ǘ��N���X�I��������
	objects->Finalize();

	//�v���C���[�폜
	delete objects;
}

eSceneType InGameScene::Update(float _delta)
{

	change_scene = __super::Update(_delta);

	//�������Ԍ���
	UserData::timer--;

	//�J�����X�V
	camera->Update();

	//�I�u�W�F�N�g�X�V
	objects->Update();

	//���͋@�\�̎擾
	InputKey* input = InputKey::Get();

	//1�L�[�Ń^�C�g����ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		change_scene = eSceneType::eTitle;
	}

	//2�L�[�Ń��U���g��ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
	{
		change_scene = eSceneType::eResult;
	}

	return change_scene;
}

void InGameScene::Draw()const
{
	DrawString(10, 10, "InGame", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Title  2 = Result", 0xffffff);

	//UI�`��
	ui->Draw();

	//�v���C���[�`��
	objects->Draw();
	camera->GetCameraLocation().x;
	camera->GetCameraLocation().y;
	//�O���b�h�\��
	for (int x = -STAGE_SIZE; x < STAGE_SIZE+SCREEN_WIDTH; x += 100)
	{
		DrawLine(x - camera->GetCameraLocation().x,
			-STAGE_SIZE - camera->GetCameraLocation().y,
			x - camera->GetCameraLocation().x,
			STAGE_SIZE + SCREEN_HEIGHT - camera->GetCameraLocation().y,
			0x00ff00);
	}
	for (int y = -STAGE_SIZE; y < STAGE_SIZE+SCREEN_HEIGHT; y += 100)
	{
		DrawLine(-STAGE_SIZE - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			STAGE_SIZE + SCREEN_WIDTH - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			0x00ff00);
	}
}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::ChangeNextStage()
{
	//���݂̃X�e�[�W���ŏI�X�e�[�W�őJ�ڂ���ꍇ�̓��U���g��
	if (UserData::now_stage >= UserData::stage_num - 1)
	{
		//���U���g�J��
		ChangeResult();
	}
	//�ŏI�X�e�[�W�����̃X�e�[�W�Ȃ玟�̃X�e�[�W��
	else
	{
		//���̃X�e�[�W�ֈړ�
		UserData::now_stage++;
		//�����̃I�u�W�F�N�g�͑S����
		objects->DeleteAllObject();
		//�ǂݍ��񂾃f�[�^����ɃX�e�[�W�𐶐�����
		std::vector<ObjectData> obj_data = UserData::stage_data[UserData::now_stage];
		for (const auto obj_data : obj_data)
		{
			objects->CreateObject(obj_data);
		}
	}

}
void InGameScene::ChangeResult()
{
	change_scene = eSceneType::eResult;
}


