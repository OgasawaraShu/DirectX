#include "PortalExit.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"
#include "Collision.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;


PortalExit::PortalExit(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void PortalExit::ExitUpdate(float angleX, float angleY)
{
	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//平行移動
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//OBj独自の関数
	OriginalUpdate(angleX, angleY);

	//追加回転行列
	AddMatrixUpdate(angleX, angleY, matRot);

	//Matrix後更新
	PostMatrixUpdate(matScale, matRot, matTrans);
}

void PortalExit::OriginalUpdate(float angleX, float angleY)
{
	OldAngle.x += angleX;
	OldAngle.y += angleY;

	scale = ScaleExit;

	if (WarpFlag == true)
	{
		if (ScaleExit.x < 0.07)
		{
			ScaleExit.x += 0.001;
			ScaleExit.y += 0.001;
			ScaleExit.z += 0.001;
	   }
	}
	else
	{
		ScaleExit.x = 0;
		ScaleExit.y = 0;
		ScaleExit.z = 0;
	}
}

void PortalExit::AddMatrixUpdate(float angleX, float angleY, XMMATRIX matRot)
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

void PortalExit::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
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
