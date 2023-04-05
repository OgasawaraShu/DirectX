#pragma once
#include "../../Engine/Input/Input.h"
#include "../../Engine/Input/GamePad.h"

class SceneSelect
{
public:
	SceneSelect(int window_width, int window_height, Input* input);
	//������
	void SceneInitialize();
	//�X�V
	void Update();
	//�}�E�X�擾
	void MouseGet();
	//�^�C�g���V�[��
	void Title();
	//�J�ڏ���
	void ChangeScene();
	//�X�e�[�W�G�f�B�b�g
	void MapEditScene();
	//���j���[�V�[��
	void MenuScene();
	//�A�t�^�[
	void PadScene();

	//hwnd�Z�b�g
	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }
	//�f�o�b�O
	float GetMousePoint() { return mouseX; }
	//�V�[���Q��
	int GetScene() { return scene; }

	int OldScene() { return Old_scene; }
	//���������Q��
	int GetPush() { return push; }
	//�W�J���Q��
	int GetTime() { return Time; }
	//�h�A���ǂꂾ����]������
	float GetDoorY() { return DoorRotate; }
	//�ǂ̃V�[���Ɉړ����邩�Q��
	int GetRed() { return OverRed; }
	//�ω������邩�Q��
	float GetChange() { return change; }
	//�o���ɓ����������Z�b�g
	void SetExit(bool a) { Exit = a; }
	//�G�f�B�b�g�V�[���ɍs�����Q��
	bool GetEdit() { return MapEdit; }
	//�`���[�g���A���̃t���O
	bool GetTutorial() { return Tutorial; }
	//�`���[�g���A���̃t���O
	bool GetTutorial_2() { return Tutorial_2; }

	void SetWalkTutorial(bool flag) { Walk_tutorial = flag; }

	bool GetMenuFlag() { return Menu_flag; }

	bool GetOptionFlag() { return Option_flag; }

	int GetMenuType() { return Menu_type; }

	float GetMouseX() { return mouseX; }
	float GetMouseY() { return mouseY; }
private:

	int Old_scene = 0;
	//�o��
	bool Exit;
	//���[�h
	bool Load = true;
	//�X�P�[��
	float scaleX = 1.0f;
	//
	float scaleY = 1.0f;
	//�G�f�B�b�g�t���O
	bool MapEdit = false;
	//�}�E�X���W
	float mouseX;
	//
	float mouseY;

	float padX;
	//
	float padY;
	//������Ԃ����邩�̑I��
	float TitleRed = 0;
	bool Tutorial = true;

	bool Menu_flag = false;

	int Menu_type = 0;

	bool Tutorial_2 = true;
	bool Walk_tutorial = true;
	int Tutorial_time = 0;
	int Tutorial_time_2 = 0;
	//input
	Input* input;
	//�V�[��
	int scene = 0;
	//��������
	int push = 0;
	//��[�ǂ̂�����
	int Time = 0;
	//�f��
	int Demo = 0;
	//
	float a;
	//���[�h�̎��ԂQ
	int Second = 0;
	//����
	float distance;
	//
	int flag;
	//�ǂ̐�
	int OverRed = 0;
    //�~�߂鎞��
	int StopTime;
	//�ǂꂾ���ω������邩
	float change = 0;
	//�ω��̃t���O
	bool changeFlag = false;
	//���[�h�̎��ԂR
	int LoadTime = 0;
	//�h�A�̉�]
	float DoorRotate = 0;
	// �}�E�X���W���擾����
	POINT p;

	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;

	int Pad_select = 0;

	bool Mouse_flag = true;

	bool Option_flag = false;


	int Old_iPad_left = 0, Old_iPad_right = 0, iOld_Pad_up = 0, Old_iPad_down = 0;
	int Old_iPad_leftshoulder = 0, Old_iPad_rightshoulder = 0;
	int Old_iPad_A = 0, Old_iPad_B = 0, Old_iPad_X = 0, Old_iPad_Y = 0;
	int Old_ipad_start = 0;

};

