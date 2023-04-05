#pragma once
#include "../../Engine/Input/Input.h"
#include "../../Engine/Input/GamePad.h"

class SceneSelect
{
public:
	SceneSelect(int window_width, int window_height, Input* input);
	//初期化
	void SceneInitialize();
	//更新
	void Update();
	//マウス取得
	void MouseGet();
	//タイトルシーン
	void Title();
	//遷移処理
	void ChangeScene();
	//ステージエディット
	void MapEditScene();
	//メニューシーン
	void MenuScene();
	//アフター
	void PadScene();

	//hwndセット
	void SetHwnd(HWND hwnd_) { hwnd = hwnd_; }
	//デバッグ
	float GetMousePoint() { return mouseX; }
	//シーン参照
	int GetScene() { return scene; }

	int OldScene() { return Old_scene; }
	//押したか参照
	int GetPush() { return push; }
	//ジカン参照
	int GetTime() { return Time; }
	//ドアがどれだけ回転したか
	float GetDoorY() { return DoorRotate; }
	//どのシーンに移動するか参照
	int GetRed() { return OverRed; }
	//変化させるか参照
	float GetChange() { return change; }
	//出口に当たったかセット
	void SetExit(bool a) { Exit = a; }
	//エディットシーンに行くか参照
	bool GetEdit() { return MapEdit; }
	//チュートリアルのフラグ
	bool GetTutorial() { return Tutorial; }
	//チュートリアルのフラグ
	bool GetTutorial_2() { return Tutorial_2; }

	void SetWalkTutorial(bool flag) { Walk_tutorial = flag; }

	bool GetMenuFlag() { return Menu_flag; }

	bool GetOptionFlag() { return Option_flag; }

	int GetMenuType() { return Menu_type; }

	float GetMouseX() { return mouseX; }
	float GetMouseY() { return mouseY; }
private:

	int Old_scene = 0;
	//出口
	bool Exit;
	//ロード
	bool Load = true;
	//スケール
	float scaleX = 1.0f;
	//
	float scaleY = 1.0f;
	//エディットフラグ
	bool MapEdit = false;
	//マウス座標
	float mouseX;
	//
	float mouseY;

	float padX;
	//
	float padY;
	//文字を赤くするかの選択
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
	//シーン
	int scene = 0;
	//押したか
	int push = 0;
	//ろーどのじかん
	int Time = 0;
	//デモ
	int Demo = 0;
	//
	float a;
	//ロードの時間２
	int Second = 0;
	//距離
	float distance;
	//
	int flag;
	//どの赤
	int OverRed = 0;
    //止める時間
	int StopTime;
	//どれだけ変化させるか
	float change = 0;
	//変化のフラグ
	bool changeFlag = false;
	//ロードの時間３
	int LoadTime = 0;
	//ドアの回転
	float DoorRotate = 0;
	// マウス座標を取得する
	POINT p;

	//ウィンドウハンドル
	HWND hwnd = nullptr;

	int Pad_select = 0;

	bool Mouse_flag = true;

	bool Option_flag = false;


	int Old_iPad_left = 0, Old_iPad_right = 0, iOld_Pad_up = 0, Old_iPad_down = 0;
	int Old_iPad_leftshoulder = 0, Old_iPad_rightshoulder = 0;
	int Old_iPad_A = 0, Old_iPad_B = 0, Old_iPad_X = 0, Old_iPad_Y = 0;
	int Old_ipad_start = 0;

};

