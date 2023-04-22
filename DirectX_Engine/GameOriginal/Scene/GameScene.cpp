#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <imgui.h>

using namespace DirectX;
class CollisionManager;

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

void GameScene::SceneInitialize(DirectXCommon* dxCommon, Input* input, Audio* audio, WinApp* winApp)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);
	assert(winApp);


	this->dxCommon_ = dxCommon;
	this->input_ = input;
	this->audio_ = audio;
	this->winApp_ = winApp;

	//初期化処理
	//物理
	physics = new Physics();
	option = new Option();
	//シーン生成
	scene = new SceneSelect(WinApp::window_width, WinApp::window_height, input);
	//カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	Bluecamera = new BlueCameraDebug(WinApp::window_width, WinApp::window_height);
	Redcamera = new RedCameraDebug(WinApp::window_width, WinApp::window_height);

	//パーティクル
    //パーティクルマネージャ生成
	particleManRed = ParticleManager::Create(dxCommon->GetDev(), camera);
	particleManBlue = ParticleManager::Create(dxCommon->GetDev(), camera);

	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };


	//FbxLoader初期化
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	// ライト静的初期化
	LightGroup::StaticInitialize(dxCommon->GetDev());
	// ライト生成
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, false);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 100.0f;
	pointLightPos[2] = 0.0f;

	pointLightPos2[0] = 400.5f;
	pointLightPos2[1] = 100.0f;
	pointLightPos2[2] = 100.0f;
	lightGroup->SetSpotLightActive(0, false);
	//ライトセット
	Fbx3d::SetLightGroup(lightGroup);
	//model読み込み
	ModelLoadInitialize();
	//Edit初期化
	EditInitialize();
	//FBX初期化
	FbxInitialize();

	//sprite読み込み
	SpriteInitialize(dxCommon,winApp);
	//ポストエフェクト用
	postCommon->PostInitialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	postEffect = PostEffect::PostCreate(postCommon, 100);
	spriteCommon->SpriteCommonLoadTexture(100, L"Resources/Red.png");




	scene->SetHwnd(winApp->GetHwnd());
	mapedit->SetHwnd(winApp->GetHwnd());
}

void GameScene::SpriteInitialize(DirectXCommon* dxCommon,WinApp* winApp)
{
	assert(dxCommon->GetDev());
	//sprite初期化
	spriteCommon->Initialize(dxCommon_->GetDev(), dxCommon_->GetCmdList(), winApp_->window_width, winApp_->window_height);

	//スプライト
	spriteAim = Sprite::Create(spriteCommon, 0);

	spriteCommon->SpriteCommonLoadTexture(0, L"Resources/Aiming.png");
	spriteAim->SetPosition({ 640,362.5,0 });
	spriteAim->SetSize({ 127,141 });
	spriteAim->SettexSize({ 127,141 });

	spriteOP = Sprite::Create(spriteCommon, 1);

	spriteCommon->SpriteCommonLoadTexture(1, L"Resources/UI/NormalOP.png");
	spriteOP->SetPosition({ 640,360,0 });
	spriteOP->SetSize({ 1280,720 });
	spriteOP->SettexSize({ 1280,720 });

	spriteStartOP = Sprite::Create(spriteCommon, 4);

	spriteCommon->SpriteCommonLoadTexture(4, L"Resources/UI/StartOP.png");
	spriteStartOP->SetPosition({ 640,360,0 });
	spriteStartOP->SetSize({ 1280,720 });
	spriteStartOP->SettexSize({ 1280,720 });

	spriteExitOP = Sprite::Create(spriteCommon, 3);

	spriteCommon->SpriteCommonLoadTexture(3, L"Resources/UI/ExitOP.png");
	spriteExitOP->SetPosition({ 640,360,0 });
	spriteExitOP->SetSize({ 1280,720 });
	spriteExitOP->SettexSize({ 1280,720 });

	spriteNameOP = Sprite::Create(spriteCommon, 6);

	spriteCommon->SpriteCommonLoadTexture(6, L"Resources/UI/NameOP.png");
	spriteNameOP->SetPosition({ 640,360,0 });
	spriteNameOP->SetSize({ 1280,720 });
	spriteNameOP->SettexSize({ 1280,720 });

	spriteChangeScene = Sprite::Create(spriteCommon, 7);

	spriteCommon->SpriteCommonLoadTexture(7, L"Resources/UI/Change.png");
	spriteChangeScene->SetPosition({ 640,360,0 });
	spriteChangeScene->SetSize({ 1280,720 });
	spriteChangeScene->SettexSize({ 1280,720 });

	spriteSceneCut = Sprite::Create(spriteCommon, 8);

	spriteCommon->SpriteCommonLoadTexture(8, L"Resources/UI/SceneCut.png");
	spriteSceneCut->SetPosition({ 640,360,0 });
	spriteSceneCut->SetSize({ 1280,720 });
	spriteSceneCut->SettexSize({ 1280,720 });

	spriteUnder = Sprite::Create(spriteCommon, 9);

	spriteCommon->SpriteCommonLoadTexture(9, L"Resources/UI/txt_under.png");
	spriteUnder->SetPosition({ 640,360,0 });
	spriteUnder->SetSize({ 1280,720 });
	spriteUnder->SettexSize({ 1280,720 });

	spriteMenu = Sprite::Create(spriteCommon, 10);

	spriteCommon->SpriteCommonLoadTexture(10, L"Resources/UI/Menu.png");
	spriteMenu->SetPosition({ 640,360,0 });
	spriteMenu->SetSize({ 1280,720 });
	spriteMenu->SettexSize({ 1280,720 });

	spriteMenuGame = Sprite::Create(spriteCommon, 11);

	spriteCommon->SpriteCommonLoadTexture(11, L"Resources/UI/Menu_game.png");
	spriteMenuGame->SetPosition({ 640,360,0 });
	spriteMenuGame->SetSize({ 1280,720 });
	spriteMenuGame->SettexSize({ 1280,720 });

	spriteMenuOp = Sprite::Create(spriteCommon, 12);

	spriteCommon->SpriteCommonLoadTexture(12, L"Resources/UI/Menu_op.png");
	spriteMenuOp->SetPosition({ 640,360,0 });
	spriteMenuOp->SetSize({ 1280,720 });
	spriteMenuOp->SettexSize({ 1280,720 });

	spriteMenuExit = Sprite::Create(spriteCommon, 13);

	spriteCommon->SpriteCommonLoadTexture(13, L"Resources/UI/Menu_exit.png");
	spriteMenuExit->SetPosition({ 640,360,0 });
	spriteMenuExit->SetSize({ 1280,720 });
	spriteMenuExit->SettexSize({ 1280,720 });

	spriteCusor = Sprite::Create(spriteCommon, 14);

	spriteCommon->SpriteCommonLoadTexture(14, L"Resources/cusor.png");
	spriteCusor->SetPosition({ 640,360,0 });
	spriteCusor->SetSize({ 40,40 });
	spriteCusor->SettexSize({ 1600,1200 });

	spriteNavi1 = Sprite::Create(spriteCommon, 15);

	spriteCommon->SpriteCommonLoadTexture(15, L"Resources/UI/Navi1.png");
	spriteNavi1->SetPosition({ 640,253,0 });
	spriteNavi1->SetSize({ 1280,720 });
	spriteNavi1->SettexSize({ 1280,720 });

	spriteNavi2 = Sprite::Create(spriteCommon, 16);

	spriteCommon->SpriteCommonLoadTexture(16, L"Resources/UI/Navi2.png");
	spriteNavi2->SetPosition({ 640,253,0 });
	spriteNavi2->SetSize({ 1280,720 });
	spriteNavi2->SettexSize({ 1280,720 });

	//デバックテキスト
	debugtext = new DebugText();
	debugtext2 = new DebugText();


	const int debugTextTexNumber = 2;

	spriteCommon->SpriteCommonLoadTexture(debugTextTexNumber, L"Resources/ASC_white1280.png");
	debugtext->debugTextInit(spriteCommon, debugTextTexNumber);
	debugtext2->debugTextInit(spriteCommon, debugTextTexNumber);
}

void GameScene::ModelLoadInitialize()
{
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
	model31 = FbxLoader::GetInstance()->LoadModelFromFile("BlockPortalBox");
	model32 = FbxLoader::GetInstance()->LoadModelFromFile("Button");
	model33 = FbxLoader::GetInstance()->LoadModelFromFile("taletto");
	model34 = FbxLoader::GetInstance()->LoadModelFromFile("BlueWalk");
}

void GameScene::FbxInitialize()
{
	Fbx3d::SetDevice(dxCommon_->GetDev());
	Fbx3d::SetCamera(camera);
	Fbx3d::SetBlueCamera(Bluecamera);
	Fbx3d::SetRedCamera(Redcamera);
	Fbx3d::CreateGraphicsPipeline();


	PlayerFbx::SetRedCamera(Redcamera);

    RenderTexture::SetDevice(dxCommon_->GetDev());
	RenderTexture::SetCamera(camera);
	RenderTexture::PortalCreateGraphicsPipeline();

	//3Dオブジェクト生成とモデルのセット
	blueBullet = new BulletFbx(input_);
	blueBullet->Initialize();
	blueBullet->SetModel(model3);

	redBullet = new BulletFbx(input_);
	redBullet->Initialize();
	redBullet->SetModel(model4);


	player = new PlayerFbx(input_, physics);
	player->Initialize();
	player->SetModel(model34);


	playerBlue = new Fbx3d(input_);
	playerBlue->Initialize();
	playerBlue->SetModel(model34);

	redExit = new PortalExit(input_);
	redExit->RenderFbxInitialize();
	redExit->SetModel(model13);

	blueExit = new PortalExit(input_);
	blueExit->RenderFbxInitialize();
	blueExit->SetModel(model13_2);

	fbx3d34 = new Fbx3d(input_);
	fbx3d34->Initialize();
	fbx3d34->SetModel(model20);

	fbx3d36 = new Fbx3d(input_);
	fbx3d36->Initialize();
	fbx3d36->SetModel(model21);

	fbx3d38 = new Fbx3d(input_);
	fbx3d38->Initialize();
	fbx3d38->SetModel(model23);

	blueBullet->SetScale({ 0.05, 0.05,0.05 });
	redBullet->SetScale({ 0.05, 0.05,0.05 });

	blueBullet->SetPosition({ 0, 0, -20 });
	player->SetPosition({ 0, 0, -20 });
	player->SetRotate({ 0,0,0 });
	player->SetScale({ 5,5,5 });

	redExit->SetPosition({ 0,0,0 });
	redExit->SetScale({ 0.1,0.1,0.1 });
	redExit->SetRotate({ 0,0,270 });

	blueExit->SetPosition({ 0,0,0 });
	blueExit->SetScale({ 0.1,0.1,0.1 });
	blueExit->SetRotate({ 0,0,270 });

	fbx3d34->SetPosition({ 300,10,60 });
	fbx3d34->SetScale({ 0.1,0.1, 0.1 });
	fbx3d34->SetRotate({ 0,0,180 });

	fbx3d36->SetPosition({ 307,10,60 });
	fbx3d36->SetScale({ 0.1,0.1, 0.1 });
	fbx3d36->SetRotate({ 0,0,180 });

	fbx3d38->SetPosition({ 0,0,0 });
	fbx3d38->SetScale({ 1,1,1 });
	fbx3d38->SetRotate({ 0,0,0 });

	collisionManager = CollisionManager::GetInstance();

	float radius = 5.0f;
	float PlayerRadius = 1.0f;

	redBullet->SetColider(new SphereCollider(XMVECTOR{ 0,0,0,0 }, radius));
	blueBullet->SetColider(new SphereCollider(XMVECTOR{ 0,0,0,0 }, radius));
	player->SetColider(new SphereCollider(XMVECTOR{ 0,PlayerRadius,0,0 }, PlayerRadius));

	//当たり判定の属性
	redBullet->SetVerBulletRed();
	blueBullet->SetVerBulletBlue();
	player->SetVer();
	player->PlayAnimation2();
	playerBlue->PlayAnimation2();
}

void GameScene::EditInitialize()
{
	mapedit = new MapEdit(WinApp::window_width, WinApp::window_height, input_);
	mapedit->SetFenceModel(model10);
	mapedit->SetWallModel(model27);
	mapedit->SetFloorModel(model26);
	mapedit->SetDrumModel(model11);
	mapedit->SetCollisionModel(model24);
	mapedit->SetExitModel(model2);
	mapedit->SetGunModel(model12);
	mapedit->SetBoxModel(model17);
	mapedit->SetBlockModel(model31);
	mapedit->SetTalletModel(model33);
	mapedit->SetRespawnModel(model25);
	mapedit->SetRedArowModel(model28);
	mapedit->SetGreenArowModel(model29);
	mapedit->SetBlueArowModel(model30);

	mapedit->Loadtxt();
	MapEdit::SetMapCamera(camera);
}


void GameScene::SceneUpdate()
{
	lightGroup->Update();
	//初期化
	if (scene->GetScene() == -1)
	{
		redBullet->Initialize2();
		blueBullet->Initialize2();
		player->Initialize2();
		scene->SceneInitialize();
		camera->Initialize2();
		redBullet->SetScale({ 0.05, 0.05,0.05 });
		blueBullet->SetScale({ 0.05, 0.05,0.05 });
		redBullet->SetPosition({ 0, 0, -20 });
		player->SetPosition({ 0, 0, -20 });
		redExit->SetPosition({ 0,0,0 });
		redExit->SetScale({ 0.1,0.1,0.1 });
		redExit->SetRotate({ 0,0,270 });
		blueExit->SetPosition({ 0,0,0 });
		blueExit->SetScale({ 0.1,0.1,0.1 });
		blueExit->SetRotate({ 0,0,270 });
		fbx3d34->SetPosition({ 300,10,60 });
		fbx3d34->SetScale({ 0.1,0.1, 0.1 });
		fbx3d34->SetRotate({ 0,0,180 });
		fbx3d36->SetPosition({ 307,10,60 });
		fbx3d36->SetScale({ 0.1,0.1, 0.1 });
		fbx3d36->SetRotate({ 0,0,180 });
		
	}
	if (player->GetColision() == false)
	{
		posi = camera->GetEye();
	}

	if (scene->GetScene() == 0)
	{
	    Cut_y_size = 0;
		Tutorial_num = 0;
		Second_text_flag = true;
		player->PlayerResetTitle();
		mapedit->MapInitialize();
		lightGroup->SetDirLightActive(0, false);
		lightGroup->SetDirLightActive(1, false);
		lightGroup->SetDirLightActive(2, false);
		lightGroup->SetPointLightActive(0, true);
		lightGroup->SetPointLightActive(1, false);
		pointLightPos[0] = 0.5f;
		pointLightPos[1] = 100.0f;
		pointLightPos[2] = 0.0f;

		pointLightPos2[0] = 400.5f;
		pointLightPos2[1] = 100.0f;
		pointLightPos2[2] = 100.0f;
		lightGroup->SetSpotLightActive(0, false);
	}



	scene->MapEditScene();

	//座標Set関連   
	mapedit->SetMyPosition(player->GetMyPosition());
	mapedit->SetCameraAxisZ(camera->GetCameraZAxis());
	mapedit->SetTarget(camera->GetTargetPos());
	redExit->SetMyposition(player->GetMyPosition());
	blueExit->SetMyposition(player->GetMyPosition());
	redBullet->SetMove(player->GetMove());
	blueBullet->SetMove(player->GetMove());
	redBullet->SetPos(camera->GetPos());
	blueBullet->SetPos(camera->GetPos());
	camera->SetWarpPosition(player->GetPosition());
	camera->SetGround(player->Getground());
	camera->SetScene(scene->GetScene());
	camera->SetMenuFlag(scene->GetMenuFlag());
	mapedit->SetShotBlue(blueBullet->GetShot());
	mapedit->SetShotRed(redBullet->GetShot2());
	mapedit->SetWark(player->GetWark());
	mapedit->SetTutorial(scene->GetTutorial_2());
	redExit->SetRotateFlag(redBullet->GetCollisionFbxFlag());
	blueExit->SetRotateFlag(blueBullet->GetCollisionFbxFlag());
	redExit->GetFlag(redBullet->GetWarpFlag());
	redExit->GetExitPosition(redBullet->GetPosition());
	blueExit->GetFlag(blueBullet->GetWarpFlag2());
	blueExit->GetExitPosition(blueBullet->GetPosition());
	Bluecamera->SetEyePos(blueExit->GetMyPosition());
	Redcamera->SetEyePos(redExit->GetMyPosition());
	scene->SetFirstIvent(camera->GetIventFlag());
	Bluecamera->SetMatRot(camera->GetRot());
	Redcamera->SetMatRot(camera->GetRot());


	float in = 2;
	if (player->Getground() == true)
	{
		in = 1;
	}
	else
	{
		in = 0;
	}

	sprintf_s(moji2, "%f", in);
	//sprintf_s(moji2,"%d",camera->GetAngleY());

	// DirectX毎フレーム処理　ここから

	fbx3d36->SetRotate({ 0,scene->GetDoorY(),180 });


	if (scene->GetScene() == 0)
	{
		scene->MouseGet();
		scene->Title();
		scene->PadScene();
	}
	else
	{
		if (scene->GetMenuFlag() == true)
		{
			scene->MouseGet();
			scene->PadScene();
		}
		scene->ChangeScene();
		scene->MenuScene();
	}

	input_->SetMenu(scene->GetMenuFlag());
	input_->SetEdit(scene->GetEdit());
	input_->Update();

	if (scene->GetMenuFlag() == false)
	{
		lightGroup->Update();
	

		camera->SetColision(player->GetColision());
		camera->SetMove(player->GetMove());
		camera->SetFallV(player->GetFall());
		camera->SetColisionVec(player->GetColisionVec());
		camera->SetTutorial(scene->GetTutorial());
		if (scene->GetEdit() == false)camera->Update();



		option->SetMouseSensi(camera->GetMouseSensi());
		option->SetPadSensi(camera->GetPadSensi());
	



		sprintf_s(moji2, "%f", camera->GetUpx());

		//playerのセット

		player->SetMemo(blueBullet->GetMemo());
		player->SetWorld(camera->GetRot());
		player->SetMemo2(redBullet->GetMemo2());
		player->SetWarpFlag(redBullet->GetWarpFlag());
		player->SetWarpFlag2(blueBullet->GetWarpFlag2());
		player->SetCameraAxis(camera->GetCameraZAxis());
		player->SetPos(camera->GetEye());
		player->GetmoveOld(camera->GetMove());
		player->SetTutorial(scene->GetTutorial());
		player->SetTarget(camera->GetTarget());
		player->SetMenu(scene->GetMenuFlag());

		playerBlue->SetPosition(player->GetMyPosition());

		playerBlue->SetRotate(player->GetRotation());
		playerBlue->SetScale(player->GetScale());


		//追加
		player->CollisionAfter();

		int i = 0;


		//1フレームの当たり判定
		if (scene->GetEdit() == false)
		{
			while (1)
			{
				collisionManager->CheckAllCollisions();
				if (player->GetColision() == true)
				{
					camera->SetColision(player->GetColision());
					if (i < 4)
					{
						camera->SetMove(player->GetMove());
					}
					else if (i % 4 == 0)
					{
						camera->SetMove(camera->GetMoveOld());
						mo = camera->GetMoveOld();
					}
					camera->SetColisionVec(player->GetColisionVec());
					camera->Update();
					player->SetPos(camera->GetEye2());
					player->GetmoveOld(camera->GetMove());
					player->PlayerUpdate(camera->GetAngleX(), camera->GetAngleY());
					player->CollisionAfter();
					i += 1;
					c = i;
				}
				else
				{
					break;
				}

				if (i > 14 && player->Getground() == true)
				{
					camera->SetEye(posi);
					player->SetPos(camera->GetPos());
				}

				if (i > 60)
				{
					break;
				}
			}

			Bluecamera->Update();
			Redcamera->Update();
			redBullet->SetCameraT(camera->GetTargetPos());
			blueBullet->SetCameraT(camera->GetTargetPos());
			redBullet->SetGet(mapedit->GetGetGun());
			blueBullet->SetGet(mapedit->GetGetGun());




			redBullet->BlueBulletUpdate(camera->GetAngleX(), camera->GetAngleY());
			blueBullet->RedBulletUpdate(camera->GetAngleX(), camera->GetAngleY());

			camera->SetBlueAngle(redBullet->GetMemoB());
			camera->SetRedAngle(blueBullet->GetMemoR());
			player->PlayerUpdate(camera->GetAngleX(), camera->GetAngleY());


			bool PortalFlag = false;

			if (redBullet->GetWarpFlag() == true && blueBullet->GetWarpFlag2() == true)
			{
				PortalFlag = true;
			}
			redExit->SetPortalFlag(PortalFlag);
			blueExit->SetPortalFlag(PortalFlag);


			sprintf_s(moji, "%d", 0);

			float RedPattern = 1;
			float BluePattern = 2;

			redExit->ExitUpdate(camera->GetAngleX(), camera->GetAngleY(), RedPattern);
			blueExit->ExitUpdate(camera->GetAngleX(), camera->GetAngleY(), BluePattern);

			//追加
			input_->GetScene(scene->GetScene());
			redExit->ExitUpdate(camera->GetAngleX(), camera->GetAngleY(), RedPattern);
			fbx3d34->Update();
			fbx3d36->Update();
			camera->SetRedTeleport(player->GetredTeleport());
			camera->SetAngleBlueY(redBullet->GetAngleY1());
			camera->SetBlueTeleport(player->GetblueTeleport());

			if(player->GetredTeleport() == true)
			{
			camera->SetRed(blueExit->GetRot());
			}



			if (player->GetblueTeleport() == true)
			{
				camera->SetBlue(redExit->GetRot());
			}

			camera->SetRedRotate(blueExit->GetMyRotate());
			camera->SetBlueRotate(redExit->GetMyRotate());

			Bluecamera->SetRot(blueExit->GetRot());
			Redcamera->SetRot(redExit->GetRot());



			// パーティクル生成
			if (blueBullet->RedP_Attack() == true) particleManRed->CreateParticles(blueBullet->GetPosition());
			if (redBullet->BlueP_Attack() == true) particleManBlue->CreateParticles(redBullet->GetPosition());

			if (redBullet->GetWarpFlag() == true)
			{
				particleManBlue->CreateParticlesPortal(redBullet->GetPosition(),redBullet->GetCollisionFbxFlag());
			}

			if (blueBullet->GetWarpFlag2() == true)
			{
				particleManRed->CreateParticlesPortal(blueBullet->GetPosition(), blueBullet->GetCollisionFbxFlag());
			}
	

			particleManRed->Update();
			particleManBlue->BlueUpdate();

			scene->SetExit(player->GetExit());
		}
		else
		{
			//mapedit中は見づらいためライトを変える
			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, true);
			lightGroup->SetDirLightActive(2, true);
			lightGroup->SetPointLightActive(0, false);

			fbx3d38->Update();
			camera->MapEditUpdate();
		}

		if (scene->GetScene() == 99)
		{
			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, true);
			lightGroup->SetDirLightActive(2, true);
			lightGroup->SetPointLightActive(0, false);
		}
		else
		{
			lightGroup->SetDirLightActive(0, false);
			lightGroup->SetDirLightActive(1, false);
			lightGroup->SetDirLightActive(2, false);
			lightGroup->SetPointLightActive(0, true);
		}

		if (scene->GetEdit() == true)
		{
			//mapedit中は見づらいためライトを変える
			lightGroup->SetDirLightActive(0, true);
			lightGroup->SetDirLightActive(1, true);
			lightGroup->SetDirLightActive(2, true);
			lightGroup->SetPointLightActive(0, false);

		}
	}
		if (scene->GetScene() == 100)
		{
			End_flag = true;
		}

		scene->SetWalkTutorial(player->GetTutorialWalk());
		mapedit->SetScene(scene->GetScene());
	
		if (camera->GetIventFlag() == false&& player->GetTutorialWalk() == false&&scene->GetScene() == 1)
		{
			Tutorial_num = 1;
		}
}

void GameScene::SceneDraw()
{
	

	//FBX描画
	redExit->RenPreDraw(dxCommon_->GetCmdList());	    
	int Blue_camera_select = 2;
	playerBlue->SetIsPlay(player->GetIsPlay());
	playerBlue->DrawPortalWindow(Blue_camera_select);
	if (redBullet->GetWarpFlag() == true)
	{
		playerBlue->Draw2(dxCommon_->GetCmdList());
		mapedit->BlueCameraObjUpdate(dxCommon_->GetCmdList());
	}
	if (redBullet->GetWarpFlag() == false || blueBullet->GetWarpFlag2() == false)
	{
		portaltime = 0;
	}
	redExit->RenPostDraw(dxCommon_->GetCmdList());

	blueExit->RenPreDraw2(dxCommon_->GetCmdList());	
	if (blueBullet->GetWarpFlag2() == true)
	{
		player->Draw2(dxCommon_->GetCmdList());
		mapedit->RedCameraObjUpdate(dxCommon_->GetCmdList());
	}
	blueExit->RenPostDraw2(dxCommon_->GetCmdList());


	//レンダ―テクスチャの描画
	postEffect->PreDrawScene(dxCommon_->GetCmdList());
	//バックバッファの番号を取得（2つなので0番か1番）
	dxCommon_->ImguiPre();

	if (scene->GetScene() == 2)
	{
		pointLightPos[0] = 400.5f;
		pointLightPos[1] = 100.0f;
		pointLightPos[2] = 0.0f;

		lightGroup->SetPointLightActive(1, true);

		lightGroup->SetPointLightPos(1, XMFLOAT3(pointLightPos2));
		lightGroup->SetPointLightColor(1, XMFLOAT3(pointLightColor2));
		lightGroup->SetPointLightAtten(1, XMFLOAT3(pointLightAtten2));
	}

	if (scene->GetScene() == 3)
	{
		pointLightPos[0] = 800.5f;
		pointLightPos[1] = 100.0f;
		pointLightPos[2] = 0.0f;

	}
	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));

	if (scene->GetEdit() == false)
	{
		postEffect->SetPost(true);

		if (scene->GetScene() != 99)
		{
			redExit->RenderFbxDraw(dxCommon_->GetCmdList());
			blueExit->RenderFbxDraw2(dxCommon_->GetCmdList());
			mapedit->LoadSet(dxCommon_->GetCmdList());
			mapedit->MapObjUpdate();
			mapedit->MapEditDraw(dxCommon_->GetCmdList());
		}

		
		fbx3d34->Draw2(dxCommon_->GetCmdList());
		fbx3d36->Draw2(dxCommon_->GetCmdList());
	}
	else
	{
		postEffect->SetPost(false);
		fbx3d38->Draw2(dxCommon_->GetCmdList());
		mapedit->CreateObj(dxCommon_->GetCmdList());
	}

	// パーティクルの描画
	if (scene->GetScene() != 99)
	{
		particleManRed->Draw(dxCommon_->GetCmdList());
		particleManBlue->Draw(dxCommon_->GetCmdList());
	}
	//ポストエフェクトここまで
	postEffect->PostDrawScene(dxCommon_->GetCmdList());

	dxCommon_->PreDraw();
	postCommon->PostPreDraw();
	postEffect->PostUpdate();
	postEffect->PostDraw();

	////スプライト共通コマンド
	spriteCommon->PreDraw();
	//debugtext->Print(moji2, 100, 100);
    //debugtext->DrawAll();//的カウント

//    debugtext2->Print(moji2, 100, 200);
//    debugtext2->DrawAll();//的カウント


		//スプライト表示
	if (scene->GetScene() == 0)
	{

		spriteNameOP->SpriteTransVertexBuffer();
		spriteNameOP->Update();
		spriteNameOP->SpriteDraw();

		if (scene->GetRed() == 0)
		{
			spriteOP->SpriteTransVertexBuffer();
			spriteOP->Update();
			spriteOP->SpriteDraw();
		}
		else  if (scene->GetRed() == 1)
		{
			spriteStartOP->SpriteTransVertexBuffer();
			spriteStartOP->Update();
			spriteStartOP->SpriteDraw();
		}
		else  if (scene->GetRed() == 2)
		{
			spriteExitOP->SpriteTransVertexBuffer();
			spriteExitOP->Update();
			spriteExitOP->SpriteDraw();
		}
	}
	else  if (scene->GetScene() == 1|| scene->GetScene() == 2 || scene->GetScene() == 3)
	{
		if(scene->GetTutorial()==false&& Cut_y_size<600)Cut_y_size += 1.0f;
		//シーンカット
		spriteSceneCut->SpriteTransVertexBuffer();
		spriteSceneCut->SetSize({ 1280, 720+Cut_y_size});
		spriteSceneCut->Update();
		spriteSceneCut->SpriteDraw();
		//自キャラ

		if (scene->GetTutorial() == false)
		{
			spriteAim->SpriteTransVertexBuffer();
			spriteAim->Update();
			spriteAim->SpriteDraw();
		}
		//テキスト
	
		if (Under_sprite_txt == true)
		{
			spriteUnder->SpriteTransVertexBuffer();
			spriteUnder->Update();
			spriteUnder->SpriteDraw();
		}
	}



	if (player->GetTutorialWalk() == true && scene->GetScene() == 1)
	{
		Tutorial_num = 2;

		if (scene->GetTutorial_2() == false&& mapedit->GetTutorialGun() == false)
		{

			TimeRate1 += 0.2f;

			float Time = min(TimeRate1 / 5.0f, 1.0f);
			float easeY = Physics::easingOut(start, end, Time);

			spriteNavi1->SetPosition({ 640,easeY,0 });
			spriteNavi1->SpriteTransVertexBuffer();
			spriteNavi1->Update();
			spriteNavi1->SpriteDraw();
		}

	}
	if (mapedit->GetTutorialGun() == true&&Tutorial_num==2&&scene->GetScene()==1)
	{
		Gun_txt_time += 1;
		Tutorial_num = 3;

		TimeRate2 += 0.2f;

		float Time = min(TimeRate2 / 5.0f, 1.0f);
		float easeY = Physics::easingOut(end, start, Time);

		if (easeY > start)
		{
			TimeRate1 = 0.0f;

			spriteNavi1->SetPosition({ 640,easeY,0 });
			spriteNavi1->SpriteTransVertexBuffer();
			spriteNavi1->Update();
			spriteNavi1->SpriteDraw();
		}
		else if(easeY <= start&&Gun_txt_time>1200)
		{
			TimeRate1 += 0.2f;



			float Time2 = min(TimeRate1 / 5.0f, 1.0f);
			float easeY2 = Physics::easingOut(start, end, Time2);

			spriteNavi2->SetPosition({ 640,easeY2,0 });
			spriteNavi2->SpriteTransVertexBuffer();
			spriteNavi2->Update();
			spriteNavi2->SpriteDraw();
		}
	}


	if (scene->GetScene() == 2)
	{
		if (Second_text_flag == true)
		{
			Tutorial_num = 4;
			Second_text_flag = false;
		}

		if (player->GetPositionZ() > 65.0f&&Tutorial_num == 4)
		{
			Tutorial_num = 5;
		}
	}
	if (scene->GetScene() == 3)
	{
		Tutorial_num = 6;
	}


	if (scene->GetMenuType() == 0&&scene->GetMenuFlag() ==true)
	{
		spriteMenu->SpriteTransVertexBuffer();
		spriteMenu->Update();
		spriteMenu->SpriteDraw();
	}
	else if (scene->GetMenuType() == 1 && scene->GetMenuFlag() == true)
	{
		spriteMenuGame->SpriteTransVertexBuffer();
		spriteMenuGame->Update();
		spriteMenuGame->SpriteDraw();
	}
	else if (scene->GetMenuType() == 2 && scene->GetMenuFlag() == true)
	{
		spriteMenuOp->SpriteTransVertexBuffer();
		spriteMenuOp->Update();
		spriteMenuOp->SpriteDraw();

		
	}
	else if (scene->GetMenuType() == 3 && scene->GetMenuFlag() == true)
	{
		spriteMenuExit->SpriteTransVertexBuffer();
		spriteMenuExit->Update();
		spriteMenuExit->SpriteDraw();
	}

	if(scene->GetMenuFlag()==true&&scene->GetOptionFlag() == true)
	{
		option->SettingSensi();
		camera->SetReverseSwitch(option->GetPadReverse());
		camera->SetMouseSensi(option->GetMouseSensi());
		camera->SetPadSensi(option->GetPadSensi());
	}

	spriteChangeScene->SpriteTransVertexBuffer();
	spriteChangeScene->Update();
	spriteChangeScene->SpriteDraw();
	spriteChangeScene->SetColor({ 1,1,1,scene->GetChange() });

	if (player->GetColision() == false && scene->GetEdit() == false)camera->ColisionAfterCameraSet(player->GetMyPosition());
	scene_ = scene->GetScene();
	camera->SetOldScene(scene->OldScene());

	// ４．描画コマンドここまで
	collisionManager->CheckAllCollisions();
}


