#pragma once
#include "../../Engine/Input/Input.h"

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
	//hwnd�Z�b�g
	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }
	//�f�o�b�O
	float GetMousePoint() { return mouseX; }
	//�V�[���Q��
	int GetScene() { return scene; }
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
private:

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
	//������Ԃ����邩�̑I��
	float TitleRed = 0;
	bool Tutorial = true;

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

};
