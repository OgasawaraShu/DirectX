#include "BulletFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void BulletFbx::OnCollision(const CollisionInfo& info)
{
	if (debug == 0 && TriggerFlag == 1)debug = 1;
	if (debug2 == 0 && TriggerFlag2 == 1)debug2 = 1;
}

BulletFbx::BulletFbx(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void BulletFbx::BlueBulletUpdate(double angleX, double angleY)
{
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();
	//パッドの更新
	GP->Update();


	//角度のフラグ
	bool dirty = false;

	//マウス角度出力Ver
	if (TriggerFlag == 0)
	{
		//マウスの左が押されない限りカメラの角度を覚え続ける
		angleX1 = angleX;
		angleY1 = angleY;
	}
	else
	{
		//押されたらフラグをtrueにし覚えるのをやめる
		dirty = true;
	}

	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//falseの間は角度を変え続ける
	if (dirty == false)
	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX1);
		matRotNew *= XMMatrixRotationY(-angleY1);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;



		//ベクトルと行列の積

		move = XMVector3Transform(move, matRot);
	}
	

	XMVECTOR moveCamera = move_;
	moveCamera = XMVector3Transform(moveCamera, matRot);


	if (input->TriggerMouseLeft())
	{
		TriggerFlag = 1;
	}


	if (TriggerFlag == 1 && debug == 0)
	{
		//平行移動(左が押されたら球がカメラの方向に合わせて前に出ていく)
		matTrans = XMMatrixTranslation(position.x += move.m128_f32[0], position.y += move.m128_f32[1], position.z += move.m128_f32[2]);
	}
	else if (TriggerFlag == 0 && debug == 0)
	{
		//平行移動
		matTrans = XMMatrixTranslation(position.x += moveCamera.m128_f32[0], position.y += moveCamera.m128_f32[1], position.z += moveCamera.m128_f32[2]);
	}
	else
	{
		//平行移動
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}

	memory.m128_f32[0] += move.m128_f32[0];
	memory.m128_f32[1] += move.m128_f32[1];
	memory.m128_f32[2] += move.m128_f32[2];


	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//カメラの行列をかける
	if (TriggerFlag == 0 && debug == 0)
	{
		matWorld *= matrot;
	}

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

void BulletFbx::RedBulletUpdate(double angleX, double angleY)
{
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();
	//パッドの更新
	GP->Update();


	//角度のフラグ
	bool dirty = false;

	//マウス角度出力Ver
	if (TriggerFlag2 == 0)
	{
		//マウスの左が押されない限りカメラの角度を覚え続ける
		angleX2 = angleX;
		angleY2 = angleY;


	}
	else
	{
		//押されたらフラグをtrueにし覚えるのをやめる
		dirty = true;
	}

	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//falseの間は角度を変え続ける
	if (dirty == false)
	{
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX2);
		matRotNew *= XMMatrixRotationY(-angleY2);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;



		//ベクトルと行列の積

		move2 = XMVector3Transform(move2, matRot);
	}
	

	XMVECTOR moveCamera = move_;
	moveCamera = XMVector3Transform(moveCamera, matRot);


	if (input->TriggerMouseRight())
	{
		TriggerFlag2 = 1;
	}


	if (TriggerFlag2 == 1 && debug2 == 0)
	{
		//平行移動(左が押されたら球がカメラの方向に合わせて前に出ていく)
		matTrans = XMMatrixTranslation(position.x += move2.m128_f32[0], position.y += move2.m128_f32[1], position.z += move2.m128_f32[2]);
	}
	else if (TriggerFlag2 == 0 && debug2 == 0)
	{
		//平行移動
		matTrans = XMMatrixTranslation(position.x +=moveCamera.m128_f32[0], position.y += moveCamera.m128_f32[1], position.z += moveCamera.m128_f32[2]);
	}
	else
	{
		//平行移動
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}

	memory3.m128_f32[0] = position.x;
	memory3.m128_f32[1] = position.y;
	memory3.m128_f32[2] = position.z;


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
