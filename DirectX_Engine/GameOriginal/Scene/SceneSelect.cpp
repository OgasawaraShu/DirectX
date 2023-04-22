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
	mouseX = p.x+padX;
	mouseY = p.y+padY;
}

void SceneSelect::Title()
{
	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	GetCursorPos(&p);

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(hwnd, &p);

	//�}�E�X�̍��W����
	mouseX = p.x;
	mouseY = p.y;

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
		if (Old_scene == 1)
		{
			scene = 2;
		}
		else if (Old_scene == 2)
		{
			scene = 3;
		}
		else if (Old_scene == 3)
		{
		    Tutorial = true;
		    Menu_flag = false;
		    Menu_type = 0;
			Tutorial_2 = true;
			Walk_tutorial = true;
			Tutorial_time = 0;
			Tutorial_time_2 = 0;

			scene = 0;
		}
	}

	if (scene == 1)
	{
		//�o���ɓ�������t�F�[�h�A�E�g�����ă��[�h���
		if (Exit == true)
		{
			change += 0.005;
			Old_scene = 1;
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

		if (Tutorial == true&&First_ivent_flag==false)
		{
			Tutorial_time += 1;

			int End_tutoirial_time = 1020;

			if (Tutorial_time > End_tutoirial_time)
			{
				Tutorial = false;
			}
		}

		if (Tutorial_2 == true&&Walk_tutorial==true)
		{
			Tutorial_time_2 += 1;

			int End_tutoirial_time_2 = 1160;

			if (Tutorial_time_2 > End_tutoirial_time_2)
			{
				Tutorial_2 = false;
			}
		}
	}


	if (scene == 2)
	{

		//�o���ɓ�������t�F�[�h�A�E�g�����ă��[�h���
		if (Exit == true)
		{
			change += 0.005;
			Old_scene = 2;
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

	if (scene == 3)
	{

		//�o���ɓ�������t�F�[�h�A�E�g�����ă��[�h���
		if (Exit == true)
		{
			change += 0.005;
			Old_scene = 3;
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
				//���������̃X�e�[�W�ړ��ɂ���
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

void SceneSelect::MenuScene()
{
	if (input->TriggerKey(DIK_ESCAPE) && scene != 99 && scene != 0)
	{
		Menu_flag = true;
	}


	if (Menu_flag == true)
	{
		if (mouseX >= 50 && mouseX <= 258 && mouseY >= 322 && mouseY <= 388)
		{
			//start������Ԃɂ��ă��C���ɔ�΂�
			Menu_type = 1;
			if (input->TriggerMouseLeft())
			{
				Menu_flag = false;
				Option_flag = false;

				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else if (mouseX >= 50 && mouseX <= 290 && mouseY >= 402 && mouseY <= 454)
		{
			//start������Ԃɂ��ă��C���ɔ�΂�
			Menu_type = 2;
			if (input->TriggerMouseLeft())
			{
				Option_flag = true;
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else if (mouseX >= 50 && mouseX <= 218 && mouseY >= 490 && mouseY <= 542)
		{
			//start������Ԃɂ��ă��C���ɔ�΂�
			Menu_type = 3;
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
			Menu_type = 0;
		}
	}
}

void SceneSelect::PadScene()
{
	//�p�b�h�̃|�C���^
	GamePad* GP = nullptr;
	GP = new GamePad();

	//�p�b�h�̍X�V
	GP->Update();

	if (GP->state.Gamepad.sThumbLX != 0 || GP->state.Gamepad.sThumbLY != 0)//�Q�[���p�b�h�A�i���O�X�e�B�b�N���͎�����
	{
		padX += static_cast<FLOAT>(GP->state.Gamepad.sThumbLX / 32767.0 * (6.0f));
		padY -= static_cast<FLOAT>(GP->state.Gamepad.sThumbLY / 32767.0 * (6.0f));
	}


	//�p�b�h�̑���
	if (scene == 0)
	{
		if (OverRed==1)
		{
			if (GP->iPad_B == 1 && Old_iPad_B == 0)
			{
				changeFlag = true;

				push = 1;
			}
			else
			{
				push = 0;
			}

		}
		else if (OverRed==2)
		{
			if (GP->iPad_B == 1 && Old_iPad_B == 0)
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

		
	}

	if ((GP->iPad_Start == 1 && Old_ipad_start == 0) && scene != 99 && scene != 0)
	{
		Menu_flag = true;
	}

	if (Menu_flag == true)
	{
		if (Menu_type ==1)
		{
			if (GP->iPad_B == 1 && Old_iPad_B == 0)
			{
				Menu_flag = false;
				Option_flag = false;

				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else if (Menu_type==2)
		{
			if (GP->iPad_B == 1 && Old_iPad_B == 0)
			{
				Option_flag = true;
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
		else if (Menu_type==3)
		{
			if (GP->iPad_B == 1 && Old_iPad_B == 0)
			{
				scene = 100;
				push = 1;
			}
			else
			{
				push = 0;
			}
		}
	}


	//�g���K�[�����̂��߂̋L��
	Old_iPad_left = GP->iPad_left, Old_iPad_right = GP->iPad_right, iOld_Pad_up = GP->iPad_up, Old_iPad_down = GP->iPad_down;
	Old_iPad_leftshoulder = GP->iPad_leftshoulder, Old_iPad_rightshoulder = GP->iPad_rightshoulder;
	Old_iPad_A = GP->iPad_A, Old_iPad_B = GP->iPad_B, Old_iPad_X = GP->iPad_X, Old_iPad_Y = GP->iPad_Y;
	Old_ipad_start = GP->iPad_Start;

}
