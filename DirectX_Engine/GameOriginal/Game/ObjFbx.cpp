#include "ObjFbx.h"
#include "../../Engine/Collision/CollisionManager.h"
#include "../../Engine/Collision/BaseCollider.h"
#include "../../Engine/Collision/CollisionAttribute.h"
#include <d3dcompiler.h>
#include "../../Engine/Collision/Collision.h"


#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;




ObjFbx::ObjFbx(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void ObjFbx::OnCollision(const CollisionInfo& info)
{
	
}

void ObjFbx::ObjInitialize()
{
	//変数の初期化
	cursorOn = false;

	cursorOn2 = false;

	Shot = false;

	Shot2 = false;

	Recoile = false;

	Recoile2 = false;

	warkmove = true;

	Wark_time = 0;

	onGround = true;
}

void ObjFbx::ObjUpdate(float angleX, float angleY)
{
	//FBX独自の更新
	oldangleY += angleY;
	oldangleX += angleX;

	XMMATRIX matScale, matRot, matTrans{};
	//Matrixの変化
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	
	//注視点と自分の位置のベクトル計算
	Vector.m128_f32[0] = Target.x- MyPosition.x;
	Vector.m128_f32[1] = Target.y- MyPosition.y;
	Vector.m128_f32[2] = Target.z+2 - MyPosition.z+2;
	//ベクトルの正規化
	Vector = XMVector3Normalize(Vector) * 2.0f;
	//レイのチェック
	RayCheck();
	//リコイルフラグがtrueならリコイル動作入れる
	if (Recoile == true)
	{
		//フラグ切り替えで動く方向変える
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

		//銃身があがり切ったら切り替える
		if (AddRotate.x < -28)
		{
			Recoile2 = true;
		}
	}

	//フラグが立ったら持ち上げて追従する
	if (cursorOn == true)
	{
		//追加の回転行列処理
		AddRotateMatrixUpdate(angleX, angleY, matRot);
		//位置を少しずらす用
		move = { 2 + AddPosition.x,AddPosition.y,AddPosition.z,0 };
		//ポジションのベクトル
		position.x = (MyPosition.x + +Vector.m128_f32[0] * 3);
		position.y = (MyPosition.y + Vector.m128_f32[1] * 3) - 2;
		position.z = (MyPosition.z + Vector.m128_f32[2] * 3);
		//どちらか打ったのならリコイルオン
		if (Shot == true || Shot2 == true)
		{
			Recoile = true;
		}
		//歩いているのなら歩いてるモーションをする
		if (wark == true)
		{
			//下記歩いてるときのモーション動かし
			if (warkmove == true && Wark_time < 60)
			{
				Wark_time += 1;
				AddPosition.y += 0.007;
				AddPosition.x += 0.012;

				if (Wark_time >= 60)
				{
					Wark_time = 0;
					warkmove = false;
				}
			}
			else if (warkmove == false && Wark_time < 60)
			{
				Wark_time += 1;
				AddPosition.y -= 0.007;
				AddPosition.x -= 0.012;


				if (Wark_time >= 60)
				{
					Wark_time = 0;
					warkmove = true;
				}
			}
		}
		else
		{
			//動いてないのなら位置を戻す
			AddPosition.y = 0;
			AddPosition.x = 0;
		}

		//atanで銃身を向ける方向を算出
		rotation.y = atan2(Vector.m128_f32[0], Vector.m128_f32[2]) * 60;

		//注視点が後ろに行ったら反転する
		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}
		//平行移動
		move = XMVector3Transform(move, matRot);
		matTrans = XMMatrixTranslation(position.x += move.m128_f32[0], position.y += move.m128_f32[1], position.z += move.m128_f32[2]);
        //向ける方向調整
		rotation.x = (-atan2(Vector.m128_f32[1], Vector.m128_f32[2]) * 60) + AddRotate.x;
		//フラグ切り替えで動く方向変える
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


	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();

	//パッドの更新
	GP->Update();


	//レイが当たったのなら取得する
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit,20.0f)&&(input->TriggerKey(DIK_F)|| (GP->iPad_B == 1 && Old_iPad_B == 0)) && !cursorOn&&Tutorial == false) {
		cursorOn = true;
		Tutorial_gun = true;
		rotation.x = 0;
		rotation.z = 0;
		rotation.y = 180;
	}

	//トリガー処理のための記憶
	Old_iPad_left = GP->iPad_left, Old_iPad_right = GP->iPad_right, iOld_Pad_up = GP->iPad_up, Old_iPad_down = GP->iPad_down;
	Old_iPad_leftshoulder = GP->iPad_leftshoulder, Old_iPad_rightshoulder = GP->iPad_rightshoulder;
	Old_iPad_A = GP->iPad_A, Old_iPad_B = GP->iPad_B, Old_iPad_X = GP->iPad_X, Old_iPad_Y = GP->iPad_Y;

}

void ObjFbx::BoxObjUpdate(float angleX, float angleY)
{
	//FBX独自の更新
	oldangleY += angleY;
	oldangleX += angleX;
	//matrix生成
	XMMATRIX matScale, matRot, matTrans{};

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//注視点とposのベクトルの計算
	Vector.m128_f32[0] = Target.x - MyPosition.x;
	Vector.m128_f32[1] = Target.y - MyPosition.y;
	Vector.m128_f32[2] = Target.z - MyPosition.z;

	Vector = XMVector3Normalize(Vector) * 2.0f;
	//レイのチェック
	BoxRayCheck();
	//重力処理
	//ObjFall();
	//平行移動
	matTrans = XMMatrixTranslation(position.x , position.y += fallV.m128_f32[1], position.z);

	//取得しているのなら追加の回転行列
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
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;
	ray.dir = CammeraZAxis;


	//パッドのポインタ
	GamePad* GP = nullptr;
	GP = new GamePad();

	//パッドの更新
	GP->Update();

	//レイが当たっているのなら取得
	if (CollisionManager::GetInstance()->Raycast(ray,COLLISION_ATTR_OBJ2, &raycastHit, 35.0f) && (input->TriggerKey(DIK_F) || (GP->iPad_B == 1 && Old_iPad_B == 0)) && !cursorOn2&&Tutorial==false) {
		cursorOn2 = true;
	}
	else if ((input->TriggerKey(DIK_F)||(GP->iPad_B == 1 && Old_iPad_B == 0)) && cursorOn2 == true)
	{
		//離す処理
		cursorOn2 = false;
	}

	//持ってるときは少し奥で追従させる
	if (cursorOn2 == true)
	{
		move = { 2,0,0,0 };
		//少し奥で持たせる
		position.x = (MyPosition.x + Vector.m128_f32[0] * 8);
		position.y = (MyPosition.y + Vector.m128_f32[1] * 8);
		position.z = (MyPosition.z + Vector.m128_f32[2] * 8);

		//反転したらを戻す
		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}
	}

	//トリガー処理のための記憶
	Old_iPad_left = GP->iPad_left, Old_iPad_right = GP->iPad_right, iOld_Pad_up = GP->iPad_up, Old_iPad_down = GP->iPad_down;
	Old_iPad_leftshoulder = GP->iPad_leftshoulder, Old_iPad_rightshoulder = GP->iPad_rightshoulder;
	Old_iPad_A = GP->iPad_A, Old_iPad_B = GP->iPad_B, Old_iPad_X = GP->iPad_X, Old_iPad_Y = GP->iPad_Y;
}

void ObjFbx::TurettUpdate()
{
	RazerRayCheck();

	BoxRayCheck();

	//FBXの更新
	Update();

}

void ObjFbx::RazerRayCheck()
{
	// レイのスタート地点を設定
	Ray ray;
	ray.start = { position.x,position.y,position.z,0 };
	ray.start.m128_f32[1] += 2.5f;
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;

	XMMATRIX matRot;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	ray.dir = XMVector3Transform(ray.dir, matRot);

	//レイが当たったのなら取得する
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PLAYER, &raycastHit, 20.0f)) {

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
	Sphere sphere;
    sphere.center = { MyPosition.x,MyPosition.y,MyPosition.z,0 };
	sphere.redius = 5.0f;
	RaycastHit raycastHit;

	if (CollisionManager::GetInstance()->Spherecast(sphere, COLLISION_ATTR_WALL, &raycastHit, 35.0f)) {
		onGround = true;
	}
	else
	{
		onGround = false;
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
