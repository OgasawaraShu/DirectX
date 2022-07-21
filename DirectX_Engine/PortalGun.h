#pragma once
#include "input.h"
#include "GamePad.h"

class PortalGun
{
public://�����o�֐�
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="window_width">��ʕ�</param>
    /// <param name="window_height">��ʍ���</param>
    /// <param name="input">����</param>
    PortalGun(Input* input,GamePad*gamepad);

    void Initialize();//������
    void Update();//�X�V


    void keyAction();//�L�[���͂ɂ��A�N�V����

    //setter
    void setter_blue(float x,float y,float z,float r);

    float get_blueZ();//blue�̋��̍��W��n��
    float get_flagZ();
   
private://�����o�ϐ�

    struct Gun//�|�[�^�����ˏo����e
    {
        float x;
        float y;
        float z;
        float flag;
    }GUN;

    struct Blue
    {
        float x;
        float y;
        float z;
        float r;
    }BLUE_BULLET;

    struct Red
    {
        float x;
        float y;
        float z;
        float r;
    }RED_BULLET;

    int Flag = 0;//�ˏo�t���O

    // ���̓N���X�̃|�C���^
    Input* input;
    //Pad�N���X�̃|�C���^
    GamePad* gamepad;
    //���x
   // Vector3 velecity_;
};

