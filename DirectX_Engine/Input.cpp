#include "Input.h"


//#include<wrl.h>
//#include<dinput.h>
//using namespace Microsoft::WRL;

//#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


//Input* Input::GetInstance()
//{/
//
//    static Input instance;

//    return &instance;
//}

Input* Input::GetInstance()
{
    static Input instance;

    return &instance;
}

void Input::Intialize(WinApp* winApp)
{
    HRESULT result;


    //DirectInputのインスタンス生成
    //ComPtr<IDirectInput8> dinput = nullptr;
    //借りてきたAppのインスタンスを記録
    this->winApp = winApp;

    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
    //キーボード生成
    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
    //入力データの形式セット
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
    //排他制御レベルのセット
    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);



    //ゲームパッド
    //result=dinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
    //   NULL, DIEDFL_ATTACHEDONLY);


}

/*
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
    HRESULT hr;

    hr =dinput->CreateDevice(pdidInstance->guidInstance, &Input::gamepad, NULL);

    if (FAILED(hr)) return DIENUM_CONTINUE;

    return DIENUM_STOP;
}
*/
void Input::Update()
{
    HRESULT result;

    //前回のキー入力を保存
   // memcpy(oldkey, key, sizeof(key));

    //キーボード情報の取得
    result = devkeyboard->Acquire();
    //全キーの入力情報を取得
   // BYTE key[256] = {};
    memcpy(oldkey, key, sizeof(key));

    result = devkeyboard->GetDeviceState(sizeof(key), key);

}

bool Input::PushKey(BYTE keyNumber)
{
    //指定キーを押していればtrueを返す
    if (key[keyNumber])
    {
        return true;
    }

    //そうでなければfalseを返す
    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
    //前回押していなく、今回押していればtrueを返す
    if (key[keyNumber] && !oldkey[keyNumber])
    {
        return true;
    }

    //そうでない場合、falseを返す
    return false;
}

void Input::PadUpdate()
{
    XINPUT_STATE state;
    XInputGetState(0, &state);


    int iPad_left = 0, iPad_right = 0, iPad_up = 0, iPad_down = 0;
    int iPad_leftshoulder = 0, iPad_rightshoulder = 0;
    int iPad_A = 0, iPad_B = 0, iPad_X = 0, iPad_Y = 0;

    XINPUT_VIBRATION vibration;
  
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) iPad_left = 1;//ゲームパッド十字キー左
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) iPad_right = 1;//ゲームパッド十字キー右
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) iPad_up = 1;//ゲームパッド十字キー上
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) iPad_down = 1;//ゲームパッド十字キー下
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) iPad_leftshoulder = 1;//ゲームパッドL
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) iPad_rightshoulder = 1;//ゲームパッドR
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) iPad_A = 1;//ゲームパッドA
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) iPad_B = 1;//ゲームパッドB
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) iPad_X = 1;//ゲームパッドX
    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) iPad_Y = 1;//ゲームパッドY
    //ゲームパッドアナログスティックのデッドゾーン処理
    if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;
    }

 
    vibration.wLeftMotorSpeed = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(0, &vibration);


}
