#pragma once
#include "../../Engine/Base/DirectXCommon.h"
#include <DirectXMath.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/2d/Sprite.h"
#include "../../Engine/2d/SpriteCommon.h"
#include "../../Engine/2d/DebugText.h"
#include "../../Engine/3d/Model.h"
#include "../../Engine/3d/FbxLoader.h"
#include "../../Engine/3d/Fbx3d.h"
#include "../../Engine/2d/PostEffectCommon.h"
#include "../../Engine/2d/PostEffect.h"
#include "../Particle/ParticleManager.h"
#include "../../Engine/Light/LightGroup.h"
#include "SceneSelect.h"
#include "../../Engine/Camera/BlueCameraDebug.h"
#include "../../Engine/Camera/RedCameraDebug.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Camera/DebugCamera.h"
#include "../Game/Physics.h" 
#include "../Game/PlayerFbx.h"
#include "../Game/BulletFbx.h"
#include "../Game/PortalExit.h"
#include "../../Engine/Collision/SphereCollider.h"
#include "../../Engine/Collision/PlaneCollider.h"
#include "../../Engine/Collision/BoxCollider.h"
#include "../../Engine/Collision/Wallcolider.h"
#include "../../Engine/Collision/CollisionManager.h"
#include "../MapEdit/MapEdit.h"

class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public:
	GameScene();

	~GameScene();

	//シーン初期化
	void SceneInitialize(DirectXCommon* dxCommon, Input* input, Audio* audio,WinApp*winApp);
	//sprite初期化
	void SpriteInitialize(DirectXCommon* dxCommon,WinApp* winApp);
	//modelデータ読み込み
	void ModelLoadInitialize();
	//Fbx初期化
	void FbxInitialize();
	//mapedit初期化
	void EditInitialize();
	//シーン更新
	void SceneUpdate();
	//シーン描画
	void SceneDraw();


	bool GetGameEnd() { return End_flag; }

private://クラス変数

	int c = 0;
	int portaltime = 0;
	XMVECTOR mo;
	XMFLOAT3 posi;
	int demo = 0;
	float Cut_y_size = 0;

	//ゲームシーン処理関連
	char moji[64];
	char moji2[64];

	BYTE key[256] = {};
	BYTE olds[256] = {};

	bool End_flag = false;

	//Imgui用の調整データ
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.0001f,0.0001f,0.0001f };

	float spotLightDir[3] = { 0,-1,0 };
	float spotLightPos[3] = { 0,5,0 };
	float spotLightColor[3] = { 1,1,1 };
	float spotLightAtten[3] = { 0.0f,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f, 30.0f };

private://ポインタ
    
	//クラスのポインタ
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	WinApp* winApp_ = nullptr;
	SpriteCommon* spriteCommon = new SpriteCommon();
	SceneSelect* scene = nullptr;
	PostEffectCommon* postCommon = new PostEffectCommon();;
	PostEffect* postEffect = nullptr;
	LightGroup* lightGroup = nullptr;
	Physics* physics = nullptr;

	//カメラ
	DebugCamera* camera = nullptr;
	BlueCameraDebug* Bluecamera = nullptr;
	RedCameraDebug* Redcamera = nullptr;


	//パーティクル
	ParticleManager* particleManRed = nullptr;
	ParticleManager* particleManBlue = nullptr;

	//スプライト
	Sprite* spriteAim = nullptr;
    Sprite* spriteStartOP = nullptr;
	Sprite* spriteExitOP = nullptr;
	Sprite* spriteOP = nullptr;
	Sprite* spriteNameOP = nullptr;
	Sprite* spriteChangeScene = nullptr;
	Sprite* spriteSceneCut = nullptr;

	//デバックテキスト
	DebugText* debugtext = nullptr;
	DebugText* debugtext2 = nullptr;

	//モデル
	Model* model1 = nullptr;
	Model* model2 = nullptr;
	Model* model3 = nullptr;
	Model* model4 = nullptr;
	Model* model5 = nullptr;
	Model* model6 = nullptr;
	Model* model7 = nullptr;
	Model* model8 = nullptr;
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

	//オブジェクト
	BulletFbx* redBullet = nullptr;
	BulletFbx* blueBullet = nullptr;
	PlayerFbx* player = nullptr;
	PortalExit* redExit = nullptr;
	PortalExit* blueExit = nullptr;
	Fbx3d* fbx3d34 = nullptr;
	Fbx3d* fbx3d36 = nullptr;
	Fbx3d* fbx3d38 = nullptr;
	MapEdit* mapedit = nullptr;

	 //衝突マネージャー
	CollisionManager* collisionManager = nullptr;
};

