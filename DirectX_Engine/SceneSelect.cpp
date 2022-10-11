#include "SceneSelect.h"

SceneSelect::SceneSelect(int window_width, int window_height, Input* input)
{
	this->input = input;

	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void SceneSelect::Initialize()
{
	
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
	if (scene == 0)
	{
		if (input->TriggerMouseLeft())
		{
			scene = 1;
			push = 1;
		}
		else
		{
			push = 0;
		}
	}
}
