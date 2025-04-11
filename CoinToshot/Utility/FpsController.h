#pragma once
#include"DxLib.h"
#include "DebugInformation.h"
#include "common.h"

class FpsController {
private:
    int FrameTime; //1�t���[���̎���(�~���b)
    int WaitTime; //�҂�����
    int LastTime, NowTime; //�Ō�Ɏ擾�������ԂƁC���̎���
    float Count; //�t���[���̃J�E���g
    float Fps; //�\�����邵��FPS�l
    int UpdateTime; //�\���l���X�V���鎞��
    int LastUpdate; //�Ō�ɕ\���l���X�V��������
    //������
    void Init(float RefreshRate, int UpdateTime);
public:

    //�R���X�g���N�^
    FpsController(float RefreshRate, int UpdateTime) {
        Init(RefreshRate, UpdateTime);
    }
    FpsController() {
        Init(FRAME_RATE, 800);
    }

    //�҂����Ԃ̌v�Z
    void Wait();

    //FPS�l�̌v�Z
    float Get();

    //�`�揈��
    void Disp() {
        DebugInfomation::Add("fps", Fps);
    }
    //�������܂Ƃ߂�����
    float All() {
        Get();
        Wait();
        return (Fps);
    }

    //�t���[�����[�g�̍X�V
    void UpdateFrameRate(float RefreshRate)
    {
        FrameTime = (int)(1000.0f / RefreshRate);
    }
};





