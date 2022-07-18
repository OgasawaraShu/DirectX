#pragma once



#include <windows.h>
#include<wrl.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include<dinput.h>

#pragma comment (lib, "xinput.lib")
#include <xinput.h>


//入力
class Input //final
{
//private:
//	Input();
//	~Input();

//public:
//	Input(const Input& obj) = delete;
//	Input& operator=(const Input& obj) = delete;

//	static Input* GetInstance();
public:


	//namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバ関数
    //初期化
	void Intialize(WinApp* winApp);
	//更新
	void Update();

	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);

//	ゲームパッド

	void PadUpdate();

	
	//XINPUT_STATE state;

	//XINPUT_VIBRATION vibration;

private://メンバ変数
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//ゲームパッド
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//ゲームパッドのデバイス
	ComPtr<IDirectInputDevice8> gamepad;


	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE oldkey[256] = {};
	//パッド

private:
	//WindowsAPI
	WinApp* winApp = nullptr;

	//以下マウス
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};


	// キーの左ボタン押下をチェック
	bool PushMouseLeft();

	// キーの左ボタントリガーをチェック
	bool TriggerMouseLeft();

	// マウス移動量を取得
	MouseMove GetMouseMove();

	void MouseSetPosition();

private://メンバ変数
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
};

