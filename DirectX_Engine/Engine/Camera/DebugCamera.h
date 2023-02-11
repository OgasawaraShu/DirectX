#pragma once
#include "Camera.h"
#include "../Input/Input.h"
#include "../Input/GamePad.h"

/// <summary>
/// デバッグ用カメラ
/// </summary>
class DebugCamera :
	public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <param name="input">入力</param>
	DebugCamera(int window_width, int window_height, Input* input);

	void Initialize2();
	// 更新
	void Update() override;

	volatile void MainSceneUpdate();

	void TitleSceneUpdate();

	void IventMainUpdate();

	void MapEditUpdate();

	void LoadUpdate();

	void ColisionAfterCameraSet(XMFLOAT3 Pos);

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

	float GetAngleX();
	float GetAngleY();

	float GetPositionX();
	float GetPositionY();

	float GetUpx() { return up.x; }
	float GetUpy() { return up.y; }


	XMFLOAT3 GetPos() { return eye; }
	XMFLOAT3 GetPosOld() { return oldeye; }


	XMFLOAT3 GetEye2() {
		return eye2;
	}
	int Gets() { return s; }


	XMMATRIX GetRot() { return matRot; }

	XMVECTOR GetMove() { return move; }

	XMVECTOR GetMoveOld() { return moveOld; }

	XMVECTOR GetMoveOld2() { return moveOld2; }

	float GetVx() { return vect.m128_f32[0]; }
	float GetVy() { return vect.m128_f32[1]; }


	void SetMove(XMVECTOR move) { move_ = move; }
	void SetWarpPosition(XMFLOAT3 Warp) { Warp_=Warp; }
	void SetGround(bool onGround) { onGround_ = onGround; }
	void SetAngleRedX(float x) { angle_RedX = x;}
	void SetAngleRedY(float y) { angle_RedY = y;}
	void SetRedTeleport(bool teleport) { redTeleport = teleport;}
	void SetAngleBlueX(float x) { angle_BlueX = x; }
	void SetAngleBlueY(float y) { angle_BlueY = y; }
	void SetBlueTeleport(bool teleport) { blueTeleport = teleport; }
	void SetEyePos(XMFLOAT3 eye) { eye_ = eye; }
	void SetScene(int scene_) { scene = scene_; }
	void SetColision(bool a) { Colision = a; }
	void SetColisionVec(XMVECTOR a) { ColisionVec = a; }
	void SetRedAngle(XMFLOAT2 a) { RedAngle = a; }
	void SetBlueAngle(XMFLOAT2 a) { BlueAngle = a; }

	void SetRed(XMMATRIX a) { RotRed = a; }
	void SetBlue(XMMATRIX a) { RotBlue = a; }

	
private:
	// 入力クラスのポインタ
	Input* input;

	// カメラ注視点までの距離
	float distance = 20;
	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	XMVECTOR ColisionVec;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	// 回転行列
	XMMATRIX OldmatRot = DirectX::XMMatrixIdentity();;

	// 回転行列
	XMMATRIX PitchRot = DirectX::XMMatrixIdentity();;


	XMMATRIX RotRed = DirectX::XMMatrixIdentity();;
	XMMATRIX RotBlue = DirectX::XMMatrixIdentity();;



	XMFLOAT2 RedAngle;
	XMFLOAT2 BlueAngle;

	//
	bool PitchFlag=false;
	float camera_posX = 0;
	float camera_posZ = 0;

	bool Colision = false;
	bool OldColision;

	bool redfall = false;
	bool bluefall = false;

	XMMATRIX RotRedFall = DirectX::XMMatrixIdentity();;
	XMMATRIX RotBlueFall = DirectX::XMMatrixIdentity();;


	float angleX = 0;//カメラの角度X
	float angleY = 0;//カメラの角度Y
	float oldx = 0;
	float oldy = 0;

	const float Ras = 3.15;

	int Flag = 0;

	bool onGround_ ;
int s = 0;

	bool redTeleport;
	bool blueTeleport;

	float angle_RedX;
	float angle_RedY;

	float angle_BlueX;
	float angle_BlueY;

	float t = 0;

	bool AngleRimit = false;

	int scene=0;

	int IventNumber = 0;

	int IventTime = 0;

	XMVECTOR fallV = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	XMVECTOR JumpV = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	XMVECTOR move= DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR moveOld = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR moveOld2 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	volatile XMVECTOR movevol{};

	XMVECTOR move_ = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR vect= DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMFLOAT3 Warp_={0,0,0};
	XMFLOAT3 eye_ = { 0,0,0 };
	XMFLOAT3 oldeye = { 0,0,0 };
	XMFLOAT3 position1{};

	bool trun = false;
	int time2 = 0;
	int time3 = 0;
};

