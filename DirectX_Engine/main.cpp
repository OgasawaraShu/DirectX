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

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "3d/Object3d.h"
#include"Model.h"


#include "Collision.h"
#include "CollisionPrimitive.h"


#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>

#include "WinApp.h"
#include "Audio.h"
#include"SpriteCommon.h"
#include "Sprite.h"
#include "DebugText.h"

#include "Player.h"
#include "Enemy.h"
#include "Item.h"

//#include "FbxLoader.h"
#include "3d/FbxLoader.h"
#include "Fbx3d.h"
#include "Camera.h"
#include "DebugCamera.h"

#include "PortalGun.h"
#include "GamePad.h"
//#include "fbxsdk.h"
#include <vector>

#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "PlayerFbx.h"
#include "BulletFbx.h"
#include "Physics.h" 
#include"ObjFbx.h"

Model* modelPlane = nullptr;
Model* modelBox = nullptr;
Model* modelPyramid = nullptr;

std::vector<Object3d*> objects;


Sphere sphere;

Plane plane;

Triangle triangle;


//
DirectXCommon* dxCommon = nullptr;
SpriteCommon* spriteCommon = new SpriteCommon();
SpriteCommon* spriteCommon2= new SpriteCommon();

class CollisionManager;
class Bullet;

//ComPtr<ID3D12Device> dev;

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
//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //FbxManager* fbxManager = FbxManager::Create();

    //ポインタ置き場
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    Audio* audio = nullptr;
    GamePad* gamepad = nullptr;
    Physics* physics=nullptr;
    //WindowsAPIの初期化
    winApp = new WinApp();
    winApp->Initialize();

    gamepad = new GamePad();
   
// DirectX初期化処理　ここから

    ComPtr<IXAudio2> xAudio2;
    IXAudio2MasteringVoice* masterVoice;

    //DirectXの初期化
    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);
    HRESULT result;
     


     input = new Input();
     physics = new Physics();

    //入力の初期化
  //  Input* input = Input::GetInstance();
  
    input->Intialize(winApp);


 
    camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

    //3D初期化
   Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);


    //FBX
 //   Fbx3d::SetDevice(dxCommon->GetDev());

  //  Fbx3d::SetCamera(camera);

  //  Fbx3d::CreateGraphicsPipeline();

    //3Dオブジェクト生成とモデルのセット
 //   fbx3d1 = new Fbx3d;
 //   fbx3d1->Initialize();
 //   fbx3d1->SetModel(model1);
    //読み込み

    //生成

    //ひもづけ

    //位置指定

    result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

    //マスターボイスを作成 
    result = xAudio2->CreateMasteringVoice(&masterVoice);
    
    //スプライト初期化
  
   spriteCommon->Initialize(dxCommon->GetDev(),dxCommon->GetCmdList(),winApp->window_width,winApp->window_height);
   spriteCommon->Initialize_Post(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);



    //ポストエフェクト用
  //Sprite*sprite=Sprite::Create(100, "Resources/Red.png");
   Sprite* sprite100 = Sprite::PostCreate(spriteCommon, 100);
   spriteCommon->SpriteCommonLoadTexture(100, L"Resources/Red.png");

    //スプライト
    Sprite* sprite = Sprite::Create(spriteCommon,0);

   spriteCommon-> SpriteCommonLoadTexture(0, L"Resources/Aiming.png"); 
   sprite->SetPosition({ 640,362.5,0 });
   sprite->SetSize({ 127,141 });
   sprite->SettexSize({ 127,141 });

   //
   Sprite* sprite2 = Sprite::Create(spriteCommon, 1);

   spriteCommon->SpriteCommonLoadTexture(1, L"Resources/Blue.png");
   sprite2->SetPosition({ 0,360,0 });
   sprite2->SetSize({ 70,70 });
   sprite2->SettexSize({ 70,70 });

    sprite2->SpriteTransVertexBuffer();

   //
   Sprite* sprite3 = Sprite::Create(spriteCommon, 3);

   spriteCommon->SpriteCommonLoadTexture(3, L"Resources/Enemy1.png");
   sprite3->SetPosition({ 0,360,0 });
   sprite3->SetSize({ 60,60 });
   sprite3->SettexSize({ 60,60 });
   //
   Sprite* sprite4 = Sprite::Create(spriteCommon, 4);

   spriteCommon->SpriteCommonLoadTexture(4, L"Resources/LEG.png");
   sprite4->SetPosition({ 0,360,0 });
   sprite4->SetSize({ 60,60 });
   sprite4->SettexSize({ 60,60 });


    //デバックテキスト
    DebugText* debugtext=nullptr;
    debugtext = new DebugText();

    DebugText* debugtext2 = nullptr;
    debugtext2 = new DebugText();


    const int debugTextTexNumber = 2;

    spriteCommon->SpriteCommonLoadTexture(debugTextTexNumber,L"Resources/ASC_white1280.png");
    //SpriteCommonLoadTexture(spriteCommon, debugTextTexNumber, L"Resources/ASC_white1280.png", dxCommon->GetDev());

    debugtext->debugTextInit(spriteCommon,debugTextTexNumber);
    debugtext2->debugTextInit(spriteCommon, debugTextTexNumber);



    // DirectX初期化処理　ここまで

    FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
 
    //3D初期値設定

    //的のフラグ
    int TargetFlag = 1;

    //オーディオ初期化
    audio = new Audio();
    audio->Initialize();

    audio->SoundLoadWave("Resources/Shot.wav");//正解の音
    audio->SoundLoadWave("Resources/Push.wav");

    //パイプライン生成
 
    BYTE key[256] = {};
    BYTE olds[256] = {};

    int debug_x = 600;
    int debug_y = 80;

    int debug2_x = 300;
    int debug2_y = 24;

    //ゲームシーン処理関連

    int GameScene = 0;

    char moji[64];
    char moji2[64];

    Player *player=nullptr;
    player = new Player();
    Enemy enemy1;
    Collision collision;
    
    Item* item = nullptr;
    item = new Item();

    //
    PortalGun*portalgun = nullptr;
    portalgun = new PortalGun(input,gamepad);

   enemy1.Intialize();
   item->Intialize();
  // camera->SetTarget({ 10,0,0 });
   //camera->SetDistance(30.0f);
   Model* model1 = nullptr;
   Fbx3d* fbx3d1 = nullptr;

   Model* model2 = nullptr;
   ObjFbx* fbx3d2 = nullptr;

   Model* model3 = nullptr;
   BulletFbx* fbx3d3 = nullptr;

   Model* model4 = nullptr;
   BulletFbx* fbx3d4 = nullptr;

   Model* model5 = nullptr;
   Fbx3d* fbx3d5 = nullptr;

   Model* model6 = nullptr;
   Fbx3d* fbx3d6 = nullptr;

   Model* model7 = nullptr;
   Fbx3d* fbx3d7 = nullptr;

   Model* model8= nullptr;
   Fbx3d* fbx3d8= nullptr;

   Model* model9 = nullptr;
   PlayerFbx* fbx3d9 = nullptr;

   Fbx3d* wall1 = nullptr;
   Fbx3d* wall2 = nullptr;
   Fbx3d* wall3 = nullptr;
   Fbx3d* wall4 = nullptr;


   model1 = FbxLoader::GetInstance()->LoadModelFromFile("wall1");
   model2 = FbxLoader::GetInstance()->LoadModelFromFile("cubebox");
   model3 = FbxLoader::GetInstance()->LoadModelFromFile("blueBall");
   model4 = FbxLoader::GetInstance()->LoadModelFromFile("redBall");
   model5 = FbxLoader::GetInstance()->LoadModelFromFile("wall2");
   model6 = FbxLoader::GetInstance()->LoadModelFromFile("wall");
   model7 = FbxLoader::GetInstance()->LoadModelFromFile("wall3");
   model8 = FbxLoader::GetInstance()->LoadModelFromFile("wall4");
   model9 = FbxLoader::GetInstance()->LoadModelFromFile("blueBall");


   //モデル読み込み
   Fbx3d::SetDevice(dxCommon->GetDev());

   Fbx3d::SetCamera(camera);

   Fbx3d::CreateGraphicsPipeline();

   portalgun->Initialize();
  
  //3Dオブジェクト生成とモデルのセット
  fbx3d1= new Fbx3d(input);
  fbx3d1->Initialize();
  fbx3d1->SetModel(model1);
  

  fbx3d2 = new ObjFbx(input, physics);
  fbx3d2->Initialize();
  fbx3d2->SetModel(model2);

  fbx3d3 = new BulletFbx(input);
  fbx3d3->Initialize();
  fbx3d3->SetModel(model3);

  fbx3d4 = new BulletFbx(input);
  fbx3d4->Initialize();
  fbx3d4->SetModel(model4);

  fbx3d5 = new Fbx3d(input);
  fbx3d5->Initialize();
  fbx3d5->SetModel(model5);

  fbx3d6 = new Fbx3d(input);
  fbx3d6->Initialize();
  fbx3d6->SetModel(model6);

  fbx3d7 = new Fbx3d(input);
  fbx3d7->Initialize();
  fbx3d7->SetModel(model7);

  fbx3d8 = new Fbx3d(input);
  fbx3d8->Initialize();
  fbx3d8->SetModel(model8);

  fbx3d9= new PlayerFbx(input,physics);
  fbx3d9->Initialize();
  fbx3d9->SetModel(model9);

  wall1 = new Fbx3d(input);
  wall1->Initialize();
  wall1->SetModel(model8);

  wall2 = new Fbx3d(input);
  wall2->Initialize();
  wall2->SetModel(model8);

  wall3 = new Fbx3d(input);
  wall3->Initialize();
  wall3->SetModel(model8);

  wall4 = new Fbx3d(input);
  wall4->Initialize();
  wall4->SetModel(model8);

  wall1->SetPosition({ -40, 0, 0 });
  wall2->SetPosition({ +40, 0, 0 });
  wall3->SetPosition({  0, 0,-40 });
  wall4->SetPosition({  0, 0,0 });

  fbx3d1->SetPosition({ 0, -10, 0 });
  fbx3d1->SetRotate({ 0,0,0 });

  fbx3d2->SetPosition({ -30, 60,0 });
  fbx3d2->SetScale({ 0.05, 0.05,0.05 });

  fbx3d3->SetScale({ 0.05, 0.05,0.05 });
  fbx3d4->SetScale({ 0.05, 0.05,0.05 });

  fbx3d3->SetPosition({ 0, 0, -20 });
  fbx3d9->SetPosition({ 0, 0, -20 });


  fbx3d5->SetPosition({ 0, -10, 0 });
  fbx3d5->SetRotate({ 0,0,0 });


  fbx3d6->SetPosition({ 0, -10, 0 });
  fbx3d6->SetRotate({ 0,0,0 });


  fbx3d7->SetPosition({ 0, -10, 0 });
  fbx3d7->SetRotate({ 0,0,0 });

  fbx3d8->SetPosition({ 0, -10, 0 });
  fbx3d8->SetRotate({ 0,0,0 });

  //衝突マネージャー
  CollisionManager* collisionManager = nullptr;
  collisionManager = CollisionManager::GetInstance();

  float radius = 5.0f;

  fbx3d1->SetColider(new BoxCollider(XMVECTOR{80,80,80,0}));
  fbx3d2->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));

  fbx3d3->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d4->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d6->SetColider(new PlaneCollider(XMVECTOR{ 0,1,0,0 }, 0));
  fbx3d9->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  
  wall1->SetColider(new BoxCollider(XMVECTOR{ 20,80,80,0 }));
  wall2->SetColider(new BoxCollider(XMVECTOR{ 20,80,80,0 }));
  wall3->SetColider(new BoxCollider(XMVECTOR{ 80,80,20,0 }));
  wall4->SetColider(new BoxCollider(XMVECTOR{ 80,80,20,0 }));
  //当たり判定の属性
  fbx3d2->SetVerObj();
  fbx3d6->SetVerObj();
  fbx3d3->SetVerBulletRed();
  fbx3d4->SetVerBulletBlue();
  fbx3d9->SetVer();



    while (true)  // ゲームループ
    {
     //3d更新   
     //スプライト

        /*
        Ray ray;
        Plane plane;

        plane.normal = XMVectorSet(0, 1, 0, 0);
        plane.distance = 0.0f;
        ray.start = { 0,1,0,1 };
        ray.dir = { 0,-1,0,0 };
        
        XMVECTOR inter;
        float distance;
        bool hit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);
        if (hit) {
            break;
        }
        */
        
       // if (collisionManager->Raycast(ray, &raycastHit)) {

        //座標Set関連
        fbx3d2->SetMyPosition(fbx3d9->GetMyPosition());
        fbx3d2->SetCameraAxisZ(camera->GetCameraZAxis());
        fbx3d2->SetTarget(camera->GetTargetPos());
    //    fbx3d3->SetWorld(camera->GetRot());
    
        fbx3d9->SetMemo(fbx3d4->GetMemo());
        fbx3d9->SetWorld(camera->GetRot());
        fbx3d9->SetMemo2(fbx3d3->GetMemo2());
        fbx3d9->SetWarpFlag(fbx3d3->GetWarpFlag());
        fbx3d9->SetCameraAxis(camera->GetCameraZAxis());
        fbx3d9->SetPos(camera->GetPos());
        fbx3d3->SetMove(fbx3d9->GetMove());
        fbx3d4->SetMove(fbx3d9->GetMove());
        fbx3d3->SetPos(camera->GetPos());
        fbx3d4->SetPos(camera->GetPos());
        camera->SetMove(fbx3d9->GetMove());
        camera->SetWarpPosition(fbx3d9->GetPosition());
        camera->SetGround(fbx3d9->Getground());
     

     // sprite2->SpriteTransVertexBuffer();
     
      sprintf_s(moji, "X=%f", fbx3d9->GetPos());
      sprintf_s(moji2, "camera=%f", fbx3d3->GetAngleX1());
     // sprintf_s(moji2, "camera=%f", camera->GetPositionY());
      //sprintf_s(moji2,"%d",camera->GetAngleY());
     
        if (winApp->ProcessMessage())
        {
            //ゲームループを抜ける
            break;
        }

        // DirectX毎フレーム処理　ここから
      
    
        //初期化処理

        //ゲーム内の動作  

        //更新
        input->Update();

        camera->Update();
        fbx3d1->Update();
        fbx3d2->ObjUpdate(camera->GetAngleX(), camera->GetAngleY());
        fbx3d3->BlueBulletUpdate(camera->GetAngleX(), camera->GetAngleY());
        fbx3d4->RedBulletUpdate(camera->GetAngleX(), camera->GetAngleY());
        fbx3d5->Update();
        fbx3d6->Update();
        fbx3d7->Update();
        fbx3d8->Update();
        fbx3d9->PlayerUpdate(camera->GetAngleX(), camera->GetAngleY());

        wall1->Update();
        wall2->Update();
        wall3->Update();
        wall4->Update();



        camera->SetAngleRedX(fbx3d4->GetAngleX2());
        camera->SetAngleRedY(fbx3d4->GetAngleY2());
        camera->SetRedTeleport(fbx3d9->GetredTeleport());
        camera->SetAngleBlueX(fbx3d3->GetAngleX1());
        camera->SetAngleBlueY(fbx3d3->GetAngleY1());
        camera->SetBlueTeleport(fbx3d9->GetblueTeleport());
        camera->SetEyePos(fbx3d9->GetMyPosition());


        //レンダ―テクスチャの描画
        sprite100->PreDrawScene(dxCommon->GetCmdList());
        ////スプライト共通コマンド
        spriteCommon->PreDraw();
       
        //FBX描画
        fbx3d1->Draw2(dxCommon->GetCmdList());
        fbx3d2->Draw2(dxCommon->GetCmdList());
        fbx3d3->Draw2(dxCommon->GetCmdList());
        fbx3d4->Draw2(dxCommon->GetCmdList());
        fbx3d5->Draw2(dxCommon->GetCmdList());
        fbx3d6->Draw2(dxCommon->GetCmdList());
        fbx3d7->Draw2(dxCommon->GetCmdList());
        fbx3d8->Draw2(dxCommon->GetCmdList());

       
        //ポストエフェクトここまで
        sprite100->PostDrawScene(dxCommon->GetCmdList());

        //バックバッファの番号を取得（2つなので0番か1番）
        dxCommon->PreDraw();


        spriteCommon->PreDraw_Post();
         sprite100->Update();
        sprite100->PostDraw();
        
        //3D描画前処理
      //  Object3d::PreDraw(dxCommon->GetCmdList());
          //3D描画
        //ここに処理追加できる
      
        //3D描画後処理
        //Object3d::PostDraw();
       

        ////スプライト共通コマンド
        spriteCommon->PreDraw();
        debugtext->Print(moji, 100, 100);
        debugtext->DrawAll();//的カウント

        debugtext2->Print(moji2, 100, 200);
        debugtext2->DrawAll();//的カウント


        //スプライト表示

        //アイテム
       // if(item->LEG_[0].Flag ==0) sprite4->SpriteDraw();


        //敵
       // if (enemy1.Enemy1[0].Flag == 1)sprite3->SpriteDraw();

        //自キャラ
        sprite->SpriteTransVertexBuffer();
        sprite->Update();
        sprite->SpriteDraw();
       
        
        // ４．描画コマンドここまで
     collisionManager->CheckAllCollisions();
        // DirectX毎フレーム処理　ここまで
        dxCommon->PostDraw();
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

    delete sprite;

    //テキスト解放
    delete debugtext;

    audio->Finalize();
    delete audio;

    delete fbx3d1;
    delete model1;

    FbxLoader::GetInstance()->Finalize();


    for (auto object : objects) {
        delete(object);
    }

    delete(modelPlane);
    delete(modelBox);
    delete(modelPyramid);

    return 0;
}
