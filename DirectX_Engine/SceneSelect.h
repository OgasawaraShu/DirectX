#pragma once
#include "Input.h"

class SceneSelect
{
public:
	SceneSelect(int window_width, int window_height, Input* input);

	void Initialize();
	void Update();
	void MouseGet();
	void Title();

	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }

	int GetScene() { return scene; }
	int GetPush() { return push; }
	int GetTime() { return Time; }
	int GetRed() { return OverRed; }

private:

	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	float mouseX;
	float mouseY;

	float TitleRed = 0;

	Input* input;
	int scene = 0;
	int push = 0;
	int Time = 0;
	float a;
	int Second = 0;
	float distance;
	int flag;
	int OverRed = 0;
	int StopTime;

	// マウス座標を取得する
	POINT p;

	//ウィンドウハンドル
	HWND hwnd = nullptr;

};

