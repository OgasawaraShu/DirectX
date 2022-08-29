#include "DebugCamera.h"
#include "Fbx3d.h"
#include <cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height, Input* input)
	: Camera(window_width, window_height)
{
	assert(input);

	this->input = input;
	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void DebugCamera::Update()
{
	bool dirty = false;
	float target_x = 0;

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	GamePad* GP = nullptr;
	GP = new GamePad();

	//パッドの更新
	GP->Update();

	float dy = mouseMove.lX * scaleY;
	float dx = mouseMove.lY * scaleX;

	angleX = -dx * XM_PI;
	angleY = -dy * XM_PI;

	dirty = true;

	oldx += angleX;
	oldy += angleY;

	//カメラのX軸ベクトルが後ろに行きそうなら押し戻す
	if (oldx > 1.00)
	{
		angleX -= 0.02;
		oldx -= 0.02;
	}
	else if (oldx < -1.00)
	{
		angleX += 0.02;
		oldx += 0.02;
	}

	

	//ゲームパッドアナログスティックR入力時処理(視点移動)
	if (GP->state.Gamepad.sThumbRX != 0 || GP->state.Gamepad.sThumbRY != 0)
	{
		float dy = static_cast<FLOAT>(GP->state.Gamepad.sThumbRX / 32767.0 * (0.02f));
		float dx = static_cast<FLOAT>(GP->state.Gamepad.sThumbRY / 32767.0 * (0.02f));

		angleX = dx * XM_PI;
		angleY = -dy * XM_PI;
		dirty = true;
	}

	//XMVECTOR move = move_;

	//move = XMVector3Transform(move, matRot);

	//MoveVectorNotY(move);
	
	//onGroundがfalseならY軸も参照したvector移動する
	if (onGround_ != true)
	{
		XMVECTOR move = move_;
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	SetEye(eye_);
	
	//ゲームパッドアナログスティックL入力時処理(場所移動)
	/*
	if (GP->state.Gamepad.sThumbLX != 0 || GP->state.Gamepad.sThumbLY != 0)
	{
		float dx = static_cast<FLOAT>(GP->state.Gamepad.sThumbLX / 32767.0 * (1.0f));
		float dz = static_cast<FLOAT>(GP->state.Gamepad.sThumbLY / 32767.0 * (1.0f));


		XMVECTOR move = { dx, 0, dz, 0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}
	*/
	//テレポートしたら座標を合わせる
	if (blueTeleport == true || redTeleport == true)
	{
		SetEye(Warp_);
	}

	//テレポートしたら角度を球の逆にする
	if (redTeleport == true)
	{
		angleX = angle_RedX;
		angleY = angle_RedY-Ras;

		dirty = true;
	}

	//テレポートしたら角度を球の逆にする
	if (blueTeleport == true)
	{
		angleX = angle_BlueX;
		angleY = angle_BlueY - Ras;

		dirty = true;
	}

	if (dirty || viewDirty) {
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
	//	matRotNew *= XMMatrixRotationZ(0);
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル  視点
		//XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		//vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 注視点からずらした位置に視点座標を決定
		//const XMFLOAT3& target = GetTarget();
		const XMFLOAT3& target = GetEye();


		//SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	
		
		SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	Camera::Update();
}

float DebugCamera::GetAngleX()
{
	return angleX;
}

float DebugCamera::GetAngleY()
{
	return angleY;
}

float DebugCamera::GetPositionX()
{
	return oldx;
}

float DebugCamera::GetPositionY()
{
	return eye.z;
}


