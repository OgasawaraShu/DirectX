#include "SceneSelect.h"

SceneSelect::SceneSelect(int window_width, int window_height, Input* input)
{
	this->input = input;

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void SceneSelect::SceneInitialize()
{
	//������
	 Load = true;
	 scaleX = 1.0f;
	 scaleY = 1.0f;
	 TitleRed = 0;
	 push = 0;
	 Time = 0;
	 Demo=0;
	 Second = 0;
	 OverRed = 0;
	 change = 0;
	 changeFlag = false;
	 LoadTime = 0;
	 DoorRotate = 0;
}

void SceneSelect::Update()
{
	//�}�E�X�̓��͂��擾
	MouseGet();

	//�^�C�g���V�[���A�N�V����
	Title();
}

void SceneSelect::MouseGet()
{

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(hwnd, &p);

	//�}�E�X�̍��W����
	mouseX = p.x;
	mouseY = p.y;
}

void SceneSelect::Title()
{
	//�^�C�g���Ŏw��͈͓��ŃN���b�N������K�؂ȃV�[���ɂ���
	if (scene == 0)
	{
		if (mouseX >= 412 && mouseX <= 835 && mouseY >= 506 && mouseY <= 568)
		{
			//start������Ԃɂ��ă��C���ɔ�΂�
			OverRed = 1;
			if (input->TriggerMouseLeft())
			{
				changeFlag = true;
				
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else if(mouseX >= 540 && mouseX <= 714 && mouseY >= 604 && mouseY <= 662)
		{
			//end��Ԃɂ��ăQ�[�����I��
			OverRed = 2;
			if (input->TriggerMouseLeft())
			{
				scene = 100;
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else
		{
			OverRed = 0;
		}

		//�t�F�[�h�A�E�g������
		if (changeFlag == true)
		{
			change += 0.005;

			if (change > 1)
			{
				scene = 1;
				
				changeFlag = false;
			}
		}
	}

	
}

void SceneSelect::ChangeScene()
{
	//���������ă^�C�g���ɖ߂�
	if (scene == -1)
	{
		scene = 0;
	}

	if (scene == 1)
	{
		//�o���ɓ�������t�F�[�h�A�E�g�����ă��[�h���
		if (Exit == true)
		{
			change += 0.005;

			if (change > 1)
			{
				Exit = false;
				scene = 99;
			}
		}
		else
		{
			change = 0;
		}
	}

	//���[�h��ʂŉ��o���I������珉������ʂɍs��
	if (scene == 99)
	{
		if (Load == true)
		{
			change = 0;
			LoadTime++;

			if (LoadTime > 140)
			{
				if (DoorRotate > -180)
				{
					DoorRotate -= 1;
				}
			}

			if (LoadTime > 170)
			{
				Load = false;
				LoadTime = 0;
			}
		}
		else
		{
			change += 0.005;

			if (change > 1)
			{
				change = 0;
				scene = -1;
				DoorRotate = 0;
			}
		}
	}

	
}

void SceneSelect::MapEditScene()
{
	//M�L�[��������G�f�B�b�g���
	if (input->TriggerKey(DIK_M))
	{
		MapEdit = true;
	}
}
