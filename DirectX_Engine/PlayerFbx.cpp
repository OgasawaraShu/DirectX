#include "PlayerFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"
#include "Collision.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void PlayerFbx::OnCollision(const CollisionInfo& info)
{
	//2進法で表現しているため4は赤球8は蒼球
	if(info.collider->attribute==4&&warpFlag==true)
	{
		blueCollision = true;
	}
	else if (info.collider->attribute == 8 && warpFlag == true)
	{
		redCollision = true;
	}
	
	
	if(info.collider->attribute == 32&&WallCollision==false)
	{
		WallCollision = true;
		ColisionPoint=info.inter;
	}
	else if(info.collider->attribute == 64 && WallCollision == false)
	{
		WallCollision = true;
		ColisionPoint = info.inter;
	}
	else if (info.collider->attribute == 128 && WallCollision == false)
	{
		WallCollision = true;
		ColisionPoint = info.inter;
	}

	if (info.collider->attribute == 256)
	{
		Exit = true;
	}
}

void PlayerFbx::Initialize_Bullet()
{

}

PlayerFbx::PlayerFbx(Input* input,Physics* physics)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
	this->physics = physics;

	
}

void PlayerFbx::PlayerUpdate(double angleX, double angleY)
{

	//前の座標を記録
	OldPos = position;
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();

	//パッドの更新
	GP->Update();
	//マウス角度出力Ver
	angleX_ = angleX;
	angleY_ = angleY;
	
	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//平行移動
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//追加の回転行列処理
	AddRotateMatrixUpdate(angleX_, angleY_, matRot);

    Landing();
	//ワープ処理
	WarpUpdate();
	//Jumpと落下の処理
	FallJump();
	//動く処理
	MoveMatrixUpdate(matRot,matTrans);
	//Matrix後更新
	PostMatrixUpdate(matScale, matRot, matTrans);
	//
	//RayCheck(angleX, angleY);

}

void PlayerFbx::FallJump()
{
	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	}
	// ジャンプ操作
	else if (input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 6.0f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}
}

void PlayerFbx::Landing()
{
	// レイのスタート地点を設定
	Ray ray;
	ray.start = { position.x,position.y+1.0f,position.z,0 };
	ray.start.m128_f32[1] += 5.0f;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	
    Plane plane;

    plane.normal = XMVectorSet(0, 1, 0, 0);
    plane.distance = 0.0f;
   
        
        XMVECTOR inter;
        float distance;
        bool hit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);
	
        if (hit&&distance<=5.0f) {

			onGround = true;
        }
		else
		{
			onGround = false;
		}
        

	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit, 5.0 * 2.0f + adsDistance)) {
			//onGround = false;
			position.y -= (raycastHit.distance - 5.0 * 2.0f);	
		}
		// 地面がないので落下
		else {
		//	onGround = true;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,5.0 * 2.0f)) {
			// 着地
		//	onGround = true;
			position.y -= (raycastHit.distance - 5.0 * 2.0f);
		}
	}
}

void PlayerFbx::RayCheck(float angleX, float angleY)
{
	// レイのスタート地点を設定
	Ray ray;
	ray.start = { position.x,position.y,position.z,0 };
	ray.start.m128_f32[1] += 2.5f;
	ray.dir = { 0,0,1,0};
	RaycastHit raycastHit;

	XMMATRIX matRot;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	// 追加回転分の回転行列を生成
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX);
	matRotNew *= XMMatrixRotationY(-angleY);
	// 累積の回転行列を合成
	// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
	// クォータニオンを使用する方が望ましい
	matRot = matRotNew * matRot;

	ray.dir = CammeraZAxis;


	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit)) {
		//onGround = false;
	}
}

void PlayerFbx::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
{
	
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;


//ビュープロジェクション行列
	const XMMATRIX& matViewProjection =
		camera->GetViewProjectionMatrix();
	//メッシュtランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;

	//定数バッファへ転送

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}

	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);



	//当たり判定更新
	if (collider) {
		collider->Update();
	}


}

void PlayerFbx::WarpUpdate()
{
	if (TimeWarpF == false)
	{
		WarpTime += 1;
	}

	if (WarpTime > 120)
	{
		WarpTime = 0;
		TimeWarpF = true;
	}

	//赤に当たってかつどちらも飛ばしている場合
	if (redCollision == true && warpFlag == true&&TimeWarpF==true && warpFlag2 == true)
	{

		Warp2.x = Warp.m128_f32[0];
		Warp2.y = Warp.m128_f32[1];
		Warp2.z = Warp.m128_f32[2];


		//position = Warp2;

		redTeleport = true;

		//onGround = false;
		TimeWarpF = false;
	}
	else if (redCollision == false)
	{
		redTeleport = false;
	}


	redCollision = false;

	//青に当たってかつどちらも飛ばしている場合
	if (blueCollision == true && warpFlag == true && TimeWarpF == true&&warpFlag2==true)
	{

		Warp2.x = Warpblue.m128_f32[0];
		Warp2.y = Warpblue.m128_f32[1];
		Warp2.z = Warpblue.m128_f32[2];

		blueTeleport = true;
		TimeWarpF = false;
	}
	else if (blueCollision == false)
	{
		blueTeleport = false;
	}

	blueCollision = false;
}

void PlayerFbx::AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot)
{
	// 追加回転分の回転行列を生成
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX);
	matRotNew *= XMMatrixRotationY(-angleY);
	// 累積の回転行列を合成
	// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
	// クォータニオンを使用する方が望ましい
	matRot = matRotNew * matRot;

}

void PlayerFbx::MoveMatrixUpdate(XMMATRIX matRot,XMMATRIX matTrans)
{
	float dx = 0;
	float dz = 0;
	float dy = 0;

//	wark = false;
	// WASDが押されていたら並行移動させる
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		if (onGround == true&&WallCollision==false)
		{
			wark = true;
			if (input->PushKey(DIK_A))
			{
				dx -= 0.3f;
			}

			if (input->PushKey(DIK_D))
			{
				dx += 0.3f;
				
			}

			if (input->PushKey(DIK_W))
			{
				dz += 0.3f;
			
			}

			if (input->PushKey(DIK_S))
			{
				dz -= 0.3f;
			}
		}
			
	}
	else
	{
		wark = false;
	}

	//moveCamera.m128_f32[0] = 0;
	//moveCamera.m128_f32[2] = 0;
	

	moveCamera = { dx += fallV.m128_f32[0], dy += fallV.m128_f32[1], dz += fallV.m128_f32[2], 0 };

	if (WallCollision==false)
	{
	//	moveCamera = { dx += fallV.m128_f32[0], dy += fallV.m128_f32[1], dz += fallV.m128_f32[2], 0 };
	//	moveOld_ = moveOld;

	//	ColOld = false;
	//	ColOld2 = false;
	}
	else
	{
		
		//if (moveOld_.m128_f32[0] != 0.00f || moveOld_.m128_f32[2] != 0.00f)
		//{
			//if (ColOld == false)
			//{
			//	moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
			//	moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);


			//}
	//	}
		
		/*

		if (ColOld == true)
		{
			
		//	moveCamera.m128_f32[0] = -1.0f * (moveOld.m128_f32[0]);
		//	moveCamera.m128_f32[2] = -1.0f * (moveOld.m128_f32[2]);
			if (moveOld.m128_f32[3] > 0&&ColOld2==false)
			{
				moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
				moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);
			}
			else if(moveOld.m128_f32[3] <= 0 && ColOld2 == false)
			{
				moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
				moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);
			}
			ColOld2 = true;

		}
		
		if (moveOld.m128_f32[0] == 0.00f || moveOld.m128_f32[2] == 0.00f)
		{
			if (moveOld_.m128_f32[0] <= 0)
			{
				moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
				moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);
			}
			else
			{
				if (moveOld_.m128_f32[2] < 0)
				{
					moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
					moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);

					if (ColisionPoint.m128_f32[2]>position.z)
					{
						moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
						moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);
					}

					if (ColisionPoint.m128_f32[0] < position.x)
					{
					
						moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
						moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);

						if (moveOld.m128_f32[2] < 0)
						{
							moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
							moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);
						}
					}
				}
				
			}
		}
		*/
		ColOld=true;
		//WallCollision = false;
	}

	
	//moveCamera = XMVector3Transform(moveCamera, matRot);
}

void PlayerFbx::CollisionAfter()
{
	WallCollision = false;
}


