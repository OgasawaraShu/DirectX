#pragma once
#include "Input.h"

class SceneSelect
{
public:
	SceneSelect(int window_width, int window_height, Input* input);

	void Initialize2();
	void Update();
	void MouseGet();
	void Title();
	void Scene1();
	void MapEditScene();

	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }

	int GetScene() { return scene; }
	int GetPush() { return push; }
	int GetTime() { return Time; }
	float GetDoorY() { return DoorRotate; }
	int GetRed() { return OverRed; }
	float GetChange() { return change; }
	void SetExit(bool a) { Exit = a; }
	bool GetEdit() { return MapEdit; }
private:

	// スケーリング
	bool Exit;
	bool Load = true;
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	bool MapEdit = false;
	float mouseX;
	float mouseY;

	float TitleRed = 0;

	Input* input;
	int scene = 0;
	int push = 0;
	int Time = 0;
	int Demo = 0;
	float a;
	int Second = 0;
	float distance;
	int flag;
	int OverRed = 0;
	int StopTime;
	float change = 0;
	bool changeFlag = false;
	int LoadTime = 0;
	float DoorRotate = 0;
	// マウス座標を取得する
	POINT p;

	//ウィンドウハンドル
	HWND hwnd = nullptr;

};

