#include "BulletFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void BulletFbx::Initialize2()
{
	 TriggerRe = 0;

	 reset_Blue = true;//球の座標リセット

	 reset_Red = true;//球の座標リセット

	 warpFlag = false;//両方の球が撃たれたらワープできるようにする

	 warpFlag2 = false;//両方の球が撃たれたらワープできるようにする

	
	 RedCollision = false;

	 RedAttack = false;

	 BlueCollision = false;

	 ShotFlag = false;
	 ShotFlag2 = false;


	BlueAttack = false;

	position = position_;
	warpFlag = false;
	TriggerFlag = 0;
	debug = 0;
	BlueAttack = false;
	BlueCollision = false;

	position = position_;
	warpFlag2 = false;
	TriggerFlag2 = 0;
	debug2 = 0;

	oldRedX = oldx2;
	oldRedY = oldy2;
	RedCollision = false;
	RedAttack = false;
}

void BulletFbx::OnCollision(const CollisionInfo& info)
{
	if (info.collider->attribute != 2&& info.collider->attribute != 4&& info.collider->attribute != 8&& info.collider->attribute != 16)
	{
		if (debug == 0 && TriggerFlag == 1)
		{
			if (info.collider->attribute != 64)
			{
				//TriggerFlag = 0;
				debug = 1;
				BlueCollision = true;
				BlueAttack = false;
			}
			else
			{
				position = position_;
				warpFlag = false;
				TriggerFlag = 0;
				debug = 0;

			}
		}
		if (debug2 == 0 && TriggerFlag2 == 1)
		{
		//	TriggerFlag2 = 0;
			//debug2 = 1;
			if (info.collider->attribute != 64)
			{
				//TriggerFlag = 0;
				debug2 = 1;
				RedCollision = true;
				RedAttack = false;
			}
			else
			{
				position = position_;
				warpFlag2 = false;
				TriggerFlag2 = 0;
				debug2 = 0;

				oldRedX = oldx2;
				oldRedY = oldy2;
			}
		}
	}
}

BulletFbx::BulletFbx(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void BulletFbx::BlueBulletUpdate(float angleX, float angleY)
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
	

		Target_ = Target;
	}
	else
	{
	
		//押されたらフラグをtrueにし覚えるのをやめる
		dirty = true;
	}

	//マウスの左が押されない限りカメラの角度を覚え続ける
	angleX1 = angleX;
	angleY1 = angleY;

	oldBlueX += angleX1;
	oldBlueY += angleY1;

	if (ShotFlag2 == true)
	{
		memoB.x = oldBlueX;
		memoB.y = oldBlueY;
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
	

	//両方の球が撃たれたらワープできるフラグをTRUEにする
	if (debug == 1)
	{
		warpFlag = true;
	}

	XMVECTOR moveCamera = move_;
	moveCamera = XMVector3Transform(moveCamera, matRot);


	if (input->TriggerMouseLeft()&&getflag==true)
	{
		TriggerFlag = 1;
		BlueAttack = true;
		ShotFlag2 = true;
	}
	else
	{
		ShotFlag2 = false;
	}


	if (input->PushKey(DIK_R) && debug == 1)
	{
		position = position_;
		warpFlag = false;
		TriggerFlag = 0;
		debug = 0;
		BlueAttack = false;
		BlueCollision = false;
	}



	if (TriggerFlag == 1 && debug == 0)
	{
		//平行移動(左が押されたら球がカメラの方向に合わせて前に出ていく)
		//matTrans = XMMatrixTranslation(position.x += move.m128_f32[0], position.y += move.m128_f32[1], position.z += move.m128_f32[2]);
		matTrans = XMMatrixTranslation(position.x += Vector.m128_f32[0], position.y += Vector.m128_f32[1], position.z += Vector.m128_f32[2]);
	}
	else if (TriggerFlag == 0 && debug == 0)
	{
		position = position_;
		Vector.m128_f32[0] = Target.x - position.x;
		Vector.m128_f32[1] = Target.y - position.y;
		Vector.m128_f32[2] = Target.z - position.z;

		Vector = XMVector3Normalize(Vector) * 4.0f;
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}
	else if(debug==1)
	{
		//平行移動
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
		TriggerFlag = 0;
	}

	memory.m128_f32[0] = position.x;
	memory.m128_f32[1] = position.y;
	memory.m128_f32[2] = position.z;

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//カメラの行列をかける
	if (TriggerFlag == 0 && debug == 0)
	{
		matWorld *= matrot;
	}

	PostMatrixUpdate();
}

void BulletFbx::RedBulletUpdate(float angleX, float angleY)
{
	//RedCollision = false;
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();
	//パッドの更新
	GP->Update();

	//角度のフラグ
	bool dirty = false;

	
	//マウスの左が押されない限りカメラの角度を覚え続ける
	
	//マウス角度出力Ver
	if (TriggerFlag2 == 0)
	{
		

		Target_ = Target;
	}
	else
	{
		//押されたらフラグをtrueにし覚えるのをやめる
		dirty = true;
	}

	angleX2 = angleX;
	angleY2 = angleY;

	oldRedX += angleX2;
	oldRedY += angleY2;

	if (ShotFlag == true)
	{
		memoR.x = oldRedX;
		memoR.y = oldRedY;
	}

	oldx2 += angleX;
	oldy2 += angleY;

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
	
	if (debug2 == 1)
	{
		warpFlag2 = true;
	}
	XMVECTOR moveCamera = move_;
	moveCamera = XMVector3Transform(moveCamera, matRot);



	if (input->TriggerMouseRight() && getflag == true)
	{
		TriggerFlag2 = 1;
		RedAttack = true;
		ShotFlag = true;
	}
	else
	{
		ShotFlag = false;
	}

	if (input->PushKey(DIK_R) && debug2 == 1)
	{
		position = position_;
		warpFlag2 = false;
		TriggerFlag2 = 0;
		debug2 = 0;

		oldRedX = oldx2;
		oldRedY = oldy2;
		RedCollision = false;
		RedAttack = false;
	}



	if (TriggerFlag2 == 1 && debug2 == 0)
	{
		matTrans = XMMatrixTranslation(position.x += Vector.m128_f32[0], position.y += Vector.m128_f32[1], position.z += Vector.m128_f32[2]);
	}
	else if (TriggerFlag2 == 0 && debug2 == 0)
	{
		position = position_;
		Vector.m128_f32[0] = Target.x - position.x;
		Vector.m128_f32[1] = Target.y - position.y;
		Vector.m128_f32[2] = Target.z - position.z;
		
		Vector = XMVector3Normalize(Vector) * 2.0f;

		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
		//平行移動
		//matTrans = XMMatrixTranslation(position.x +=moveCamera.m128_f32[0], position.y += moveCamera.m128_f32[1], position.z += moveCamera.m128_f32[2]);
	}
	else if(debug2==1)
	{
		//平行移動
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
		TriggerFlag2 = 0;
	}

	memory3.m128_f32[0] = position.x;
	memory3.m128_f32[1] = position.y;
	memory3.m128_f32[2] = position.z;

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//カメラの行列をかける
	if (TriggerFlag2 == 0 && debug2 == 0)
	{
		matWorld *= matrot;
	}

	PostMatrixUpdate();
}

void BulletFbx::PostMatrixUpdate()
{
	
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
