#include "PlayerFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void PlayerFbx::OnCollision(const CollisionInfo& info)
{
	onGround = true;
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
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();
	//重力のポインタ
//	Physics* phy = nullptr;
	//phy = new Physics();
	//パッドの更新
	GP->Update();


	//角度のフラグ
	bool dirty = false;

	//マウス角度出力Ver

	angleX = angleX;
	angleY = angleY;



	//押されたらフラグをtrueにし覚えるのをやめる
	dirty = true;


	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//falseの間は角度を変え続ける
	if (dirty == true)
	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;



		//ベクトルと行列の積

		move2 = XMVector3Transform(move2, matRot);
	}
	float dx1 = 0;
	float dz = 0;
	float dy = 0;
	// WASDが押されていたらカメラを並行移動させる
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{

		if (input->PushKey(DIK_A))
		{
			dx1 -= 0.6f;
		}

		if (input->PushKey(DIK_D))
		{
			dx1 += 0.6f;
		}

		if (input->PushKey(DIK_W))
		{
			dz += 0.6f;
		}

		if (input->PushKey(DIK_S))
		{
			dz -= 0.6f;
		}


	}

	if (input->TriggerKey(DIK_SPACE))
	{
		
		Warp2.x = Warp.m128_f32[0];
		Warp2.y = Warp.m128_f32[1];
		Warp2.z = Warp.m128_f32[2];

		
		position = Warp2;
	}

	dy = fallV.m128_f32[1];
	
	moveCamera = { dx1, dy, dz, 0};

	moveCamera = XMVector3Transform(moveCamera, matRot);
	
	//positionにVectorを足す
	position.x += moveCamera.m128_f32[0];
	position.y += moveCamera.m128_f32[1] + fallV.m128_f32[1];
	position.z += moveCamera.m128_f32[2];

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


