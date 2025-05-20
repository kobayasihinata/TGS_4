#include "DxLib.h"

#include "InGameScene.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy1.h"
#include "../Utility/InputKey.h"
#include "../Utility/UserData.h"
#define _OPEN_SYS_ITOA_EXT
#include <string.h>

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
	UserData::is_gamestop = false;

	change_result_delay = -1;//0�ɂȂ����烊�U���g�J��
	change_result = false;

	camera = Camera::Get();

	//�I�u�W�F�N�g�Ǘ��N���X����
	objects = new ObjectManager();
	objects->Initialize(this);

	//UI����
	ui = new GameSceneUI();
	ui->Initialize();

	//�v���C���[����
	objects->CreateObject({ Vector2D{0,0},Vector2D{40,40},ePLAYER });

	objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{100,100},eSLOT/*, 20.f*/ });
	//objects->CreateObject({ Vector2D{(float)GetRand(200),(float)GetRand(200)},Vector2D{ENEMY5_WIDTH,ENEMY5_HEIGHT},eENEMY5/*, 20.f*/ });
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

	//���Ԓ�~�t���O�������Ă�����I�u�W�F�N�g�̓����͂��ׂĎ~�߂�
	if (!UserData::is_gamestop)
	{
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
	}


	//���͋@�\�̎擾
	InputKey* input = InputKey::Get();

	//���Ԑ؂�ŏI���i���������j
	if (UserData::timer <= 0 && !change_result)
	{
		UserData::is_clear = true;
		UserData::is_gamestop = true;
		change_result = true;
		change_result_delay = 120;
	}

	//���U���g�J�ڑO�̉��o
	if (change_result && --change_result_delay <= 0)
	{
		change_scene = eSceneType::eResult;
	}


#ifdef _DEBUG
	//1�L�[�Ń^�C�g����ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_1) == eInputState::Pressed)
	{
		change_result_delay = 20;
	}

	//2�L�[�Ń��U���g��ʂɑJ�ڂ���
	if (input->GetKeyState(KEY_INPUT_2) == eInputState::Pressed)
	{
		change_scene = eSceneType::eResult;
	}
#endif // _DEBUG

	return change_scene;
}

void InGameScene::Draw()const
{
	DrawString(10, 10, "InGame", GetColor(255, 255, 255));
	DrawString(10, 30, "1 = Title  2 = Result", 0xffffff);


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

	//�I�u�W�F�N�g�`��
	objects->Draw();

	//UI�`��
	ui->Draw();

	//�Q�[���I�[�o�[���N���A���\��
	if (UserData::is_gamestop)
	{
		if (UserData::is_clear)
		{
			DrawBox((SCREEN_WIDTH/2) - 200, (SCREEN_HEIGHT/2) - 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2) + 50, 0x000000, true);
			DrawBox((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2) - 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2) + 50, 0xffffff, false);
			DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 30, "GameClear!", 0xffffff);
		}
		else
		{
			DrawBox((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2) - 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2) + 50, 0x000000, true);
			DrawBox((SCREEN_WIDTH / 2) - 200, (SCREEN_HEIGHT / 2 )- 50, (SCREEN_WIDTH / 2) + 200, (SCREEN_HEIGHT / 2 )+ 50, 0xffffff, false);
			DrawString((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 30, "GameOver...", 0xaaaaaa);
		}
	}
}

eSceneType InGameScene::GetNowSceneType()const
{
	return eSceneType::eInGame;
}

void InGameScene::ChangeResult(int _delay)
{
	//�x������
	if (_delay <= 0)
	{
		change_scene = eSceneType::eResult;
	}
	//�x������
	else
	{
		//���~��
		UserData::is_gamestop = true;
		//���U���g�J�ڏ����J�n
		change_result = true;
		//���U���g�J�ڂ܂ł̎��Ԑݒ�
		change_result_delay = _delay;
	}
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
	if ((int)frame % 90 == 0)
	{
		objects->CreateObject(GetEnemyData());
	}
}

void InGameScene::CreatePopUp(Vector2D _location, string _text, int _font_size, int _text_color, float _move, int _life_span)
{
	ui->SetUIData(_location, _text, _font_size,_text_color, _move, _life_span);
}

Vector2D InGameScene::GetRandLoc()
{
	Vector2D ret;

	//���E�̒[���㉺�̒[�������_���Ō���
	if ((bool)GetRand(1))
	{
		//���E�̒[
		ret.x = (float)(camera->player_location.x - SCREEN_WIDTH + (SCREEN_WIDTH * 2 * GetRand(1)));
		ret.y = (float)(camera->player_location.y - SCREEN_HEIGHT + GetRand(SCREEN_HEIGHT*2));
	}
	else
	{
		//�㉺�̒[
		ret.x = (float)(camera->player_location.x - SCREEN_WIDTH + GetRand(SCREEN_WIDTH*2));
		ret.y = (float)(camera->player_location.y - SCREEN_HEIGHT + (SCREEN_HEIGHT * 2 * GetRand(1)));
	}
	return ret;
}

ObjectList InGameScene::GetRandEnemy()
{
	int coin = UserData::coin;
	//�R�C����0�`99�Ȃ�enemy1���X�|�[��
	if (coin < 50)
	{
		return GetEnemy(eENEMY5, 100);
	}
	//�R�C����100�`199�Ȃ�enemy1��2�������_���ŃX�|�[��
	if (coin < 100)
	{
		return GetEnemy(eENEMY1, 50, eENEMY2, 50);
	}
	//�R�C����200�`299�Ȃ�enemy2���X�|�[��
	if (coin < 150)
	{
		return GetEnemy(eENEMY2, 100);
	}
	//�R�C����300�`399�Ȃ�enemy2��3���X�|�[��
	if (coin < 200)
	{
		return GetEnemy(eENEMY2, 50, eENEMY3, 50);
	}
	//�R�C����400�`499�Ȃ�enemy2��4���X�|�[��
	if (coin < 250)
	{
		return GetEnemy(eENEMY2, 90, eENEMY4, 10);
	}
	//�R�C����500�`599�Ȃ�enemy2��5���X�|�[��
	if (coin < 300)
	{
		return GetEnemy(eENEMY2, 80, eENEMY5, 20);
	}
	//����ȍ~�͑S�Ă̓G���ϓ���
	else
	{
		return GetEnemy(eENEMY1, 20, eENEMY2, 20, eENEMY3, 20, eENEMY4, 20, eENEMY5, 20);
	}

	//�����Ȃ���ΓG1���X�|�[��������
	return eENEMY2;
}

ObjectList InGameScene::GetEnemy(ObjectList _list1, int _prob1,
	ObjectList _list2, int _prob2,
	ObjectList _list3, int _prob3,
	ObjectList _list4, int _prob4,
	ObjectList _list5, int _prob5)
{
	int rand = GetRand(_prob1 + _prob2 + _prob3 + _prob4 + _prob5);

	//�����_���Ȓl���I�u�W�F�N�g�P�̊m���̒l��������Ă�����
	if (rand < _prob1)
	{
		//�I�u�W�F�N�g�P��Ԃ�
		return _list1;
	}
	//�����_���Ȓl���I�u�W�F�N�g�P�A�Q�̊m���̍��v��������Ă�����
	else if (rand < _prob1 + _prob2)
	{
		//�I�u�W�F�N�g�Q
		return _list2;
	}
	//�����_����(��)
	else if (rand < _prob1 + _prob2 + _prob3)
	{
		//�I�u�W�F�N�g�R
		return _list3;
	}
	//�����_����(��)
	else if (rand < _prob1 + _prob2 + _prob3 + _prob4)
	{
		//�I�u�W�F�N�g�S
		return _list4;
	}
	//�����_����(��)
	else if (rand < _prob1 + _prob2 + _prob3 + _prob4 + _prob5)
	{
		//�I�u�W�F�N�g�T
		return _list5;
	}

	//�����̃G���[�Ŏ��s������R�C�����h���b�v����
	return eCOIN;
}

ObjectData InGameScene::GetEnemyData()
{
	ObjectData ret;
	ObjectList spawn = GetRandEnemy();
	//�X�|�[������G�ɉ������傫���������l�ɑ������
	switch (spawn)
	{
	case ObjectList::eENEMY1:
		ret = { GetRandLoc(), Vector2D{ ENEMY1_WIDTH,ENEMY1_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY2:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY2_WIDTH,ENEMY2_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY3:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY3_WIDTH,ENEMY3_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY4:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY4_WIDTH,ENEMY4_HEIGHT }, spawn};
		break;														  
	case ObjectList::eENEMY5:										  
		ret = { GetRandLoc(), Vector2D{ ENEMY5_WIDTH,ENEMY5_HEIGHT }, spawn};
		break;
	default:
		break;
	}
	return ret;
}