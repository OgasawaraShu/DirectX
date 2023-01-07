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
void DebugCamera::Initialize2()
{
 camera_posX = 0;
	 camera_posZ = 0;

	 Colision = false;

	 angleX = 0;//カメラの角度X
	 angleY = 0;//カメラの角度Y
	 oldx = 0;
	 oldy = 0;

	 Flag = 0;
	 s = 0;
	 t = 0;

 AngleRimit = false;

	 scene = 0;

	 IventNumber = 0;

	 IventTime = 0;

	 trun = false;
	 time2 = 0;
	 time3 = 0;
	 SetTarget({ eye.x, eye.y, eye.z + 20 });
}
#pragma optimize("", off)
void DebugCamera::Update()
{
	//bool dirty = false;
	if (scene == 1)
	{

		
		MainSceneUpdate();

		if (time2 == 0)
		{
			eye = { 0,0,-20 };
			angleX = 0.2;
			angleY = -(oldy);

			oldx = 0;
			oldy = 0;


			time2 += 1;

			SetEye(eye);

		}

		//IventMainUpdate();


	}
	else if (scene == 0)
	{
		TitleSceneUpdate();
	}
	else if (scene == 99)
	{
		LoadUpdate();	

		SetTarget({ eye.x, eye.y, eye.z + 20 });
		SetUp({ 0, 1,0});
	}


	if (viewDirty) {
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
 
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい



		matRot = matRotNew * matRot;

		FXMVECTOR Quatenion=XMQuaternionRotationMatrix(matRot);

		//テレポートしたら角度を球の逆にする
		if (redTeleport == true)
		{
			matRot = RotRed;
			RotRedFall = matRot;
			XMVECTOR movewarp;
			movewarp = { 0,0,2,0 };
			movewarp = XMVector3Transform(movewarp, matRot);
			MoveVectorNotY(movewarp);
			redfall = true;
			
		}

		//テレポートしたら角度を球の逆にする
		if (blueTeleport == true)
		{
			matRot = RotBlue;
			RotBlueFall = matRot;

			XMVECTOR movewarp;
			movewarp = { 0,0,2,0 };
			movewarp = XMVector3Transform(movewarp, matRot);
			MoveVectorNotY(movewarp);
			bluefall = true;
		
		}

		if (onGround_ != true && redfall == true)
		{
			XMVECTOR move = move_;
			
			move = XMVector3Transform(move, RotRedFall);
			MoveVector(move);
		}
		else if (onGround_ == true&&redfall==true)
		{
			redfall = false;
		}


		if (onGround_ != true && bluefall == true)
		{
			XMVECTOR move = move_;
			move = XMVector3Transform(move, RotBlueFall);
			MoveVector(move);
		}
		else if (onGround_ == true && bluefall == true)
		{
			bluefall = false;
		}



		// 注視点から視点へのベクトルと、上方向ベクトル  視点
		//XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		//vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
		vUp = XMVector3Transform(vUp, matRot);

	
		
		
		vect = vEyeTarget;
		// 注視点からずらした位置に視点座標を決定
		//const XMFLOAT3& target = GetTarget();
		const XMFLOAT3& target = GetEye();


		//SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	
		if (scene != 99)
		{
			if (vEyeTarget.m128_f32[1]<19 && vEyeTarget.m128_f32[1]>-19) {
				SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });
			}
		
			//SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
		}
	}

	if(Colision==false)oldeye = eye;


	//up.x = 0;
	//up.y = 1;
	//up.z = 0;

	Camera::Update();
}

volatile void DebugCamera::MainSceneUpdate()
{
	oldeye = eye;



	float target_x = 0;

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	GamePad* GP = nullptr;
	GP = new GamePad();

	//パッドの更新
	GP->Update();


	// マウスの左ボタンが押されていたらカメラを回転させる
	if (input->PushMouseLeft())
	{

		float dy = mouseMove.lX * scaleY;
		float dx = mouseMove.lY * scaleX;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;
	}
	else
	{
		angleX = 0;
		angleY = 0;
	}
	oldx += angleX;
	oldy += angleY;

	//ゲームパッドアナログスティックR入力時処理(視点移動)
	if (GP->state.Gamepad.sThumbRX != 0 || GP->state.Gamepad.sThumbRY != 0)
	{
		float dy = static_cast<FLOAT>(GP->state.Gamepad.sThumbRX / 32767.0 * (0.02f));
		float dx = static_cast<FLOAT>(GP->state.Gamepad.sThumbRY / 32767.0 * (0.02f));

		angleX = dx * XM_PI;
		angleY = -dy * XM_PI;
		//dirty = true;
	}
	move = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	move = move_;

	if (Colision == false)
	{
		OldmatRot = matRot;
		move = XMVector3Transform(move, matRot);
		oldeye = eye;
	}
	else if(Colision==true&&onGround_==true)
	{

		move = XMVector3Transform(move, OldmatRot);

		XMVector3Normalize(ColisionVec);

	
		if (move.m128_f32[0] > 0 && move.m128_f32[2] > 0)
		{
			s = 1;
		}
		else if (move.m128_f32[0] < 0 && move.m128_f32[2] > 0)
		{
			s = 1;
		}
		else if (move.m128_f32[0] > 0 && move.m128_f32[2] < 0)
		{
			s = 1;
		}
		else
		{
			s = 2;
		}
			
		if (move.m128_f32[0] == 0 && move.m128_f32[2] ==0)
		{
			s = 3;
		}


		if (s == 1) {
			move = (XMVector3Normalize(ColisionVec - XMVector3Dot(ColisionVec, move) * move));
		}
		else if (s == 2) {
			move = -(XMVector3Normalize(ColisionVec - XMVector3Dot(ColisionVec, move) * move));
		}
		else if (s == 3) {
			move = -(XMVector3Normalize(ColisionVec - XMVector3Dot(ColisionVec, move) * move));
		}

		move.m128_f32[1] = 0;

	//D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal_n) * normal_n));
	}

	if (Colision == false)
		MoveVectorNotY(move);
	else
		MoveVectorNotY2(move);
	moveOld = -1*move;
	moveOld2 =move;

	//onGroundがfalseならY軸も参照したvector移動する
	if (onGround_ != true)
	{
		XMVECTOR move = move_;
	//	move = XMVector3Transform(move, matRot);

		MoveVector(move);
		//dirty = true;
	}

	
	//SetEye(eye_);

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

	
}
#pragma optimize("", on)
void DebugCamera::TitleSceneUpdate()
{

	
	eye = { -80,60,-60 };
	
	SetEye(eye);
	
	oldx += angleX;
	oldy += angleY;

	//angleX=0.1

	if (time3 == 0)
	{
		angleX = -0.2;
		time3 += 1;
	}
	else
	{
		angleX = 0;
	}

	if (trun == false)
	{
		angleY = -0.001;
	}
	else
	{
		angleY = 0.001;
	}



	if (oldy < -1)
	{
		trun = true;
	}
	else if(oldy > 0.01)
	{
		trun = false;
	}
	
}

void DebugCamera::IventMainUpdate()
{

	IventTime += 1;
	eye = { 0,-7,-20 };
	if (IventNumber == 0)
	{
		trun = false;
		eye = { 0,-7,-20 };
		if (IventTime > 180)
		{
			IventNumber += 1;
			IventTime = 0;
		}
	}
	else if (IventNumber == 1)
	{
		if (trun == false)
		{
			angleY += 0.005;
			if (oldy > 1)
			{
				trun = true;
			}
		}
		else
		{
			angleY -= 0.005;
		}
	}
	oldx += angleX;
	oldy += angleY;


}

void DebugCamera::MapEditUpdate()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	// マウスの左ボタンが押されていたらカメラを回転させる
	if (input->PushMouseLeft())
	{
		float dy = mouseMove.lX * scaleY;
		float dx = mouseMove.lY * scaleX;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;
		dirty = true;
	}

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (input->PushMouseMid())
	{
		float dx = mouseMove.lX / 10.0f;
		float dy = mouseMove.lY / 10.0f;

		XMVECTOR move = { -dx, +dy, 0, 0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	// ホイール入力で距離を変更
	if (mouseMove.lZ != 0) {
		distance -= mouseMove.lZ / 20.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty || viewDirty) {
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 注視点からずらした位置に視点座標を決定
		const XMFLOAT3& target = GetTarget();
		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	Camera::Update();
}

void DebugCamera::LoadUpdate()
{
	
	if (IventTime == 0)
	{
		eye = { 300,0,0 };
		move = { 0,0,0 };
		SetEye(eye);
		
	}
	else if (IventTime >= 40)
	{
		if (IventTime >= 40&&IventTime<140)
		{
			eye.z += 0.3;
			SetEye(eye);

		}
		
	}

	IventTime++;

}

void DebugCamera::ColisionAfterCameraSet(XMFLOAT3 Pos)
{
	SetEye(Pos);
	

	if (viewDirty) {
		

		// 注視点から視点へのベクトルと、上方向ベクトル  視点
		//XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		//vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
		//vUp = XMVector3Transform(vUp, matRot);

		// 注視点からずらした位置に視点座標を決定
		//const XMFLOAT3& target = GetTarget();
		const XMFLOAT3& target = GetEye();


		//SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });

		

		if (scene != 99)
		{
			/*
			if (vEyeTarget.m128_f32[1]<19 && vEyeTarget.m128_f32[1]>-19) {
				PitchFlag=false;
				PitchRot = matRot;
			}
			else
			{
				PitchFlag = true;
			}

			if (PitchFlag == true)
			{
				matRot = PitchRot;
				vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
			}
			*/

		
				SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });
			
			//SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });
		//	SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		}
	}

	if (Colision == false)oldeye = eye;
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
	return matRot.r[1].m128_f32[0];
}

float DebugCamera::GetPositionY()
{
	return eye.z;
}


