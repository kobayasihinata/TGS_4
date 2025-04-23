#include "DxLib.h"

#include "InGameScene.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy1.h"
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
	UserData::player_hp = DEFAULT_HP;
	UserData::coin = 20;

	camera = Camera::Get();

	//�I�u�W�F�N�g�Ǘ��N���X����
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI����
	ui = new GameSceneUI();

	//�v���C���[����
	objects->CreateObject({ Vector2D{0,0},Vector2D{40,40},ePLAYER });

	objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{30,30},eENEMY1/*, 20.f*/ });
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

	//UI�X�V
	ui->Update();

	//�A�C�e������
	SpawnItem();

	//�G����
	SpawnEnemy();

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

	//�v���C���[�`��
	objects->Draw();
	camera->GetCameraLocation().x;
	camera->GetCameraLocation().y;

	//�O���b�h�\��
	for (int x = -STAGE_SIZE; x < STAGE_SIZE; x += 100)
	{
		DrawLineAA(x - camera->GetCameraLocation().x,
			-STAGE_SIZE - camera->GetCameraLocation().y,
			x - camera->GetCameraLocation().x,
			STAGE_SIZE - camera->GetCameraLocation().y,
			0x00ff00);
	}
	for (int y = -STAGE_SIZE; y < STAGE_SIZE; y += 100)
	{
		DrawLineAA(-STAGE_SIZE - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			STAGE_SIZE - camera->GetCameraLocation().x,
			y - camera->GetCameraLocation().y,
			0x00ff00);
	}

	//UI�`��
	ui->Draw();
}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::ChangeResult()
{
	change_scene = eSceneType::eResult;
}

void InGameScene::SpawnItem()
{
	DebugInfomation::Add("frame", frame);

	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(GetRand(100)),(float)(GetRand(100))},Vector2D{40,40},eCOIN, 20.f});
		//objects->CreateObject({ Vector2D{-100,-100},Vector2D{30,30},eENEMY1,/* 20.f */ });
	}
}

void InGameScene::SpawnEnemy()
{
	//��ʊO���烉���_���Ɉ������ŃX�|�[��
	if ((int)frame % 60 == 0)
	{
		objects->CreateObject({ Vector2D{(float)(camera->player_location.x - (SCREEN_WIDTH/2) + (SCREEN_WIDTH * GetRand(1))),(float)(camera->player_location.y - (SCREEN_HEIGHT / 2) + GetRand(SCREEN_HEIGHT))},Vector2D{40,40},eENEMY1, /*20.f */});

	}
}

void InGameScene::CreatePopUp(Vector2D _location, const char* _text, int _font_size, int _text_color, float _move, int _life_span)
{
	ui->SetUIData(_location, _text, _font_size,_text_color, _move, _life_span);
}