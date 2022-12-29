#include "ObjFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"
#include "Collision.h"


#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;




ObjFbx::ObjFbx(Input* input, Physics* physics)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
	this->physics = physics;
}

void ObjFbx::Initialize2()
{
	//変数
	cursorOn = false;
	cursorOn2 = false;

	Shot = false;
	Shot2 = false;
	Recoile = false;
	Recoile2 = false;

	warkmove = true;
	warktime = 0;
	onGround = true;//地面の判定
}

void ObjFbx::ObjUpdate(float angleX, float angleY)
{
	//FBX独自の更新
	oldangleY += angleY;
	oldangleX += angleX;

	XMMATRIX matScale, matRot, matTrans{};

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	


	Vector.m128_f32[0] = Target.x- MyPosition.x;
	Vector.m128_f32[1] = Target.y- MyPosition.y;
	Vector.m128_f32[2] = Target.z+2 - MyPosition.z+2;

	Vector = XMVector3Normalize(Vector) * 2.0f;

	RayCheck();


	if (Recoile == true)
	{

		if (Recoile2 == false)
		{
			AddRotate.x -= 6;
			AddPosition.z = -0.5;
		}
		else
		{
			AddRotate.x += 6;
			AddPosition.z = 0.5;
			if (AddRotate.x > 1)
			{
				Recoile = false;
				Recoile2 = false;

				AddPosition.z = 0;
				AddRotate.x = 0;
			}
		}


		if (AddRotate.x < -28)
		{
			Recoile2 = true;
		}
	}




	if (cursorOn == true)
	{
		//追加の回転行列処理
		AddRotateMatrixUpdate(angleX, angleY, matRot);

		move = { 2+AddPosition.x,AddPosition.y,AddPosition.z,0};

		position.x = (MyPosition.x+ + Vector.m128_f32[0] * 3);
		position.y = (MyPosition.y + Vector.m128_f32[1] * 3)-2;
		position.z = (MyPosition.z + Vector.m128_f32[2] * 3);

		if (Shot == true||Shot2==true)
		{
			Recoile = true;
		}
		
		if (wark == true)
		{
			if (warkmove == true&&warktime<60)
			{
				warktime += 1;
				AddPosition.y += 0.007;
				AddPosition.x += 0.012;

				if (warktime >= 60)
				{
					warktime = 0;
					warkmove = false;
				}
			}
			else if (warkmove == false && warktime < 60)
			{
				warktime += 1;
				AddPosition.y -= 0.007;
				AddPosition.x -= 0.012;


				if (warktime >= 60)
				{
					warktime = 0;
					warkmove = true;
				}
			}
		}
		else
		{
			AddPosition.y = 0;
			AddPosition.x = 0;

			//warktime = 0;
		}

		rotation.y = atan2(Vector.m128_f32[0], Vector.m128_f32[2])*60;

		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}

		move = XMVector3Transform(move, matRot);
		matTrans = XMMatrixTranslation(position.x+=move.m128_f32[0], position.y+=move.m128_f32[1], position.z += move.m128_f32[2]);

	
			rotation.x = ( - atan2(Vector.m128_f32[1], Vector.m128_f32[2]) * 60)+AddRotate.x;

			if (Recoile == false)
			{
				if (rotation.x > 90)
				{
					rotation.x = rotation.x - 180;
				}
				else if (rotation.x < -90)
				{
					rotation.x = rotation.x + 180;
				}
			}
		
	}


	//行列後更新
	PostMatrixUpdate(matScale, matRot, matTrans);
}

void ObjFbx::RayCheck()
{
	// レイのスタート地点を設定
	Ray ray;
	ray.start = { MyPosition.x,MyPosition.y,MyPosition.z,0 };
	ray.start.m128_f32[1] += 2.5f;
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;

	ray.dir = CammeraZAxis;

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit,10.0f)&&input->TriggerMouseMid()&&!cursorOn) {
		cursorOn = true;

		rotation.x = 0;
		rotation.z = 0;
		rotation.y = 180;
	}
	else if(input->TriggerMouseMid() &&cursorOn==true)
	{
		//cursorOn = false;
	}
}

void ObjFbx::BoxObjUpdate(float angleX, float angleY)
{
	//FBX独自の更新
	oldangleY += angleY;
	oldangleX += angleX;

	XMMATRIX matScale, matRot, matTrans{};

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));



	Vector.m128_f32[0] = Target.x - MyPosition.x;
	Vector.m128_f32[1] = Target.y - MyPosition.y;
	Vector.m128_f32[2] = Target.z - MyPosition.z;

	Vector = XMVector3Normalize(Vector) * 2.0f;

	BoxRayCheck();

	ObjFall();

	matTrans = XMMatrixTranslation(position.x , position.y += fallV.m128_f32[1], position.z);

	if (cursorOn2 == true)
	{
		//追加の回転行列処理
		AddRotateMatrixUpdate(angleX, angleY, matRot);

	
	}
	//行列後更新
	PostMatrixUpdate(matScale, matRot, matTrans);
}

void ObjFbx::BoxRayCheck()
{
	// レイのスタート地点を設定
	Ray ray;
	ray.start = { MyPosition.x,MyPosition.y,MyPosition.z,0 };
	//ray.start.m128_f32[1] -= 2.5f;
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;

	ray.dir = CammeraZAxis;

	if (CollisionManager::GetInstance()->Raycast(ray,COLLISION_ATTR_OBJ2, &raycastHit, 35.0f) && input->TriggerMouseMid() && !cursorOn2) {
		cursorOn2 = true;
	}
	else if (input->TriggerMouseMid() && cursorOn2 == true)
	{
		cursorOn2 = false;
	}

	if (cursorOn2 == true)
	{
		move = { 2,0,0,0 };

		position.x = (MyPosition.x + Vector.m128_f32[0] * 8);
		position.y = (MyPosition.y + Vector.m128_f32[1] * 8);
		position.z = (MyPosition.z + Vector.m128_f32[2] * 8);

		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}
	}
}

void ObjFbx::ObjFall()
{
	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	}

	// レイのスタート地点を設定
	Ray ray;
	ray.start = { position.x,position.y,position.z,0 };
	ray.start.m128_f32[1] += 5.0f;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;


	Plane plane;

	plane.normal = XMVectorSet(0, 1, 0, 0);
	plane.distance = 0.0f;


	XMVECTOR inter;
	float distance;
	bool hit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);

	if (hit && distance <= 0.5f) {

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
			position.y -= (raycastHit.distance - 3.0 * 2.0f);
		}
		// 地面がないので落下
		else {
			//	onGround = true;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, 5.0 * 2.0f)) {
			// 着地
			onGround = true;
			position.y -= (raycastHit.distance - 3.0 * 2.0f);
		}
	}
}

void ObjFbx::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
{
	//平行移動
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

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

void ObjFbx::AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot)
{
	angleX1 = angleX;
	angleY1 = angleY;

	// 追加回転分の回転行列を生成
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX1);
	matRotNew *= XMMatrixRotationY(-angleY1);
	// 累積の回転行列を合成
	// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
	// クォータニオンを使用する方が望ましい
	matRot = matRotNew * matRot;

    move = XMVector3Transform(move, matRot);
}
