#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

#include <stdio.h>
#include <stdlib.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include <cstdlib>      // srand,rand
#include <time.h>
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include<DirectXTex.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#include<d3dx12.h>
#include "Engine/Input/Input.h"
#include "Engine/Base/WinApp.h"
#include "Engine/Base/DirectXCommon.h"
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include "Engine/Base/WinApp.h"
#include "Engine/Audio/Audio.h"
#include "GameOriginal/Scene/GameScene.h"
#include "Engine/2d/DirectWrite.h"
#include <imgui.h>


LRESULT CALLBACK WindowProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        _stprintf( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // 空実装
#endif

//# Windowsアプリでのエントリーポイント(main関数)
#pragma optimize( "", off )
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //ポインタ置き場
    DirectXCommon* dxCommon = nullptr;
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    Audio* audio = nullptr;
    DirectWrite* directWrite = nullptr;
    GameScene* gameScene = nullptr;
    directWrite = new DirectWrite();

    //WindowsAPIの初期化
    winApp = new WinApp();
    winApp->Initialize();

// DirectX初期化処理　ここから
    ComPtr<IXAudio2> xAudio2;
    IXAudio2MasteringVoice* masterVoice;

    //DirectXの初期化
    dxCommon = new DirectXCommon();
    dxCommon->SetHwnd(winApp->GetHwnd());
    dxCommon->PreDirectWriteInitialize(winApp);

    //ここにWrite初期化
    directWrite->SetCmdQueue(dxCommon->GetCmdQueue());
    directWrite->SetDev(dxCommon->GetDev());
    directWrite->DirectWriteInitialize();
    dxCommon->PostDirectWriteInitialize();

    //ここRender
    directWrite->SetHwnd(winApp->GetHwnd());
    directWrite->SetBackbuffersSize0(dxCommon->GetBackBuffersSize0());
    directWrite->SetBackbuffersSize1(dxCommon->GetBackBuffersSize1());
    directWrite->DirectWriteRenderInitialize();
    dxCommon->PostRenderDirectWriteInitialize();

    HRESULT result;
     
    input = new Input();
    input->Intialize(winApp);

    //DirectWrite
    std::string keys="a";
    directWrite->DirectWritePre(keys);
    directWrite->registerTextFormat(keys, 44);
    directWrite->DirectWriteTextLoad();

    result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    //マスターボイスを作成 
    result = xAudio2->CreateMasteringVoice(&masterVoice);
    //オーディオ初期化
    audio = new Audio();
    audio->Initialize();

    gameScene = new GameScene();
    gameScene->SceneInitialize(dxCommon, input, audio, winApp);

    int demo = 0;
    while (true)  // ゲームループ
    {
        //ゲームシーン更新
        gameScene->SceneUpdate();
    


        if (winApp->ProcessMessage())
        {
            //ゲームループを抜ける
            break;
        }
        //ESCAPE押されたら終了
        if (input->PushKey(DIK_ESCAPE))
        {
            break;
        }
        //EXITを押されたら終了
        if (gameScene->GetGameEnd() == true)
        {
            break;
        }

        //ゲームシーン描画
        gameScene->SceneDraw();

        // DirectX毎フレーム処理　ここまで
        dxCommon->PostDrawPreDirectWrite();
        directWrite->SetSwapChain(dxCommon->GetSwapChain());
        directWrite->DirectWriteText();
        dxCommon->PostDrawPostDirectWrite();
    }
    //入力開放
    delete input;
    //DirectX解放
    delete dxCommon;

    winApp->Finalize();
    //WindowsAPI解放
    delete winApp;
    winApp = nullptr;
    //3Dモデル解放
    audio->Finalize();
    delete audio;

    FbxLoader::GetInstance()->Finalize();
    return 0;
}
#pragma optimize( "", on )