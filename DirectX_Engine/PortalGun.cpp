#include "PortalGun.h"

PortalGun::PortalGun(Input* input, GamePad* gamepad)
{
	//assert(input);

	this->input = input;
	this->gamepad = gamepad;
}

void PortalGun::Initialize()
{
  BLUE_BULLET = { 0,0,0,10 };//xyzr�̏���

  RED_BULLET = { 30,0,0,10 };//��ɓ���
}

void PortalGun::Update()
{
	//�L�[���͏���
	keyAction();

	//�����͓���������t���O���~�߂�������ǉ�����
	if (Flag == 1&&BLUE_BULLET.z<60)
	{
		BLUE_BULLET.z += 1;
	}
    
	if (Flag == 2&&RED_BULLET.z < 60)
	{
		RED_BULLET.z += 1;
	}



}

void PortalGun::keyAction()
{
	//�}�E�X�̍��L�[�����͂��ꂽ�狅���ˏo����
	if (input->TriggerKey(DIK_0))
	{
		if (Flag == 0)
		{
			Flag = 1;
		}
		else if (Flag == 1)
		{
			Flag = 2;
		}
	}
}

void PortalGun::setter_blue(float x, float y, float z, float r)
{
	BLUE_BULLET.x=x;
	BLUE_BULLET.y=y;
	BLUE_BULLET.z=z;
	BLUE_BULLET.r=r;
}

float PortalGun::get_blueZ()
{
	return BLUE_BULLET.z;
}

float PortalGun::get_flagZ()
{
	return Flag;
}
