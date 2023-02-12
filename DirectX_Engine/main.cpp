﻿#pragma GCC optimize("Ofast")
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
#include"Engine/3d/Model.h"
#include "Engine/Collision/Collision.h"
#include "Engine/Collision/CollisionPrimitive.h"
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include "Engine/Base/WinApp.h"
#include "Engine/Audio/Audio.h"
//#include"Engine/2d/SpriteCommon.h"
#include "Engine/2d/Sprite.h"
#include "Engine/2d/DebugText.h"
#include "Engine/2d/PostEffectCommon.h"
#include "Engine/2d/PostEffect.h"
#include "Engine/3d/FbxLoader.h"
#include "Engine/3d/Fbx3d.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Camera/DebugCamera.h"
#include "Engine/Input/GamePad.h"
#include <vector>
#include "Engine/Collision/SphereCollider.h"
#include "Engine/Collision/PlaneCollider.h"
#include "Engine/Collision/BoxCollider.h"
#include "Engine/Collision/Wallcolider.h"
#include "Engine/Collision/CollisionManager.h"
#include "GameOriginal/Game/PlayerFbx.h"
#include "GameOriginal/Game/BulletFbx.h"
#include "GameOriginal/Game/Physics.h" 
#include "GameOriginal/Game/ObjFbx.h"
#include "GameOriginal/Scene/SceneSelect.h"
#include "GameOriginal/Scene/GameScene.h"
#include "GameOriginal/Game/PortalExit.h"
#include "Engine/Camera/BlueCameraDebug.h"
#include "Engine/Camera/RedCameraDebug.h"
#include "GameOriginal/Particle/ParticleManager.h"
#include "Engine/Light/LightGroup.h"
#include "lib/nlohmann/json.hpp"
#include <fstream>
#include<map>
#include "GameOriginal/MapEdit/MapEdit.h"
#include "Engine/2d/DirectWrite.h"


SpriteCommon* spriteCommon = new SpriteCommon();
PostEffectCommon* posteffectCommon = new PostEffectCommon();
SpriteCommon* spriteCommon2= new SpriteCommon();

class CollisionManager;

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
//カメラ
DebugCamera* camera = nullptr;
BlueCameraDebug* Bluecamera = nullptr;
RedCameraDebug* Redcamera = nullptr;
//# Windowsアプリでのエントリーポイント(main関数)
#pragma optimize( "", off )
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //ポインタ置き場
    DirectXCommon* dxCommon = nullptr;
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    Audio* audio = nullptr;
  //  Physics* physics=nullptr;
    SceneSelect* scene = nullptr;
    //ParticleManager* particleMan = nullptr;
    //ParticleManager* particleManBlue = nullptr;
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
    scene = new SceneSelect(WinApp::window_width, WinApp::window_height, input);

    input->Intialize(winApp);

    /*
    camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
    Bluecamera= new BlueCameraDebug(WinApp::window_width, WinApp::window_height);
    Redcamera = new RedCameraDebug(WinApp::window_width, WinApp::window_height);

   //パーティクル
   // パーティクルマネージャ生成
   particleMan = ParticleManager::Create(dxCommon->GetDev(), camera);
   particleManBlue = ParticleManager::Create(dxCommon->GetDev(), camera);

   // 光線方向初期値
   float lightDir0[3] = { 0,0,1 };
   float lightColor0[3] = { 1,0,0 };

   float lightDir1[3] = { 0,1,0 };
   float lightColor1[3] = { 0,1,0 };

   float lightDir2[3] = { 1,0,0 };
   float lightColor2[3] = { 0,0,1 };
   // ライト静的初期化
   LightGroup::StaticInitialize(dxCommon->GetDev());
   LightGroup* lightGroup = nullptr;
   // ライト生成
   lightGroup = LightGroup::Create();

   Fbx3d::SetLightGroup(lightGroup);
   */
   //DirectWrite
   std::string keys="a";
  

    directWrite->DirectWritePre(keys);
    directWrite->registerTextFormat(keys, 44);
    directWrite->DirectWriteTextLoad();

   
    //読み込み

    //生成

    //ひもづけ

    //位置指定

    result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

    //マスターボイスを作成 
    result = xAudio2->CreateMasteringVoice(&masterVoice);
    /*
    //スプライト初期化  
   spriteCommon->Initialize(dxCommon->GetDev(),dxCommon->GetCmdList(),winApp->window_width,winApp->window_height);
   posteffectCommon->PostInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

    //ポストエフェクト用
   PostEffect* postEffect = PostEffect::PostCreate(posteffectCommon, 100);
   spriteCommon->SpriteCommonLoadTexture(100, L"Resources/Red.png");

    //スプライト
    Sprite* sprite = Sprite::Create(spriteCommon,0);

   spriteCommon-> SpriteCommonLoadTexture(0, L"Resources/Aiming.png"); 
   sprite->SetPosition({ 640,362.5,0 });
   sprite->SetSize({ 127,141 });
   sprite->SettexSize({ 127,141 });

   Sprite* sprite1 = Sprite::Create(spriteCommon, 1);

   spriteCommon->SpriteCommonLoadTexture(1, L"Resources/UI/NormalOP.png");
   sprite1->SetPosition({ 640,360,0 });
   sprite1->SetSize({ 1280,720 });
   sprite1->SettexSize({ 1280,720 });

   Sprite* sprite2 = Sprite::Create(spriteCommon, 4);

   spriteCommon->SpriteCommonLoadTexture(4, L"Resources/UI/StartOP.png");
   sprite2->SetPosition({ 640,360,0 });
   sprite2->SetSize({ 1280,720 });
   sprite2->SettexSize({ 1280,720 });

   Sprite* sprite3 = Sprite::Create(spriteCommon, 3);

   spriteCommon->SpriteCommonLoadTexture(3, L"Resources/UI/ExitOP.png");
   sprite3->SetPosition({ 640,360,0 });
   sprite3->SetSize({ 1280,720 });
   sprite3->SettexSize({ 1280,720 });

   Sprite* sprite4 = Sprite::Create(spriteCommon, 5);

   spriteCommon->SpriteCommonLoadTexture(5, L"Resources/UI/debug_rule.png");
   sprite4->SetPosition({ 150,150,0 });
   sprite4->SetSize({ 300,300 });
   sprite4->SettexSize({ 300,300 });

   Sprite* sprite5 = Sprite::Create(spriteCommon, 6);

   spriteCommon->SpriteCommonLoadTexture(6, L"Resources/UI/NameOP.png");
   sprite5->SetPosition({ 640,360,0 });
   sprite5->SetSize({ 1280,720 });
   sprite5->SettexSize({ 1280,720 });

   Sprite* sprite6 = Sprite::Create(spriteCommon, 7);

   spriteCommon->SpriteCommonLoadTexture(7, L"Resources/UI/Change.png");
   sprite6->SetPosition({ 640,360,0 });
   sprite6->SetSize({ 1280,720 });
   sprite6->SettexSize({ 1280,720 });


    //デバックテキスト
    DebugText* debugtext=nullptr;
    debugtext = new DebugText();

    DebugText* debugtext2 = nullptr;
    debugtext2 = new DebugText();


    const int debugTextTexNumber = 2;

    spriteCommon->SpriteCommonLoadTexture(debugTextTexNumber,L"Resources/ASC_white1280.png");
    debugtext->debugTextInit(spriteCommon,debugTextTexNumber);
    debugtext2->debugTextInit(spriteCommon, debugTextTexNumber);
    // DirectX初期化処理　ここまで

    FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
 */

    //オーディオ初期化
    audio = new Audio();
    audio->Initialize();

    //パイプライン生成
 /*
    BYTE key[256] = {};
    BYTE olds[256] = {};

    int debug_x = 600;
    int debug_y = 80;

    int debug2_x = 300;
    int debug2_y = 24;

    //ゲームシーン処理関連
    char moji[64];
    char moji2[64];

    BulletFbx* fbx3d3 = nullptr;
    BulletFbx* fbx3d4 = nullptr;
    PlayerFbx* fbx3d9 = nullptr;
    PortalExit* fbx3d23 = nullptr;
    PortalExit* fbx3d24 = nullptr;
    Fbx3d* fbx3d34 = nullptr;
    Fbx3d* fbx3d36 = nullptr;
    Fbx3d* fbx3d38 = nullptr;



   Model* model1 = nullptr;
   Model* model2 = nullptr;
   Model* model3 = nullptr;
   Model* model4 = nullptr;
   Model* model5 = nullptr;
   Model* model6 = nullptr;
   Model* model7 = nullptr;
   Model* model8= nullptr;
   Model* model9 = nullptr;
   Model* model10 = nullptr;
   Model* model11 = nullptr;
   Model* model12 = nullptr;
   Model* model13 = nullptr;
   Model* model13_2 = nullptr;
   Model* model15 = nullptr;
   Model* model16 = nullptr;
   Model* model17 = nullptr;
   Model* model14 = nullptr;
   Model* model18 = nullptr;
   Model* model19 = nullptr;
   Model* model20 = nullptr;
   Model* model21 = nullptr;
   Model* model22 = nullptr;
   Model* model23 = nullptr;
   Model* model24 = nullptr;
   Model* model25 = nullptr;
   Model* model26 = nullptr;
   Model* model27 = nullptr;
   Model* model28 = nullptr;
   Model* model29 = nullptr;
   Model* model30 = nullptr;

   model1 = FbxLoader::GetInstance()->LoadModelFromFile("wall1");
   model2 = FbxLoader::GetInstance()->LoadModelFromFile("Door");
   model3 = FbxLoader::GetInstance()->LoadModelFromFile("blueBall");
   model4 = FbxLoader::GetInstance()->LoadModelFromFile("redBall");
   model5 = FbxLoader::GetInstance()->LoadModelFromFile("wall2");
   model6 = FbxLoader::GetInstance()->LoadModelFromFile("wall");
   model7 = FbxLoader::GetInstance()->LoadModelFromFile("wall3");
   model8 = FbxLoader::GetInstance()->LoadModelFromFile("IronWall");
   model9 = FbxLoader::GetInstance()->LoadModelFromFile("blueBall");
   model10 = FbxLoader::GetInstance()->LoadModelFromFile("Fence");
   model11 = FbxLoader::GetInstance()->LoadModelFromFile("drum");
   model12 = FbxLoader::GetInstance()->LoadModelFromFile("PortalGun");
   model13 = FbxLoader::GetInstance()->RenLoadModelFromFile("PortalExit");
   model13_2 = FbxLoader::GetInstance()->RenLoadModelFromFile2("PortalExit");
   model14 = FbxLoader::GetInstance()->LoadModelFromFile("stand");
   model15 = FbxLoader::GetInstance()->LoadModelFromFile("movepepar");
   model16 = FbxLoader::GetInstance()->LoadModelFromFile("mousepepar");
   model17 = FbxLoader::GetInstance()->LoadModelFromFile("cubebox");
   model18 = FbxLoader::GetInstance()->LoadModelFromFile("PortalFrameOrange");
   model19 = FbxLoader::GetInstance()->LoadModelFromFile("exit");
   model20 = FbxLoader::GetInstance()->LoadModelFromFile("Door_");
   model21 = FbxLoader::GetInstance()->LoadModelFromFile("Door_not");
   model22 = FbxLoader::GetInstance()->LoadModelFromFile("portalpepar");
   model23 = FbxLoader::GetInstance()->LoadModelFromFile("Grid");
   model24 = FbxLoader::GetInstance()->LoadModelFromFile("CollisionBox");
   model25 = FbxLoader::GetInstance()->LoadModelFromFile("Respawn");
   model26 = FbxLoader::GetInstance()->LoadModelFromFile("floor");
   model27 = FbxLoader::GetInstance()->LoadModelFromFile("walls");
   model28 = FbxLoader::GetInstance()->LoadModelFromFile("RedArow");
   model29 = FbxLoader::GetInstance()->LoadModelFromFile("GreenArow");
   model30 = FbxLoader::GetInstance()->LoadModelFromFile("BlueArow");
   
   //マップエディット
   MapEdit* mapedit = nullptr;
   mapedit = new MapEdit(WinApp::window_width, WinApp::window_height, input);
   mapedit->SetFenceModel(model10);
   mapedit->SetWallModel(model27);
   mapedit->SetFloorModel(model26);
   mapedit->SetDrumModel(model11);
   mapedit->SetCollisionModel(model24);
   mapedit->SetExitModel(model2);
   mapedit->SetGunModel(model12);
   mapedit->SetBoxModel(model17);
   mapedit->SetRespawnModel(model25);
   mapedit->SetRedArowModel(model28);
   mapedit->SetGreenArowModel(model29);
   mapedit->SetBlueArowModel(model30);

   mapedit->Loadtxt();
  
   MapEdit::SetMapCamera(camera);
   //モデル読み込み
   Fbx3d::SetDevice(dxCommon->GetDev());
   Fbx3d::SetCamera(camera);
   Fbx3d::SetBlueCamera(Bluecamera);
   Fbx3d::SetRedCamera(Redcamera);
   Fbx3d::CreateGraphicsPipeline();

  //3Dオブジェクト生成とモデルのセット
  fbx3d3 = new BulletFbx(input);
  fbx3d3->Initialize();
  fbx3d3->SetModel(model3);

  fbx3d4 = new BulletFbx(input);
  fbx3d4->Initialize();
  fbx3d4->SetModel(model4);

  fbx3d9= new PlayerFbx(input,physics);
  fbx3d9->Initialize();
  fbx3d9->SetModel(model14);

  fbx3d23 = new PortalExit(input);
 // fbx3d23->PortalCreateGraphicsPipeline();
  fbx3d23->RenderFbxInitialize();
  fbx3d23->SetModel(model13);

  fbx3d24 = new PortalExit(input);
 // fbx3d24->PortalCreateGraphicsPipeline();
  fbx3d24->RenderFbxInitialize();
  fbx3d24->SetModel(model13_2);


  fbx3d34 = new Fbx3d(input);
  fbx3d34->Initialize();
  fbx3d34->SetModel(model20);


  fbx3d36 = new Fbx3d(input);
  fbx3d36->Initialize();
  fbx3d36->SetModel(model21);


  fbx3d38 = new Fbx3d(input);
  fbx3d38->Initialize();
  fbx3d38->SetModel(model23);

  fbx3d3->SetScale({ 0.05, 0.05,0.05 });
  fbx3d4->SetScale({ 0.05, 0.05,0.05 });

  fbx3d3->SetPosition({ 0, 0, -20 });
  fbx3d9->SetPosition({ 0, 0, -20 });

  fbx3d23->SetPosition({ 0,0,0 });
  fbx3d23->SetScale({ 0.1,0.1,0.1});
  fbx3d23->SetRotate({ 0,0,270 });

  fbx3d24->SetPosition({ 0,0,0 });
  fbx3d24->SetScale({ 0.1,0.1,0.1 });
  fbx3d24->SetRotate({ 0,0,270 });

  fbx3d34->SetPosition({ 300,10,60 });
  fbx3d34->SetScale({ 0.1,0.1, 0.1 });
  fbx3d34->SetRotate({ 0,0,180 });

  fbx3d36->SetPosition({ 307,10,60 });
  fbx3d36->SetScale({ 0.1,0.1, 0.1 });
  fbx3d36->SetRotate({ 0,0,180 });

  fbx3d38->SetPosition({ 0,0,0 });
  fbx3d38->SetScale({ 1,1,1 });
  fbx3d38->SetRotate({ 0,0,0 });

  //衝突マネージャー
  CollisionManager* collisionManager = nullptr;
  collisionManager = CollisionManager::GetInstance();

  float radius = 5.0f;
  float PlayerRadius = 1.0f;

  fbx3d3->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d4->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d9->SetColider(new SphereCollider(XMVECTOR{ 0,PlayerRadius,0,0 }, PlayerRadius));

  //当たり判定の属性
  fbx3d3->SetVerBulletRed();
  fbx3d4->SetVerBulletBlue();
  fbx3d9->SetVer();

  

  fbx3d9->PlayAnimation2();


  mapedit->SetHwnd(winApp->GetHwnd());
  int c = 0;
  int portaltime = 0;
  XMVECTOR mo;
  XMFLOAT3 posi;
  */

gameScene = new GameScene();
gameScene->SceneInitialize(dxCommon, input, audio, winApp);

      scene->SetHwnd(winApp->GetHwnd());
    while (true)  // ゲームループ
    {
     /*
        if (scene->GetScene() == -1)
        {

            fbx3d3->Initialize2();
            fbx3d4->Initialize2();
            fbx3d9->Initialize2();
    
            scene->SceneInitialize();
            camera->Initialize2();

            fbx3d3->SetScale({ 0.05, 0.05,0.05 });
            fbx3d4->SetScale({ 0.05, 0.05,0.05 });

            fbx3d3->SetPosition({ 0, 0, -20 });
            fbx3d9->SetPosition({ 0, 0, -20 });

            fbx3d23->SetPosition({ 0,0,0 });
            fbx3d23->SetScale({ 0.1,0.1,0.1 });
            fbx3d23->SetRotate({ 0,0,270 });

            fbx3d24->SetPosition({ 0,0,0 });
            fbx3d24->SetScale({ 0.1,0.1,0.1 });
            fbx3d24->SetRotate({ 0,0,270 });

            fbx3d34->SetPosition({ 300,10,60 });
            fbx3d34->SetScale({ 0.1,0.1, 0.1 });
            fbx3d34->SetRotate({ 0,0,180 });

            fbx3d36->SetPosition({ 307,10,60 });
            fbx3d36->SetScale({ 0.1,0.1, 0.1 });
            fbx3d36->SetRotate({ 0,0,180 });
        }
        if (fbx3d9->GetColision() == false)
        {
            posi = camera->GetEye();
        }


        scene->MapEditScene();

        //座標Set関連   
        mapedit->SetMyPosition(fbx3d9->GetMyPosition());
        mapedit->SetCameraAxisZ(camera->GetCameraZAxis());
        mapedit->SetTarget(camera->GetTargetPos());

        fbx3d23->SetMyposition(fbx3d9->GetMyPosition());
        fbx3d24->SetMyposition(fbx3d9->GetMyPosition());

        fbx3d3->SetMove(fbx3d9->GetMove());
        fbx3d4->SetMove(fbx3d9->GetMove());
        fbx3d3->SetPos(camera->GetPos());
        fbx3d4->SetPos(camera->GetPos());
       
        camera->SetWarpPosition(fbx3d9->GetPosition());
        camera->SetGround(fbx3d9->Getground());
        camera->SetScene(scene->GetScene());

        mapedit->SetShotBlue(fbx3d4->GetShot());
        mapedit->SetShotRed(fbx3d3->GetShot2());
        mapedit->SetWark(fbx3d9->GetWark());

        fbx3d23->GetFlag(fbx3d3->GetWarpFlag());
        fbx3d23->GetExitPosition(fbx3d3->GetPosition());
      
        fbx3d24->GetFlag(fbx3d4->GetWarpFlag2());
        fbx3d24->GetExitPosition(fbx3d4->GetPosition());
       
        Bluecamera->SetEyePos(fbx3d24->GetMyPosition());
        Redcamera->SetEyePos(fbx3d23->GetMyPosition());
     */
     //sprintf_s(moji2, "camera=%f", camera->GetPositionY());
     //sprintf_s(moji2,"%d",camera->GetAngleY());
     
        if (winApp->ProcessMessage())
        {
            //ゲームループを抜ける
            break;
        }
        /*
        // DirectX毎フレーム処理　ここから
      
        fbx3d36->SetRotate({ 0,scene->GetDoorY(),180});

    
        //初期化処理

        //ゲーム内の動作  
      
        //更新
       
        if (scene->GetScene() == 0)
        {

            scene->MouseGet();
            scene->Title();


        }
        else
        {
            scene->ChangeScene();
        }
        lightGroup->Update();
        input->Update();
      
        camera->SetColision(fbx3d9->GetColision());
        camera->SetMove(fbx3d9->GetMove());
        camera->SetColisionVec(fbx3d9->GetColisionVec());
        if (scene->GetEdit() == false)camera->Update();
 
        sprintf_s(moji2, "%f", camera->GetPositionX());
        fbx3d9->SetMemo(fbx3d4->GetMemo());
        fbx3d9->SetWorld(camera->GetRot());
        fbx3d9->SetMemo2(fbx3d3->GetMemo2());
        fbx3d9->SetWarpFlag(fbx3d3->GetWarpFlag());
        fbx3d9->SetWarpFlag2(fbx3d4->GetWarpFlag2());
        fbx3d9->SetCameraAxis(camera->GetCameraZAxis());
        fbx3d9->SetPos(camera->GetEye());
        fbx3d9->GetmoveOld(camera->GetMove());

        //追加
        fbx3d9->CollisionAfter();
      
        int i = 0;


        if (scene->GetEdit() == false)
        {
            while (1)
            {
                collisionManager->CheckAllCollisions();
                if (fbx3d9->GetColision() == true)
                {
                    camera->SetColision(fbx3d9->GetColision());
                    if (i < 4)
                    {
                        camera->SetMove(fbx3d9->GetMove());
                    }
                    else if (i % 4 == 0)
                    {
                        camera->SetMove(camera->GetMoveOld());
                        mo = camera->GetMoveOld();
                    }
                    camera->SetColisionVec(fbx3d9->GetColisionVec());
                    camera->Update();
                    fbx3d9->SetPos(camera->GetEye2());
                    fbx3d9->GetmoveOld(camera->GetMove());
                    fbx3d9->PlayerUpdate(camera->GetAngleX(), camera->GetAngleY());
                    fbx3d9->CollisionAfter();
                    i += 1;
                    c = i;
                }
                else
                {
                    break;
                }

                if (i > 14 && fbx3d9->Getground() == true)
                {

                    camera->SetEye(posi);
                    fbx3d9->SetPos(camera->GetPos());
                }

                if (i > 60)
                {
                    break;
                }
            }

            Bluecamera->Update();
            Redcamera->Update();
            fbx3d3->SetCameraT(camera->GetTargetPos());
            fbx3d4->SetCameraT(camera->GetTargetPos());

            //後で変える
            fbx3d3->SetGet(mapedit->GetGetGun());
            fbx3d4->SetGet(mapedit->GetGetGun());




            fbx3d3->BlueBulletUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d4->RedBulletUpdate(camera->GetAngleX(), camera->GetAngleY());

            camera->SetBlueAngle(fbx3d3->GetMemoB());
            camera->SetRedAngle(fbx3d4->GetMemoR());
            fbx3d9->PlayerUpdate(camera->GetAngleX(), camera->GetAngleY());




            sprintf_s(moji, "%d", fbx3d9->GetDebug());

            fbx3d23->ExitUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d24->ExitUpdate(camera->GetAngleX(), camera->GetAngleY());

             //追加
            input->GetScene(scene->GetScene());
            fbx3d23->ExitUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d34->Update();
            fbx3d36->Update();
            camera->SetRedTeleport(fbx3d9->GetredTeleport());
            camera->SetAngleBlueY(fbx3d3->GetAngleY1());
            camera->SetBlueTeleport(fbx3d9->GetblueTeleport());
            camera->SetBlue(fbx3d23->GetRot());
            camera->SetRed(fbx3d24->GetRot());
            Bluecamera->SetRot(fbx3d24->GetRot());
            Redcamera->SetRot(fbx3d23->GetRot());



            // パーティクル生成
            if (fbx3d4->RedP_Attack() == true) particleMan->CreateParticles(fbx3d4->GetPosition());
            if (fbx3d3->BlueP_Attack() == true) particleManBlue->CreateParticles(fbx3d3->GetPosition());

            particleMan->Update();
            particleManBlue->BlueUpdate();

            scene->SetExit(fbx3d9->GetExit());
        }
        else
        {
        fbx3d38->Update();
        camera->MapEditUpdate();
        }
        //FBX描画
        fbx3d23->RenPreDraw(dxCommon->GetCmdList());
 

        if (fbx3d3->GetWarpFlag() == true && fbx3d4->GetWarpFlag2() == true)
        {

        }
        else
        {
            portaltime = 0;
        }
      
        fbx3d23->RenPostDraw(dxCommon->GetCmdList());

        fbx3d24->RenPreDraw2(dxCommon->GetCmdList());
        if (fbx3d3->GetWarpFlag() == true && fbx3d4->GetWarpFlag2() == true)
        {
          
        }
        fbx3d24->RenPostDraw2(dxCommon->GetCmdList());

        //レンダ―テクスチャの描画
        postEffect->PreDrawScene(dxCommon->GetCmdList());


        //バックバッファの番号を取得（2つなので0番か1番）
        dxCommon->ImguiPre();

        if (scene->GetEdit() == false)
        {
            postEffect->SetPost(true);

            if (scene->GetScene() != 99)
            {

               
                mapedit->LoadSet(dxCommon->GetCmdList());


                fbx3d23->RenderFbxDraw(dxCommon->GetCmdList());
                fbx3d24->RenderFbxDraw2(dxCommon->GetCmdList());

            }
            fbx3d34->Draw2(dxCommon->GetCmdList());
            fbx3d36->Draw2(dxCommon->GetCmdList());
        }
        else
        {
         
            postEffect->SetPost(false);
            fbx3d38->Draw2(dxCommon->GetCmdList());
            mapedit->CreateObj(dxCommon->GetCmdList());
        }

         // パーティクルの描画
        particleMan->Draw(dxCommon->GetCmdList());
        particleManBlue->Draw(dxCommon->GetCmdList());
        //ポストエフェクトここまで
        postEffect->PostDrawScene(dxCommon->GetCmdList());

        dxCommon->PreDraw();
     
        posteffectCommon->PostPreDraw();
        postEffect->PostUpdate();
     
        postEffect->PostDraw();
     */
        if (input->PushKey(DIK_ESCAPE))
        {
            break;
        }
        /*
        ////スプライト共通コマンド
        spriteCommon->PreDraw();
    //    debugtext->Print(moji, 100, 100);
    //    debugtext->DrawAll();//的カウント
//
  //     debugtext2->Print(moji2, 100, 200);
  //     debugtext2->DrawAll();//的カウント


        //スプライト表示
        if (scene->GetScene() == 0)
        {

            sprite5->SpriteTransVertexBuffer();
            sprite5->Update();
            sprite5->SpriteDraw();

            if (scene->GetRed() == 0)
            {
                sprite1->SpriteTransVertexBuffer();
                sprite1->Update();
                sprite1->SpriteDraw();
            }
            else  if (scene->GetRed() == 1)
            {
                sprite2->SpriteTransVertexBuffer();
                sprite2->Update();
                sprite2->SpriteDraw();
            }
            else  if (scene->GetRed() == 2)
            {
                sprite3->SpriteTransVertexBuffer();
                sprite3->Update();
                sprite3->SpriteDraw();
            }
        }
        else  if (scene->GetScene() == 1)
        {
            //自キャラ
            sprite->SpriteTransVertexBuffer();
            sprite->Update();
            sprite->SpriteDraw();
        }
        
        
      

        sprite6->SpriteTransVertexBuffer();
        sprite6->Update();
        sprite6->SpriteDraw();
        sprite6->SetColor({ 1,1,1,scene->GetChange() });
        if (fbx3d9->GetColision() == false&&scene->GetEdit()==false)camera->ColisionAfterCameraSet(fbx3d9->GetMyPosition());

        // ４．描画コマンドここまで
        collisionManager->CheckAllCollisions();
        */

        gameScene->SceneUpdate();
        gameScene->SceneDraw();



        if (scene->GetScene() == 100)
        {
            break;
        }
        // DirectX毎フレーム処理　ここまで
        dxCommon->PostDrawPreDirectWrite();
        directWrite->SetSwapChain(dxCommon->GetSwapChain());
        directWrite->DirectWriteText();
        dxCommon->PostDrawPostDirectWrite();
    }
    //3D解放

    //入力開放
    delete input;

    //DirectX解放
    delete dxCommon;


    winApp->Finalize();

    //WindowsAPI解放
    delete winApp;
    winApp = nullptr;

   //3Dモデル解放

    //スプライト
    delete spriteCommon;
    delete posteffectCommon;

   // delete sprite;

    //テキスト解放
    //delete debugtext;

    audio->Finalize();
    delete audio;

    //delete model1;

    FbxLoader::GetInstance()->Finalize();
    return 0;
}
#pragma optimize( "", on )