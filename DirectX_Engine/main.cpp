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
#include "Wallcolider.h"
#include "CollisionManager.h"
#include "PlayerFbx.h"
#include "BulletFbx.h"
#include "Physics.h" 
#include"ObjFbx.h"
#include "SceneSelect.h"
#include "PortalExit.h"
#include "BlueCameraDebug.h"
#include "RedCameraDebug.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "lib/nlohmann/json.hpp"
#include <fstream>
#include<map>
#include "MapLoader.h"
#include "MapWrite.h"
#include "MapEdit.h"


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
BlueCameraDebug* Bluecamera = nullptr;
RedCameraDebug* Redcamera = nullptr;
//# Windowsアプリでのエントリーポイント(main関数)
#pragma optimize( "", off )
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //FbxManager* fbxManager = FbxManager::Create();

    //ポインタ置き場
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    Audio* audio = nullptr;
    GamePad* gamepad = nullptr;
    Physics* physics=nullptr;
    SceneSelect* scene = nullptr;
    ParticleManager* particleMan = nullptr;
    ParticleManager* particleManBlue = nullptr;

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
     scene = new SceneSelect(WinApp::window_width, WinApp::window_height, input);

    //入力の初期化
  //  Input* input = Input::GetInstance();
  
    input->Intialize(winApp);


 
    camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
    Bluecamera= new BlueCameraDebug(WinApp::window_width, WinApp::window_height);
    Redcamera = new RedCameraDebug(WinApp::window_width, WinApp::window_height);


    //3D初期化
   Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

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
    //FBX
 //   Fbx3d::SetDevice(dxCommon->GetDev());

  //  Fbx3d::SetCamera(camera);

  //  Fbx3d::CreateGraphicsPipeline();

    //3Dオブジェクト生成とモデルのセット
 //   fbx3d1 = new Fbx3d;
 //   fbx3d1->Initialize();
 //   fbx3d1->SetModel(model1);
 // 
 // 
 // 
 // 
 

   /*
   struct LevelData {
       struct ObjectData
       {
           std::string fileName;
           XMVECTOR translation;
           XMVECTOR rotation;
           XMVECTOR scaling;

       };

       std::vector<ObjectData> objects;
   };

   std::map<std::string,Model*>models;
   //レベルエディター
   const std::string fullpath = std::string("Resources/levels/") + "untitled.json";

   std::ifstream file;

   file.open(fullpath);
   if (file.fail()) {
       assert(0);
   }
   //解凍
   nlohmann::json deserialized;

   file >> deserialized;
   //Levelチェック
   assert(deserialized.is_object());
   assert(deserialized.contains("name"));
   assert(deserialized["name"].is_string());

   std::string name =
       deserialized["name"].get<std::string>();
   assert(name.compare("scene") == 0);

   //レベルデータ格納インスタンス
   LevelData* levelData = new LevelData();

   for (nlohmann::json& object : deserialized["objects"]) {
       assert(object.contains("type"));

       std::string type = object["type"].get<std::string>();

       //種類ごとの処理
       //MESH

       if (type.compare("MESH") == 0)
       {
           levelData->objects.emplace_back(LevelData::ObjectData{});
           //要素の参照を得る
           LevelData::ObjectData& objectData = levelData->objects.back();

           if (object.contains("file_name")) {
               //ファイル名
               objectData.fileName = object["file_name"];
           }

           nlohmann::json& transform = object["transform"];

           objectData.translation.m128_f32[0] = (float)transform["translation"][1];
           objectData.translation.m128_f32[1] = (float)transform["translation"][2];
           objectData.translation.m128_f32[2] = (float)transform["translation"][0];
           objectData.translation.m128_f32[3] = 1.0f;


           objectData.rotation.m128_f32[0] = (float)transform["rotation"][1];
           objectData.rotation.m128_f32[1] = (float)transform["rotation"][2];
           objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
           objectData.rotation.m128_f32[3] = 1.0f;

           objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
           objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
           objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
           objectData.scaling.m128_f32[3] = 1.0f;

       }
   }


   for (auto& objectData : levelData->objects) {
       Model* model = nullptr;
       decltype(models)::iterator it = models.find(objectData.fileName);
       if (it != models.end()) { model = it->second; }

       Object3d* newObject = Object3d::Create();
       newObject->SetModel(model);

       DirectX::XMFLOAT3 pos;
       DirectX::XMStoreFloat3(&pos, objectData.translation);

       DirectX::XMFLOAT3 rot;
       DirectX::XMStoreFloat3(&rot, objectData.rotation);

       DirectX::XMFLOAT3 scale;
       DirectX::XMStoreFloat3(&scale, objectData.scaling);

       objects.push_back(newObject);
   }
   */

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

   //Sprite* sprite101 = Sprite::PostCreate(spriteCommon, 101);
   //spriteCommon->SpriteCommonLoadTexture(101, L"Resources/Red.png");


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
  //  audio->SoundLoadWave("Resources/SOUND/Wark_sund.wav");//押す音
 //   audio->SoundLoadWave("Resources/Shot.wav");//正解の音
  //  audio->SoundLoadWave("Resources/Push.wav");

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
   Fbx3d* fbx3d2 = nullptr;

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

   Model* model10 = nullptr;
   ObjFbx* fbx3d10 = nullptr;

   Fbx3d* fbx3d11 = nullptr;
   Fbx3d* fbx3d12 = nullptr;
   Fbx3d* fbx3d13 = nullptr;
   Fbx3d* fbx3d14 = nullptr;
   Fbx3d* fbx3d15 = nullptr;

   Fbx3d* fbx3d16 = nullptr;
   Fbx3d* fbx3d17 = nullptr;
   Fbx3d* fbx3d18 = nullptr;
   Fbx3d* fbx3d19 = nullptr;


   Model* model11 = nullptr;
   Fbx3d* fbx3d20 = nullptr;

   //Model* model11 = nullptr;
   Fbx3d* fbx3d22 = nullptr;



   Model* model12 = nullptr;
   ObjFbx* fbx3d21 = nullptr;

   Model* model13 = nullptr;
   Model* model13_2 = nullptr;
   PortalExit* fbx3d23 = nullptr;
   PortalExit* fbx3d24 = nullptr;

   Model* model15 = nullptr;
   Fbx3d* fbx3d25 = nullptr;

   Model* model16 = nullptr;
   Fbx3d* fbx3d26 = nullptr;

   Model* model17 = nullptr;
   ObjFbx* fbx3d27 = nullptr;

   Fbx3d* fbx3d28 = nullptr;
   Fbx3d* fbx3d29 = nullptr;
   Fbx3d* fbx3d30 = nullptr;
   Fbx3d* fbx3d31 = nullptr;

   ObjFbx* fbx3d32 = nullptr;



   Fbx3d* wall1 = nullptr;
   Fbx3d* wall2 = nullptr;
   Fbx3d* wall3 = nullptr;
   Fbx3d* wall4 = nullptr;

   Model* model14 = nullptr;

   Model* model18 = nullptr;
   Fbx3d* fbx3d33 = nullptr;

   Fbx3d* fbx3d34 = nullptr;

   Model* model19 = nullptr;
   Fbx3d* fbx3d35 = nullptr;

   Model* model20 = nullptr;

   Model* model21 = nullptr;
   Fbx3d* fbx3d36 = nullptr;

   Model* model22 = nullptr;
   Fbx3d* fbx3d37 = nullptr;


   Model* model23 = nullptr;
   Fbx3d* fbx3d38 = nullptr;

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


   Fbx3d* fbx3d1_2 = nullptr;
   Fbx3d* fbx3d2_2 = nullptr;
   Fbx3d* fbx3d3_2 = nullptr;
   Fbx3d* fbx3d4_2 = nullptr;
   Fbx3d* fbx3d5_2 = nullptr;
   Fbx3d* fbx3d6_2 = nullptr;
   Fbx3d* fbx3d7_2 = nullptr;
   Fbx3d* fbx3d8_2 = nullptr;
   Fbx3d* fbx3d9_2 = nullptr;
   Fbx3d* fbx3d10_2 = nullptr;
   Fbx3d* fbx3d11_2 = nullptr;
   Fbx3d* fbx3d12_2 = nullptr;
   Fbx3d* fbx3d13_2 = nullptr;
   Fbx3d* fbx3d14_2 = nullptr;
   Fbx3d* fbx3d15_2 = nullptr;
   Fbx3d* fbx3d16_2 = nullptr;
   Fbx3d* fbx3d17_2 = nullptr;
   Fbx3d* fbx3d18_2 = nullptr;
   Fbx3d* fbx3d19_2 = nullptr;
   Fbx3d* fbx3d20_2 = nullptr;
   Fbx3d* fbx3d21_2 = nullptr;
   Fbx3d* fbx3d22_2 = nullptr;
   Fbx3d* fbx3d23_2 = nullptr;
   Fbx3d* fbx3d24_2 = nullptr;
   Fbx3d* fbx3d25_2 = nullptr;
   Fbx3d* fbx3d26_2 = nullptr;
   Fbx3d* fbx3d27_2 = nullptr;
   Fbx3d* fbx3d28_2 = nullptr;
   Fbx3d* fbx3d29_2 = nullptr;
   Fbx3d* fbx3d30_2 = nullptr;
   Fbx3d* fbx3d31_2 = nullptr;
   Fbx3d* fbx3d32_2 = nullptr;
   Fbx3d* fbx3d33_2 = nullptr;
   Fbx3d* fbx3d34_2 = nullptr;
   Fbx3d* fbx3d35_2 = nullptr;
   Fbx3d* fbx3d36_2 = nullptr;
   Fbx3d* fbx3d37_2 = nullptr;

   Fbx3d* fbx3d1_3 = nullptr;
   Fbx3d* fbx3d2_3 = nullptr;
   Fbx3d* fbx3d3_3 = nullptr;
   Fbx3d* fbx3d4_3 = nullptr;
   Fbx3d* fbx3d5_3 = nullptr;
   Fbx3d* fbx3d6_3 = nullptr;
   Fbx3d* fbx3d7_3 = nullptr;
   Fbx3d* fbx3d8_3 = nullptr;
   Fbx3d* fbx3d9_3 = nullptr;
   Fbx3d* fbx3d10_3 = nullptr;
   Fbx3d* fbx3d11_3 = nullptr;
   Fbx3d* fbx3d12_3 = nullptr;
   Fbx3d* fbx3d13_3 = nullptr;
   Fbx3d* fbx3d14_3 = nullptr;
   Fbx3d* fbx3d15_3 = nullptr;
   Fbx3d* fbx3d16_3 = nullptr;
   Fbx3d* fbx3d17_3 = nullptr;
   Fbx3d* fbx3d18_3 = nullptr;
   Fbx3d* fbx3d19_3 = nullptr;
   Fbx3d* fbx3d20_3 = nullptr;
   Fbx3d* fbx3d21_3 = nullptr;
   Fbx3d* fbx3d22_3 = nullptr;
   Fbx3d* fbx3d23_3 = nullptr;
   Fbx3d* fbx3d24_3 = nullptr;
   Fbx3d* fbx3d25_3 = nullptr;
   Fbx3d* fbx3d26_3 = nullptr;
   Fbx3d* fbx3d27_3 = nullptr;
   Fbx3d* fbx3d28_3 = nullptr;
   Fbx3d* fbx3d29_3 = nullptr;
   Fbx3d* fbx3d30_3 = nullptr;
   Fbx3d* fbx3d31_3 = nullptr;
   Fbx3d* fbx3d32_3 = nullptr;
   Fbx3d* fbx3d33_3 = nullptr;
   Fbx3d* fbx3d34_3 = nullptr;
   Fbx3d* fbx3d35_3 = nullptr;
   Fbx3d* fbx3d36_3 = nullptr;
   Fbx3d* fbx3d37_3 = nullptr;
   
   Fbx3d* Bluefbx3d1 = nullptr;
 



   //マップエディット
   MapEdit* mapedit = nullptr;
   mapedit = new MapEdit(WinApp::window_width, WinApp::window_height, input);
   mapedit->SetFenceModel(model10);
   mapedit->SetWallModel(model27);
   mapedit->SetFloorModel(model26);
   mapedit->SetDrumModel(model11);
   mapedit->SetCollisionModel(model24);
   mapedit->SetExitModel(model2);
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
   
   //Fbx3d::SetCamera(Bluecamera);



   Fbx3d::CreateGraphicsPipeline();

   portalgun->Initialize();
  
  //3Dオブジェクト生成とモデルのセット
  fbx3d1= new Fbx3d(input);
  fbx3d1->Initialize();
  fbx3d1->SetModel(model1);

  fbx3d2 = new Fbx3d(input);
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
  fbx3d5->SetModel(model8);

  fbx3d6 = new Fbx3d(input);
  fbx3d6->Initialize();
  fbx3d6->SetModel(model6);

  fbx3d7 = new Fbx3d(input);
  fbx3d7->Initialize();
  fbx3d7->SetModel(model8);

  fbx3d8 = new Fbx3d(input);
  fbx3d8->Initialize();
  fbx3d8->SetModel(model8);

  fbx3d9= new PlayerFbx(input,physics);
  fbx3d9->Initialize();
  fbx3d9->SetModel(model14);

  fbx3d10 = new ObjFbx(input,physics);
  fbx3d10->Initialize();
  fbx3d10->SetModel(model10);


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

  fbx3d11 = new Fbx3d(input);
  fbx3d11->Initialize();
  fbx3d11->SetModel(model10);

  fbx3d12 = new Fbx3d(input);
  fbx3d12->Initialize();
  fbx3d12->SetModel(model10);

  fbx3d13 = new Fbx3d(input);
  fbx3d13->Initialize();
  fbx3d13->SetModel(model10);

  fbx3d14 = new Fbx3d(input);
  fbx3d14->Initialize();
  fbx3d14->SetModel(model10);

  fbx3d15 = new Fbx3d(input);
  fbx3d15->Initialize();
  fbx3d15->SetModel(model10);

  fbx3d16 = new Fbx3d(input);
  fbx3d16->Initialize();
  fbx3d16->SetModel(model10);

  fbx3d17 = new Fbx3d(input);
  fbx3d17->Initialize();
  fbx3d17->SetModel(model10);

  fbx3d18 = new Fbx3d(input);
  fbx3d18->Initialize();
  fbx3d18->SetModel(model10);

  fbx3d19 = new Fbx3d(input);
  fbx3d19->Initialize();
  fbx3d19->SetModel(model10);

  fbx3d20 = new Fbx3d(input);
  fbx3d20->Initialize();
  fbx3d20->SetModel(model11);

  fbx3d21 = new ObjFbx(input,physics);
  fbx3d21->Initialize();
  fbx3d21->SetModel(model12);


  fbx3d22 = new Fbx3d(input);
  fbx3d22->Initialize();
  fbx3d22->SetModel(model8);

  fbx3d23 = new PortalExit(input);
 // fbx3d23->PortalCreateGraphicsPipeline();
  fbx3d23->RenderFbxInitialize();
  fbx3d23->SetModel(model13);

  fbx3d24 = new PortalExit(input);
 // fbx3d24->PortalCreateGraphicsPipeline();
  fbx3d24->RenderFbxInitialize();
  fbx3d24->SetModel(model13_2);

  fbx3d25 = new Fbx3d(input);
  fbx3d25->Initialize();
  fbx3d25->SetModel(model15);

  fbx3d26 = new Fbx3d(input);
  fbx3d26->Initialize();
  fbx3d26->SetModel(model16);

  fbx3d27 = new ObjFbx(input, physics);
  fbx3d27->Initialize();
  fbx3d27->SetModel(model17);

  fbx3d28 = new Fbx3d(input);
  fbx3d28->Initialize();
  fbx3d28->SetModel(model10);

  fbx3d29 = new Fbx3d(input);
  fbx3d29->Initialize();
  fbx3d29->SetModel(model10);

  fbx3d30 = new Fbx3d(input);
  fbx3d30->Initialize();
  fbx3d30->SetModel(model10);

  fbx3d31 = new Fbx3d(input);
  fbx3d31->Initialize();
  fbx3d31->SetModel(model10);

  fbx3d32= new ObjFbx(input, physics);
  fbx3d32->Initialize();
  fbx3d32->SetModel(model17);

  fbx3d33 = new Fbx3d(input);
  fbx3d33->Initialize();
  fbx3d33->SetModel(model18);

  fbx3d34 = new Fbx3d(input);
  fbx3d34->Initialize();
  fbx3d34->SetModel(model20);

  fbx3d35 = new Fbx3d(input);
  fbx3d35->Initialize();
  fbx3d35->SetModel(model19);

  fbx3d36 = new Fbx3d(input);
  fbx3d36->Initialize();
  fbx3d36->SetModel(model21);

  fbx3d37 = new Fbx3d(input);
  fbx3d37->Initialize();
  fbx3d37->SetModel(model22);

  fbx3d38 = new Fbx3d(input);
  fbx3d38->Initialize();
  fbx3d38->SetModel(model23);
  //3Dオブジェクト生成とモデルのセット
  fbx3d1_2 = new Fbx3d(input);
  fbx3d1_2->Initialize();
  fbx3d1_2->SetModel(model1);

  fbx3d2_2 = new ObjFbx(input, physics);
  fbx3d2_2->Initialize();
  fbx3d2_2->SetModel(model2);

  fbx3d3_2 = new BulletFbx(input);
  fbx3d3_2->Initialize();
  fbx3d3_2->SetModel(model3);

  fbx3d4_2 = new BulletFbx(input);
  fbx3d4_2->Initialize();
  fbx3d4_2->SetModel(model4);

  fbx3d5_2 = new Fbx3d(input);
  fbx3d5_2->Initialize();
  fbx3d5_2->SetModel(model8);

  fbx3d6_2 = new Fbx3d(input);
  fbx3d6_2->Initialize();
  fbx3d6_2->SetModel(model6);

  fbx3d7_2 = new Fbx3d(input);
  fbx3d7_2->Initialize();
  fbx3d7_2->SetModel(model8);

  fbx3d8_2 = new Fbx3d(input);
  fbx3d8_2->Initialize();
  fbx3d8_2->SetModel(model8);

  fbx3d9_2 = new PlayerFbx(input, physics);
  fbx3d9_2->Initialize();
  fbx3d9_2->SetModel(model14);

  fbx3d10_2 = new ObjFbx(input, physics);
  fbx3d10_2->Initialize();
  fbx3d10_2->SetModel(model10);

  fbx3d11_2 = new Fbx3d(input);
  fbx3d11_2->Initialize();
  fbx3d11_2->SetModel(model10);

  fbx3d12_2 = new Fbx3d(input);
  fbx3d12_2->Initialize();
  fbx3d12_2->SetModel(model10);

  fbx3d13_2 = new Fbx3d(input);
  fbx3d13_2->Initialize();
  fbx3d13_2->SetModel(model10);

  fbx3d14_2 = new Fbx3d(input);
  fbx3d14_2->Initialize();
  fbx3d14_2->SetModel(model10);

  fbx3d15_2 = new Fbx3d(input);
  fbx3d15_2->Initialize();
  fbx3d15_2->SetModel(model10);

  fbx3d16_2 = new Fbx3d(input);
  fbx3d16_2->Initialize();
  fbx3d16_2->SetModel(model10);

  fbx3d17_2 = new Fbx3d(input);
  fbx3d17_2->Initialize();
  fbx3d17_2->SetModel(model10);

  fbx3d18_2 = new Fbx3d(input);
  fbx3d18_2->Initialize();
  fbx3d18_2->SetModel(model10);

  fbx3d19_2 = new Fbx3d(input);
  fbx3d19_2->Initialize();
  fbx3d19_2->SetModel(model10);

  fbx3d20_2 = new Fbx3d(input);
  fbx3d20_2->Initialize();
  fbx3d20_2->SetModel(model11);

  fbx3d21_2 = new ObjFbx(input, physics);
  fbx3d21_2->Initialize();
  fbx3d21_2->SetModel(model12);


  fbx3d22_2 = new Fbx3d(input);
  fbx3d22_2->Initialize();
  fbx3d22_2->SetModel(model8);

  fbx3d23_2 = new PortalExit(input);
  // fbx3d23->PortalCreateGraphicsPipeline();
  fbx3d23_2->RenderFbxInitialize();
  fbx3d23_2->SetModel(model13);

  fbx3d24_2 = new PortalExit(input);
  // fbx3d24->PortalCreateGraphicsPipeline();
  fbx3d24_2->RenderFbxInitialize();
  fbx3d24_2->SetModel(model13);

  fbx3d25_2 = new Fbx3d(input);
  fbx3d25_2->Initialize();
  fbx3d25_2->SetModel(model15);

  fbx3d26_2 = new Fbx3d(input);
  fbx3d26_2->Initialize();
  fbx3d26_2->SetModel(model16);

  fbx3d27_2 = new ObjFbx(input, physics);
  fbx3d27_2->Initialize();
  fbx3d27_2->SetModel(model17);

  fbx3d28_2 = new Fbx3d(input);
  fbx3d28_2->Initialize();
  fbx3d28_2->SetModel(model10);

  fbx3d29_2 = new Fbx3d(input);
  fbx3d29_2->Initialize();
  fbx3d29_2->SetModel(model10);

  fbx3d30_2 = new Fbx3d(input);
  fbx3d30_2->Initialize();
  fbx3d30_2->SetModel(model10);

  fbx3d31_2 = new Fbx3d(input);
  fbx3d31_2->Initialize();
  fbx3d31_2->SetModel(model10);

  fbx3d32_2 = new ObjFbx(input, physics);
  fbx3d32_2->Initialize();
  fbx3d32_2->SetModel(model17);

  fbx3d33_2 = new Fbx3d(input);
  fbx3d33_2->Initialize();
  fbx3d33_2->SetModel(model18);

  fbx3d34_2 = new Fbx3d(input);
  fbx3d34_2->Initialize();
  fbx3d34_2->SetModel(model20);

  fbx3d35_2 = new Fbx3d(input);
  fbx3d35_2->Initialize();
  fbx3d35_2->SetModel(model19);

  fbx3d36_2 = new Fbx3d(input);
  fbx3d36_2->Initialize();
  fbx3d36_2->SetModel(model21);

  fbx3d37_2 = new Fbx3d(input);
  fbx3d37_2->Initialize();
  fbx3d37_2->SetModel(model22);

  //3Dオブジェクト生成とモデルのセット
  fbx3d1_3 = new Fbx3d(input);
  fbx3d1_3->Initialize();
  fbx3d1_3->SetModel(model1);

  fbx3d2_3 = new ObjFbx(input, physics);
  fbx3d2_3->Initialize();
  fbx3d2_3->SetModel(model2);

  fbx3d3_3 = new BulletFbx(input);
  fbx3d3_3->Initialize();
  fbx3d3_3->SetModel(model3);

  fbx3d4_3 = new BulletFbx(input);
  fbx3d4_3->Initialize();
  fbx3d4_3->SetModel(model4);

  fbx3d5_3 = new Fbx3d(input);
  fbx3d5_3->Initialize();
  fbx3d5_3->SetModel(model8);

  fbx3d6_3 = new Fbx3d(input);
  fbx3d6_3->Initialize();
  fbx3d6_3->SetModel(model6);

  fbx3d7_3 = new Fbx3d(input);
  fbx3d7_3->Initialize();
  fbx3d7_3->SetModel(model8);

  fbx3d8_3 = new Fbx3d(input);
  fbx3d8_3->Initialize();
  fbx3d8_3->SetModel(model8);

  fbx3d9_3 = new PlayerFbx(input, physics);
  fbx3d9_3->Initialize();
  fbx3d9_3->SetModel(model14);

  fbx3d10_3 = new ObjFbx(input, physics);
  fbx3d10_3->Initialize();
  fbx3d10_3->SetModel(model10);

  fbx3d11_3 = new Fbx3d(input);
  fbx3d11_3->Initialize();
  fbx3d11_3->SetModel(model10);

  fbx3d12_3 = new Fbx3d(input);
  fbx3d12_3->Initialize();
  fbx3d12_3->SetModel(model10);

  fbx3d13_3 = new Fbx3d(input);
  fbx3d13_3->Initialize();
  fbx3d13_3->SetModel(model10);

  fbx3d14_3 = new Fbx3d(input);
  fbx3d14_3->Initialize();
  fbx3d14_3->SetModel(model10);

  fbx3d15_3 = new Fbx3d(input);
  fbx3d15_3->Initialize();
  fbx3d15_3->SetModel(model10);

  fbx3d16_3 = new Fbx3d(input);
  fbx3d16_3->Initialize();
  fbx3d16_3->SetModel(model10);

  fbx3d17_3 = new Fbx3d(input);
  fbx3d17_3->Initialize();
  fbx3d17_3->SetModel(model10);

  fbx3d18_3 = new Fbx3d(input);
  fbx3d18_3->Initialize();
  fbx3d18_3->SetModel(model10);

  fbx3d19_3 = new Fbx3d(input);
  fbx3d19_3->Initialize();
  fbx3d19_3->SetModel(model10);

  fbx3d20_3 = new Fbx3d(input);
  fbx3d20_3->Initialize();
  fbx3d20_3->SetModel(model11);

  fbx3d21_3 = new ObjFbx(input, physics);
  fbx3d21_3->Initialize();
  fbx3d21_3->SetModel(model12);


  fbx3d22_3 = new Fbx3d(input);
  fbx3d22_3->Initialize();
  fbx3d22_3->SetModel(model8);

  fbx3d23_3 = new PortalExit(input);
  // fbx3d23->PortalCreateGraphicsPipeline();
  fbx3d23_3->RenderFbxInitialize();
  fbx3d23_3->SetModel(model13);

  fbx3d24_3 = new PortalExit(input);
  // fbx3d24->PortalCreateGraphicsPipeline();
  fbx3d24_3->RenderFbxInitialize();
  fbx3d24_3->SetModel(model13);

  fbx3d25_3 = new Fbx3d(input);
  fbx3d25_3->Initialize();
  fbx3d25_3->SetModel(model15);

  fbx3d26_3 = new Fbx3d(input);
  fbx3d26_3->Initialize();
  fbx3d26_3->SetModel(model16);

  fbx3d27_3 = new ObjFbx(input, physics);
  fbx3d27_3->Initialize();
  fbx3d27_3->SetModel(model17);

  fbx3d28_3 = new Fbx3d(input);
  fbx3d28_3->Initialize();
  fbx3d28_3->SetModel(model10);

  fbx3d29_3 = new Fbx3d(input);
  fbx3d29_3->Initialize();
  fbx3d29_3->SetModel(model10);

  fbx3d30_3 = new Fbx3d(input);
  fbx3d30_3->Initialize();
  fbx3d30_3->SetModel(model10);

  fbx3d31_3 = new Fbx3d(input);
  fbx3d31_3->Initialize();
  fbx3d31_3->SetModel(model10);

  fbx3d32_3 = new ObjFbx(input, physics);
  fbx3d32_3->Initialize();
  fbx3d32_3->SetModel(model17);

  fbx3d33_3 = new Fbx3d(input);
  fbx3d33_3->Initialize();
  fbx3d33_3->SetModel(model18);

  fbx3d34_3 = new Fbx3d(input);
  fbx3d34_3->Initialize();
  fbx3d34_3->SetModel(model20);

  fbx3d35_3 = new Fbx3d(input);
  fbx3d35_3->Initialize();
  fbx3d35_3->SetModel(model19);

  fbx3d36_3 = new Fbx3d(input);
  fbx3d36_3->Initialize();
  fbx3d36_3->SetModel(model21);

  fbx3d37_3 = new Fbx3d(input);
  fbx3d37_3->Initialize();
  fbx3d37_3->SetModel(model22);

  wall1->SetPosition({ -90, 0, 0 });
  wall2->SetPosition({ +90, 0, 0 });
  wall3->SetPosition({  0, 0,-90 });
  wall4->SetPosition({  0, 0,+90 });

  fbx3d1->SetPosition({ 0, -10, 0 });
  fbx3d1->SetRotate({ 0,0,0 });

  fbx3d2->SetPosition({ -60,1,100 });
  fbx3d2->SetScale({ 0.1,0.1, 0.1 });
  fbx3d2->SetRotate({ 0,0,180 });

  fbx3d3->SetScale({ 0.05, 0.05,0.05 });
  fbx3d4->SetScale({ 0.05, 0.05,0.05 });

  fbx3d3->SetPosition({ 0, 0, -20 });
  fbx3d9->SetPosition({ 0, 0, -20 });


  fbx3d5->SetPosition({105, 80, 0 });
  fbx3d5->SetRotate({ 90,270,0 });

  fbx3d22->SetPosition({ -105, 80, 0 });
  fbx3d22->SetRotate({ 90,90,0 });



  fbx3d6->SetPosition({ 0, -10, 0 });
  fbx3d6->SetRotate({ 0,0,0 });


  fbx3d7->SetPosition({ 0, 80, -105 });
  fbx3d7->SetRotate({ 90,0,0 });

  fbx3d8->SetPosition({ 0, 0, 106 });
  fbx3d8->SetRotate({ 270,0,0 });

  fbx3d10->SetPosition({ 0, 0, -12 });
  fbx3d10->SetScale({ 0.1,0.1,0.1 });
  fbx3d10->SetRotate({ 0,0,0 });

  fbx3d11->SetPosition({ 20, 0, -12 });
  fbx3d11->SetScale({ 0.1,0.1,0.1 });
  fbx3d11->SetRotate({ 0,0,0 });

  fbx3d12->SetPosition({ 40, 0, -12 });
  fbx3d12->SetScale({ 0.1,0.1,0.1 });
  fbx3d12->SetRotate({ 0,0,0 });

  fbx3d13->SetPosition({ 60, 0, -12 });
  fbx3d13->SetScale({ 0.1,0.1,0.1 });
  fbx3d13->SetRotate({ 0,0,0 });

  fbx3d14->SetPosition({ 80, 0, -12});
  fbx3d14->SetScale({ 0.1,0.1,0.1 });
  fbx3d14->SetRotate({ 0,0,0 });

  fbx3d15->SetPosition({ -20, 0, -12 });
  fbx3d15->SetScale({ 0.1,0.1,0.1 });
  fbx3d15->SetRotate({ 0,0,0 });

  fbx3d16->SetPosition({ -40, 0, -12 });
  fbx3d16->SetScale({ 0.1,0.1,0.1 });
  fbx3d16->SetRotate({ 0,0,0 });

  fbx3d17->SetPosition({ -60, 0, -12 });
  fbx3d17->SetScale({ 0.1,0.1,0.1 });
  fbx3d17->SetRotate({ 0,0,0 });

  fbx3d18->SetPosition({ -80, 0, -12 });
  fbx3d18->SetScale({ 0.1,0.1,0.1 });
  fbx3d18->SetRotate({ 0,0,0 });

  fbx3d19->SetPosition({ 0, 0, -12 });
  fbx3d19->SetScale({ 0.1,0.1,0.1 });
  fbx3d19->SetRotate({ 0,0,0 });

  fbx3d20->SetPosition({ 60, -6, -30 });
  fbx3d20->SetScale({ 0.014,0.014,0.014 });
  fbx3d20->SetRotate({ 0,0,0 });

  fbx3d21->SetPosition({ 60, -1.8, -30 });
  fbx3d21->SetScale({ 0.012,0.012,0.012 });
  fbx3d21->SetRotate({ 0,45,90 });

  fbx3d23->SetPosition({ 0,0,0 });
  fbx3d23->SetScale({ 0.1,0.1,0.1});
  fbx3d23->SetRotate({ 0,0,270 });

  fbx3d24->SetPosition({ 0,0,0 });
  fbx3d24->SetScale({ 0.1,0.1,0.1 });
  fbx3d24->SetRotate({ 0,0,270 });

  fbx3d25->SetPosition({ 0,-1,-14 });
  fbx3d25->SetScale({ 0.04,0.04,0.04 });
  fbx3d25->SetRotate({ 0,360,0 });


  fbx3d26->SetPosition({ 28,-1,-60 });
  fbx3d26->SetScale({ 0.04,0.04,0.04 });
  fbx3d26->SetRotate({ 0,90,0 });

  fbx3d27->SetPosition({ 30,0,-41 });
  fbx3d27->SetScale({ 3.5,3.5,3.5 });
  fbx3d27->SetRotate({ 0,0,0 });

  fbx3d28->SetPosition({ 30, 0, -22 });
  fbx3d28->SetScale({ 0.1,0.1,0.1 });
  fbx3d28->SetRotate({ 0,90,0 });

  fbx3d29->SetPosition({ 30, 0, -62 });
  fbx3d29->SetScale({ 0.1,0.1,0.1 });
  fbx3d29->SetRotate({ 0,90,0 });

  fbx3d30->SetPosition({ 30, 0, -82 });
  fbx3d30->SetScale({ 0.1,0.1,0.1 });
  fbx3d30->SetRotate({ 0,90,0 });

  fbx3d31->SetPosition({ 30, 0, -102 });
  fbx3d31->SetScale({ 0.1,0.1,0.1 });
  fbx3d31->SetRotate({ 0,90,0 });

  fbx3d32->SetPosition({ 30,0,-37 });
  fbx3d32->SetScale({ 3.5,3.5,3.5 });
  fbx3d32->SetRotate({ 0,0,0 });

  fbx3d33->SetPosition({ 0,0,0 });
  fbx3d33->SetScale({ 0.1,0.1,0.1 });
  fbx3d33->SetRotate({ 0,0,270 });


  fbx3d34->SetPosition({ 300,10,60 });
  fbx3d34->SetScale({ 0.1,0.1, 0.1 });
  fbx3d34->SetRotate({ 0,0,180 });

  fbx3d35->SetPosition({ -45,2,90 });
  fbx3d35->SetScale({ 0.02,0.02,0.02 });
  fbx3d35->SetRotate({ 0,90,0 });



  fbx3d36->SetPosition({ 307,10,60 });
  fbx3d36->SetScale({ 0.1,0.1, 0.1 });
  fbx3d36->SetRotate({ 0,0,180 });


  fbx3d37->SetPosition({ 40,-1,-14 });
  fbx3d37->SetScale({ 0.04,0.04,0.04 });
  fbx3d37->SetRotate({ 0,360,0 });


  fbx3d38->SetPosition({ 0,0,0 });
  fbx3d38->SetScale({ 1,1,1 });
  fbx3d38->SetRotate({ 0,0,0 });

  //衝突マネージャー
  CollisionManager* collisionManager = nullptr;
  collisionManager = CollisionManager::GetInstance();

  float radius = 5.0f;
  float PlayerRadius = 1.0f;

  fbx3d1->SetColider(new BoxCollider(XMVECTOR{70,70,70,0}));
  fbx3d2->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius+30));

  fbx3d3->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d4->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d6->SetColider(new WallCollider(XMVECTOR{ 100,2,100,0 }));
  fbx3d9->SetColider(new SphereCollider(XMVECTOR{ 0,PlayerRadius,0,0 }, PlayerRadius));
  fbx3d10->SetColider(new WallCollider(XMVECTOR{ 20,10,0.1,0 }));

  
  wall1->SetColider(new WallCollider(XMVECTOR{ 3,80,100,0 }));
  wall2->SetColider(new WallCollider(XMVECTOR{ 3,80,100,0 }));
  wall3->SetColider(new WallCollider(XMVECTOR{ 100,80,3,0 }));
  wall4->SetColider(new WallCollider(XMVECTOR{ 100,80,3,0 }));

  fbx3d11->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d12->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d13->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d14->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d15->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d16->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d17->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d18->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));
  fbx3d19->SetColider(new WallCollider(XMVECTOR{ 20,15,2,0 }));

  fbx3d20->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d21->SetColider(new SphereCollider(XMVECTOR{ 0,radius,0,0 }, radius));
  fbx3d27->SetColider(new SphereCollider(XMVECTOR{ 0,radius-2,0,0 }, radius-2));
 // fbx3d32->SetColider(new SphereCollider(XMVECTOR{ 0,radius - 2,0,0 }, radius - 2));



  fbx3d28->SetColider(new WallCollider(XMVECTOR{ 1,15,10,0 }));
  fbx3d29->SetColider(new WallCollider(XMVECTOR{ 1,15,10,0 }));
  fbx3d30->SetColider(new WallCollider(XMVECTOR{ 1,15,10,0 }));
  fbx3d31->SetColider(new WallCollider(XMVECTOR{ 1,15,10,0 }));

  //当たり判定の属性
  fbx3d2->SetVerExit();
  fbx3d6->SetVerObj();
  fbx3d3->SetVerBulletRed();
  fbx3d4->SetVerBulletBlue();
  fbx3d9->SetVer();
 // fbx3d10->SetVerObj();
  fbx3d21->SetVerObj();
  fbx3d6->SetVerWall();
  fbx3d10->SetVerWall();
  fbx3d11->SetVerWall();
  fbx3d12->SetVerWall(); 
  fbx3d13->SetVerWall();
  fbx3d14->SetVerWall();
  fbx3d15->SetVerWall();
  fbx3d16->SetVerWall();
  fbx3d17->SetVerWall();
  fbx3d18->SetVerWall();
  fbx3d19->SetVerWall();
  fbx3d27->SetVerObj2();
//  fbx3d32->SetVerObj2();
  fbx3d20->SetVerSPHEREOBJ();
  fbx3d28->SetVerWall();
  fbx3d29->SetVerWall();
  fbx3d30->SetVerWall();
  fbx3d31->SetVerWall();
//  fbx3d21->SetVerWall();

  wall1->SetVerWall();
  wall2->SetVerWall();
  wall3->SetVerWall();
  wall4->SetVerWall();
  //fbx3d2->PlayAnimation2();
  fbx3d9->PlayAnimation2();

  scene->SetHwnd(winApp->GetHwnd());
  mapedit->SetHwnd(winApp->GetHwnd());
  int c = 0;
  int portaltime = 0;
  XMVECTOR mo;
  XMFLOAT3 posi;

 
    while (true)  // ゲームループ
    {
     //   Fbx3d::SetCamera(camera);
        if (scene->GetScene() == -1)
        {

            fbx3d3->Initialize2();
            fbx3d4->Initialize2();
            fbx3d21->Initialize2();
            fbx3d9->Initialize2();
            fbx3d27->Initialize2();
            scene->Initialize2();
            camera->Initialize2();
            wall1->SetPosition({ -90, 0, 0 });
            wall2->SetPosition({ +90, 0, 0 });
            wall3->SetPosition({ 0, 0,-90 });
            wall4->SetPosition({ 0, 0,+90 });

            fbx3d1->SetPosition({ 0, -10, 0 });
            fbx3d1->SetRotate({ 0,0,0 });

            fbx3d2->SetPosition({ -60,1,100 });
            fbx3d2->SetScale({ 0.1,0.1, 0.1 });
            fbx3d2->SetRotate({ 0,0,180 });

            fbx3d3->SetScale({ 0.05, 0.05,0.05 });
            fbx3d4->SetScale({ 0.05, 0.05,0.05 });

            fbx3d3->SetPosition({ 0, 0, -20 });
            fbx3d9->SetPosition({ 0, 0, -20 });


            fbx3d5->SetPosition({ 105, 80, 0 });
            fbx3d5->SetRotate({ 90,270,0 });

            fbx3d22->SetPosition({ -105, 80, 0 });
            fbx3d22->SetRotate({ 90,90,0 });



            fbx3d6->SetPosition({ 0, -10, 0 });
            fbx3d6->SetRotate({ 0,0,0 });


            fbx3d7->SetPosition({ 0, 80, -105 });
            fbx3d7->SetRotate({ 90,0,0 });

            fbx3d8->SetPosition({ 0, 0, 106 });
            fbx3d8->SetRotate({ 270,0,0 });

            fbx3d10->SetPosition({ 0, 0, -12 });
            fbx3d10->SetScale({ 0.1,0.1,0.1 });
            fbx3d10->SetRotate({ 0,0,0 });

            fbx3d11->SetPosition({ 20, 0, -12 });
            fbx3d11->SetScale({ 0.1,0.1,0.1 });
            fbx3d11->SetRotate({ 0,0,0 });

            fbx3d12->SetPosition({ 40, 0, -12 });
            fbx3d12->SetScale({ 0.1,0.1,0.1 });
            fbx3d12->SetRotate({ 0,0,0 });

            fbx3d13->SetPosition({ 60, 0, -12 });
            fbx3d13->SetScale({ 0.1,0.1,0.1 });
            fbx3d13->SetRotate({ 0,0,0 });

            fbx3d14->SetPosition({ 80, 0, -12 });
            fbx3d14->SetScale({ 0.1,0.1,0.1 });
            fbx3d14->SetRotate({ 0,0,0 });

            fbx3d15->SetPosition({ -20, 0, -12 });
            fbx3d15->SetScale({ 0.1,0.1,0.1 });
            fbx3d15->SetRotate({ 0,0,0 });

            fbx3d16->SetPosition({ -40, 0, -12 });
            fbx3d16->SetScale({ 0.1,0.1,0.1 });
            fbx3d16->SetRotate({ 0,0,0 });

            fbx3d17->SetPosition({ -60, 0, -12 });
            fbx3d17->SetScale({ 0.1,0.1,0.1 });
            fbx3d17->SetRotate({ 0,0,0 });

            fbx3d18->SetPosition({ -80, 0, -12 });
            fbx3d18->SetScale({ 0.1,0.1,0.1 });
            fbx3d18->SetRotate({ 0,0,0 });

            fbx3d19->SetPosition({ 0, 0, -12 });
            fbx3d19->SetScale({ 0.1,0.1,0.1 });
            fbx3d19->SetRotate({ 0,0,0 });

            fbx3d20->SetPosition({ 60, -6, -30 });
            fbx3d20->SetScale({ 0.014,0.014,0.014 });
            fbx3d20->SetRotate({ 0,0,0 });

            fbx3d21->SetPosition({ 60, -1.8, -30 });
            fbx3d21->SetScale({ 0.012,0.012,0.012 });
            fbx3d21->SetRotate({ 0,45,90 });

            fbx3d23->SetPosition({ 0,0,0 });
            fbx3d23->SetScale({ 0.1,0.1,0.1 });
            fbx3d23->SetRotate({ 0,0,270 });

            fbx3d24->SetPosition({ 0,0,0 });
            fbx3d24->SetScale({ 0.1,0.1,0.1 });
            fbx3d24->SetRotate({ 0,0,270 });

            fbx3d25->SetPosition({ 0,-1,-14 });
            fbx3d25->SetScale({ 0.04,0.04,0.04 });
            fbx3d25->SetRotate({ 0,360,0 });


            fbx3d26->SetPosition({ 28,-1,-60 });
            fbx3d26->SetScale({ 0.04,0.04,0.04 });
            fbx3d26->SetRotate({ 0,90,0 });

            fbx3d27->SetPosition({ 30,0,-41 });
            fbx3d27->SetScale({ 3.5,3.5,3.5 });
            fbx3d27->SetRotate({ 0,0,0 });

            fbx3d28->SetPosition({ 30, 0, -22 });
            fbx3d28->SetScale({ 0.1,0.1,0.1 });
            fbx3d28->SetRotate({ 0,90,0 });

            fbx3d29->SetPosition({ 30, 0, -62 });
            fbx3d29->SetScale({ 0.1,0.1,0.1 });
            fbx3d29->SetRotate({ 0,90,0 });

            fbx3d30->SetPosition({ 30, 0, -82 });
            fbx3d30->SetScale({ 0.1,0.1,0.1 });
            fbx3d30->SetRotate({ 0,90,0 });

            fbx3d31->SetPosition({ 30, 0, -102 });
            fbx3d31->SetScale({ 0.1,0.1,0.1 });
            fbx3d31->SetRotate({ 0,90,0 });

            fbx3d32->SetPosition({ 30,0,-37 });
            fbx3d32->SetScale({ 3.5,3.5,3.5 });
            fbx3d32->SetRotate({ 0,0,0 });

            fbx3d33->SetPosition({ 0,0,0 });
            fbx3d33->SetScale({ 0.1,0.1,0.1 });
            fbx3d33->SetRotate({ 0,0,270 });


            fbx3d34->SetPosition({ 300,10,60 });
            fbx3d34->SetScale({ 0.1,0.1, 0.1 });
            fbx3d34->SetRotate({ 0,0,180 });

            fbx3d35->SetPosition({ -45,2,90 });
            fbx3d35->SetScale({ 0.02,0.02,0.02 });
            fbx3d35->SetRotate({ 0,90,0 });



            fbx3d36->SetPosition({ 307,10,60 });
            fbx3d36->SetScale({ 0.1,0.1, 0.1 });
            fbx3d36->SetRotate({ 0,0,180 });


            fbx3d37->SetPosition({ 40,-1,-14 });
            fbx3d37->SetScale({ 0.04,0.04,0.04 });
            fbx3d37->SetRotate({ 0,360,0 });


        }
        if (fbx3d9->GetColision() == false)
        {
            posi = camera->GetEye();
        }


        scene->MapEditScene();

        //座標Set関連   
        //fbx3d2->SetMyPosition(fbx3d9->GetMyPosition());
       // fbx3d2->SetCameraAxisZ(camera->GetCameraZAxis());
       // fbx3d2->SetTarget(camera->GetTargetPos());

        fbx3d10->SetMyPosition(fbx3d9->GetMyPosition());
        fbx3d10->SetCameraAxisZ(camera->GetCameraZAxis());
        fbx3d10->SetTarget(camera->GetTargetPos());

        fbx3d21->SetMyPosition(fbx3d9->GetMyPosition());
        fbx3d21->SetCameraAxisZ(camera->GetCameraZAxis());
        fbx3d21->SetTarget(camera->GetTargetPos());

        fbx3d27->SetMyPosition(fbx3d9->GetMyPosition());
        fbx3d27->SetCameraAxisZ(camera->GetCameraZAxis());
        fbx3d27->SetTarget(camera->GetTargetPos());

        fbx3d32->SetMyPosition(fbx3d9->GetMyPosition());
        fbx3d32->SetCameraAxisZ(camera->GetCameraZAxis());
        fbx3d32->SetTarget(camera->GetTargetPos());

        fbx3d23->SetMyposition(fbx3d9->GetMyPosition());
        fbx3d24->SetMyposition(fbx3d9->GetMyPosition());
    //    fbx3d3->SetWorld(camera->GetRot());
    
      
        fbx3d3->SetMove(fbx3d9->GetMove());
        fbx3d4->SetMove(fbx3d9->GetMove());
        fbx3d3->SetPos(camera->GetPos());
        fbx3d4->SetPos(camera->GetPos());
       

      
        camera->SetWarpPosition(fbx3d9->GetPosition());
        camera->SetGround(fbx3d9->Getground());
        camera->SetScene(scene->GetScene());

        fbx3d21->SetShot(fbx3d4->GetShot());
        fbx3d21->SetShot2(fbx3d3->GetShot2());
        fbx3d21->SetWark(fbx3d9->GetWark());

        fbx3d23->GetFlag(fbx3d3->GetWarpFlag());
        fbx3d23->GetExitPosition(fbx3d3->GetPosition());
      
        fbx3d24->GetFlag(fbx3d4->GetWarpFlag2());
        fbx3d24->GetExitPosition(fbx3d4->GetPosition());


        fbx3d33->SetPosition({ fbx3d24->GetMyPosition() });
        fbx3d33->SetRotate({ fbx3d24->GetMyRotate() });
        fbx3d33->SetScale({ fbx3d24->GetMyScale() });

       
        Bluecamera->SetEyePos(fbx3d24->GetMyPosition());
        Redcamera->SetEyePos(fbx3d23->GetMyPosition());

        
     // sprite2->SpriteTransVertexBuffer();
     
    
     
     //sprintf_s(moji2, "camera=%f", camera->GetPositionY());
     //sprintf_s(moji2,"%d",camera->GetAngleY());
     
        if (winApp->ProcessMessage())
        {
            //ゲームループを抜ける
            break;
        }

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
            scene->Scene1();
        }
        lightGroup->Update();
        input->Update();
      
        
       
     //   Fbx3d::SetCamera(camera);
       // fbx3d9->SetWall(camera->GetWall());
     
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
                    //  
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





            //if (scene->GetScene() == 1)
            //{
            Bluecamera->Update();
            Redcamera->Update();
            fbx3d3->SetCameraT(camera->GetTargetPos());
            fbx3d4->SetCameraT(camera->GetTargetPos());
            fbx3d3->SetGet(fbx3d21->Getgetobj());
            fbx3d4->SetGet(fbx3d21->Getgetobj());


            fbx3d3->BlueBulletUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d4->RedBulletUpdate(camera->GetAngleX(), camera->GetAngleY());

            camera->SetBlueAngle(fbx3d3->GetMemoB());
            camera->SetRedAngle(fbx3d4->GetMemoR());

            fbx3d21->ObjUpdate(camera->GetAngleX(), camera->GetAngleY());

            fbx3d9->PlayerUpdate(camera->GetAngleX(), camera->GetAngleY());




            sprintf_s(moji, "%d", fbx3d9->GetDebug());


            fbx3d10->Update();
            fbx3d23->ExitUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d24->ExitUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d27->BoxObjUpdate(camera->GetAngleX(), camera->GetAngleY());
            //     fbx3d32->BoxObjUpdate(camera->GetAngleX(), camera->GetAngleY());
           //  }

             //追加



            input->GetScene(scene->GetScene());
            fbx3d2->Update();

            fbx3d1->Update();

            fbx3d5->Update();
            fbx3d6->Update();
            fbx3d7->Update();
            fbx3d8->Update();
            fbx3d11->Update();
            fbx3d12->Update();
            fbx3d13->Update();
            fbx3d14->Update();
            fbx3d15->Update();
            fbx3d16->Update();
            fbx3d17->Update();
            fbx3d18->Update();
            fbx3d19->Update();
            fbx3d20->Update();
            fbx3d22->Update();
            fbx3d25->Update();
            fbx3d26->Update();
            fbx3d23->ExitUpdate(camera->GetAngleX(), camera->GetAngleY());
            fbx3d28->Update();
            fbx3d29->Update();
            fbx3d30->Update();
            fbx3d31->Update();
            fbx3d34->Update();
            fbx3d35->Update();
            fbx3d36->Update();
            fbx3d37->Update();
            //        fbx3d33->Update();
            wall1->Update();
            wall2->Update();
            wall3->Update();
            wall4->Update();
            /*
            for (auto& object : objects) {
                object->Update();
            }


            for (auto& object : objects) {
                object->Draw();
            }
            */
            //    camera->SetAngleRedX(fbx3d4->GetAngleX2());
            //    camera->SetAngleRedY(fbx3d4->GetAngleY2());
            camera->SetRedTeleport(fbx3d9->GetredTeleport());
            //    camera->SetAngleBlueX(fbx3d3->GetAngleX1());
            camera->SetAngleBlueY(fbx3d3->GetAngleY1());
            camera->SetBlueTeleport(fbx3d9->GetblueTeleport());
            //camera->SetEyePos(fbx3d9->GetMyPosition());
            camera->SetBlue(fbx3d23->GetRot());
            camera->SetRed(fbx3d24->GetRot());
            Bluecamera->SetRot(fbx3d24->GetRot());
            Redcamera->SetRot(fbx3d23->GetRot());



            // パーティクル生成
            if (fbx3d4->RedP_Attack() == true) particleMan->CreateParticles(fbx3d4->GetPosition());
            if (fbx3d3->BlueP_Attack() == true) particleManBlue->CreateParticles(fbx3d3->GetPosition());
            //     if (fbx3d4->RedP_Flag() == true) particleMan->CreateParticles2(fbx3d4->GetPosition());
           //      if (fbx3d3->BlueP_Flag() == true) particleManBlue->CreateParticles2(fbx3d3->GetPosition());



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
       // fbx3d1->Draw2(dxCommon->GetCmdList());


        fbx3d23->RenPreDraw(dxCommon->GetCmdList());
 

        if (fbx3d3->GetWarpFlag() == true && fbx3d4->GetWarpFlag2() == true)
        {
            if (portaltime == 0)
            {
                fbx3d2_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d2->GetMatWorld());
                fbx3d5_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d5->GetMatWorld());
                fbx3d6_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d6->GetMatWorld());
                fbx3d7_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d7->GetMatWorld());
                fbx3d8_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d8->GetMatWorld());
                fbx3d9_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d9->GetMatWorld());
                fbx3d11_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d11->GetMatWorld());
                fbx3d12_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d12->GetMatWorld());
                fbx3d13_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d13->GetMatWorld());
                fbx3d14_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d14->GetMatWorld());
                fbx3d15_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d15->GetMatWorld());
                fbx3d16_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d16->GetMatWorld());
                fbx3d17_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d17->GetMatWorld());
                fbx3d18_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d18->GetMatWorld());
                fbx3d19_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d19->GetMatWorld());
                fbx3d20_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d20->GetMatWorld());
                fbx3d21_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d21->GetMatWorld());
                fbx3d22_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d22->GetMatWorld());
                fbx3d27_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d27->GetMatWorld());
                fbx3d28_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d28->GetMatWorld());
                fbx3d29_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d29->GetMatWorld());
                fbx3d30_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d30->GetMatWorld());
                fbx3d31_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d31->GetMatWorld());
                fbx3d35_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d35->GetMatWorld());
                fbx3d37_2->DrawPortalWindow(dxCommon->GetCmdList(), fbx3d37->GetMatWorld());

            }
            else
            {
                fbx3d2_2->Draw2(dxCommon->GetCmdList());
                fbx3d5_2->Draw2(dxCommon->GetCmdList());
                fbx3d6_2->Draw2(dxCommon->GetCmdList());
                fbx3d7_2->Draw2(dxCommon->GetCmdList());
                fbx3d8_2->Draw2(dxCommon->GetCmdList());
                fbx3d9_2->Draw2(dxCommon->GetCmdList());
                fbx3d10_2->Draw2(dxCommon->GetCmdList());
                fbx3d11_2->Draw2(dxCommon->GetCmdList());
                fbx3d12_2->Draw2(dxCommon->GetCmdList());
                fbx3d13_2->Draw2(dxCommon->GetCmdList());
                fbx3d14_2->Draw2(dxCommon->GetCmdList());
                fbx3d15_2->Draw2(dxCommon->GetCmdList());
                fbx3d16_2->Draw2(dxCommon->GetCmdList());
                fbx3d17_2->Draw2(dxCommon->GetCmdList());
                fbx3d18_2->Draw2(dxCommon->GetCmdList());
                fbx3d19_2->Draw2(dxCommon->GetCmdList());
                fbx3d20_2->Draw2(dxCommon->GetCmdList());
                fbx3d21_2->Draw2(dxCommon->GetCmdList());
                fbx3d22_2->Draw2(dxCommon->GetCmdList());
                fbx3d25_2->Draw2(dxCommon->GetCmdList());
                fbx3d26_2->Draw2(dxCommon->GetCmdList());
                fbx3d27_2->Draw2(dxCommon->GetCmdList());
                fbx3d28_2->Draw2(dxCommon->GetCmdList());
                fbx3d29_2->Draw2(dxCommon->GetCmdList());
                fbx3d30_2->Draw2(dxCommon->GetCmdList());
                fbx3d31_2->Draw2(dxCommon->GetCmdList());
                fbx3d33_2->Draw2(dxCommon->GetCmdList());
                fbx3d35_2->Draw2(dxCommon->GetCmdList());
                fbx3d37_2->Draw2(dxCommon->GetCmdList());
            }
        }
        else
        {
            portaltime = 0;
        }
      
        fbx3d23->RenPostDraw(dxCommon->GetCmdList());

        fbx3d24->RenPreDraw2(dxCommon->GetCmdList());
        if (fbx3d3->GetWarpFlag() == true && fbx3d4->GetWarpFlag2() == true)
        {
            if (portaltime == 0)
            {
                fbx3d2_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d2->GetMatWorld());
                fbx3d5_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d5->GetMatWorld());
                fbx3d6_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d6->GetMatWorld());
                fbx3d7_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d7->GetMatWorld());
                fbx3d8_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d8->GetMatWorld());
                fbx3d9_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d9->GetMatWorld());
                fbx3d11_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d11->GetMatWorld());
                fbx3d12_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d12->GetMatWorld());
                fbx3d13_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d13->GetMatWorld());
                fbx3d14_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d14->GetMatWorld());
                fbx3d15_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d15->GetMatWorld());
                fbx3d16_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d16->GetMatWorld());
                fbx3d17_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d17->GetMatWorld());
                fbx3d18_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d18->GetMatWorld());
                fbx3d19_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d19->GetMatWorld());
                fbx3d20_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d20->GetMatWorld());
                fbx3d21_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d21->GetMatWorld());
                fbx3d22_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d22->GetMatWorld());
                fbx3d27_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d27->GetMatWorld());
                fbx3d28_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d28->GetMatWorld());
                fbx3d29_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d29->GetMatWorld());
                fbx3d30_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d30->GetMatWorld());
                fbx3d31_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d31->GetMatWorld());
                fbx3d35_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d35->GetMatWorld());
                fbx3d37_3->DrawPortalWindowRed(dxCommon->GetCmdList(), fbx3d37->GetMatWorld());
                portaltime += 1;
            }
            else
            {
                fbx3d2_3->Draw2(dxCommon->GetCmdList());
                fbx3d5_3->Draw2(dxCommon->GetCmdList());
                fbx3d6_3->Draw2(dxCommon->GetCmdList());
                fbx3d7_3->Draw2(dxCommon->GetCmdList());
                fbx3d8_3->Draw2(dxCommon->GetCmdList());
                fbx3d9_3->Draw2(dxCommon->GetCmdList());
                fbx3d10_3->Draw2(dxCommon->GetCmdList());
                fbx3d11_3->Draw2(dxCommon->GetCmdList());
                fbx3d12_3->Draw2(dxCommon->GetCmdList());
                fbx3d13_3->Draw2(dxCommon->GetCmdList());
                fbx3d14_3->Draw2(dxCommon->GetCmdList());
                fbx3d15_3->Draw2(dxCommon->GetCmdList());
                fbx3d16_3->Draw2(dxCommon->GetCmdList());
                fbx3d17_3->Draw2(dxCommon->GetCmdList());
                fbx3d18_3->Draw2(dxCommon->GetCmdList());
                fbx3d19_3->Draw2(dxCommon->GetCmdList());
                fbx3d20_3->Draw2(dxCommon->GetCmdList());
                fbx3d21_3->Draw2(dxCommon->GetCmdList());
                fbx3d22_3->Draw2(dxCommon->GetCmdList());
                fbx3d25_3->Draw2(dxCommon->GetCmdList());
                fbx3d26_3->Draw2(dxCommon->GetCmdList());
                fbx3d27_3->Draw2(dxCommon->GetCmdList());
                fbx3d28_3->Draw2(dxCommon->GetCmdList());
                fbx3d29_3->Draw2(dxCommon->GetCmdList());
                fbx3d30_3->Draw2(dxCommon->GetCmdList());
                fbx3d31_3->Draw2(dxCommon->GetCmdList());
                fbx3d33_3->Draw2(dxCommon->GetCmdList());
                fbx3d35_3->Draw2(dxCommon->GetCmdList());
                fbx3d37_3->Draw2(dxCommon->GetCmdList());
            }
        }
        fbx3d24->RenPostDraw2(dxCommon->GetCmdList());

     //レンダ―テクスチャの描画
        sprite100->PreDrawScene(dxCommon->GetCmdList());
      
        //     sprite101->PreDrawScene(dxCommon->GetCmdList());
             ////スプライト共通コマンド
            // spriteCommon->PreDraw();

             //FBX描画
            // fbx3d1->Draw2(dxCommon->GetCmdList());
           //バックバッファの番号を取得（2つなので0番か1番）
      
     //   mapedit->CreateObj(dxCommon->GetCmdList());
        dxCommon->ImguiPre();

        if (scene->GetEdit() == false)
        {
            sprite100->SetPost(true);

            if (scene->GetScene() != 99)
            {

               
                //     mapedit->LoadSet(dxCommon->GetCmdList());


                fbx3d23->RenderFbxDraw(dxCommon->GetCmdList());
                fbx3d24->RenderFbxDraw2(dxCommon->GetCmdList());
                fbx3d2->Draw2(dxCommon->GetCmdList());
                fbx3d5->Draw2(dxCommon->GetCmdList());
                fbx3d6->Draw2(dxCommon->GetCmdList());
                fbx3d7->Draw2(dxCommon->GetCmdList());
                fbx3d8->Draw2(dxCommon->GetCmdList());
                //   fbx3d9->Draw2(dxCommon->GetCmdList());
                fbx3d10->Draw2(dxCommon->GetCmdList());
                fbx3d11->Draw2(dxCommon->GetCmdList());
                fbx3d12->Draw2(dxCommon->GetCmdList());
                fbx3d13->Draw2(dxCommon->GetCmdList());
                fbx3d14->Draw2(dxCommon->GetCmdList());
                fbx3d15->Draw2(dxCommon->GetCmdList());
                fbx3d16->Draw2(dxCommon->GetCmdList());
                fbx3d17->Draw2(dxCommon->GetCmdList());
                fbx3d18->Draw2(dxCommon->GetCmdList());
                fbx3d19->Draw2(dxCommon->GetCmdList());
                fbx3d20->Draw2(dxCommon->GetCmdList());
                fbx3d21->Draw2(dxCommon->GetCmdList());
                fbx3d22->Draw2(dxCommon->GetCmdList());
                fbx3d25->Draw2(dxCommon->GetCmdList());
                fbx3d26->Draw2(dxCommon->GetCmdList());
                fbx3d27->Draw2(dxCommon->GetCmdList());
                fbx3d28->Draw2(dxCommon->GetCmdList());
                fbx3d29->Draw2(dxCommon->GetCmdList());
                fbx3d30->Draw2(dxCommon->GetCmdList());
                fbx3d31->Draw2(dxCommon->GetCmdList());
                fbx3d33->Draw2(dxCommon->GetCmdList());
                fbx3d35->Draw2(dxCommon->GetCmdList());
                fbx3d37->Draw2(dxCommon->GetCmdList());
            }
            fbx3d34->Draw2(dxCommon->GetCmdList());
            fbx3d36->Draw2(dxCommon->GetCmdList());
        }
        else
        {
         
            sprite100->SetPost(false);
            fbx3d38->Draw2(dxCommon->GetCmdList());
            mapedit->CreateObj(dxCommon->GetCmdList());
        }
    
     //   mapedit->SetMousePositionX(scene->GetMousePoint());

        //      fbx3d32->Draw2(dxCommon->GetCmdList());
          //   // fbx3d23->RenPreDraw(dxCommon->GetCmdList());
           //   fbx3d23->RenPreDraw(dxCommon->GetCmdList());
          //    Bluecamera->Update();
          //    fbx3d23->RenPostDraw(dxCommon->GetCmdList());
 

        // fbx3d23->RenPostDraw(dxCommon->GetCmdList());
         // パーティクルの描画
        particleMan->Draw(dxCommon->GetCmdList());
        particleManBlue->Draw(dxCommon->GetCmdList());
        //ポストエフェクトここまで
        sprite100->PostDrawScene(dxCommon->GetCmdList());
        //     sprite101->PostDrawScene(dxCommon->GetCmdList());

        dxCommon->PreDraw();
     
        spriteCommon->PreDraw_Post();
        sprite100->PostUpdate();
    //    sprite101->Update();

     
        sprite100->PostDraw();
     
        //fbx3d23->RenderFbxDraw(dxCommon->GetCmdList());
      //  sprite101->PostDraw();


        if (fbx3d9->GetWark() == true)
        {
            //audio->SoundPlayWave("Resources/SOUND/Wark_sund.wav");//押す音
        }

        if (input->PushKey(DIK_ESCAPE))
        {
            break;
        }
        //3D描画前処理
      //  Object3d::PreDraw(dxCommon->GetCmdList());
          //3D描画
        //ここに処理追加できる
      
        //3D描画後処理
        //Object3d::PostDraw();
       

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

            //debug
           // sprite4->SpriteTransVertexBuffer();
           // sprite4->Update();
           // sprite4->SpriteDraw();
        }
        else if (scene->GetScene() == 100)
        {
            break;
        }

        sprite6->SpriteTransVertexBuffer();
        sprite6->Update();
        sprite6->SpriteDraw();
        sprite6->SetColor({ 1,1,1,scene->GetChange() });
        if (fbx3d9->GetColision() == false&&scene->GetEdit()==false)camera->ColisionAfterCameraSet(fbx3d9->GetMyPosition());

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
#pragma optimize( "", on )