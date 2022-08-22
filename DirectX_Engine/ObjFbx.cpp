#include "ObjFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"

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

void ObjFbx::ObjUpdate(float angleX, float angleY)
{
	//FBX独自の更新
	
	XMMATRIX matScale, matRot, matTrans{};

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//追加の回転行列処理
	AddRotateMatrixUpdate(angleX, angleY, matRot);
	
	RayCheck();

	if (cursorOn == true)
	{
		position.x = MyPosition.x;
		position.y = MyPosition.y;
		position.z = MyPosition.z + 60;
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

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit)&&input->TriggerMouseMid()&&!cursorOn) {
		cursorOn = true;
	}
	else if(input->TriggerMouseMid() &&cursorOn==true)
	{
		cursorOn = false;
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
	// 追加回転分の回転行列を生成
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX);
	matRotNew *= XMMatrixRotationY(-angleY);
	// 累積の回転行列を合成
	// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
	// クォータニオンを使用する方が望ましい
	matRot = matRotNew * matRot;
}
