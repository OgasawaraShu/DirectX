#pragma once
#include "Camera.h"
#include "Input.h"
#include "GamePad.h"

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

	// 更新
	void Update() override;

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

	float GetAngleX();
	float GetAngleY();

	float GetPositionX();
	float GetPositionY();


	XMMATRIX GetRot() { return matRot; }

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


private:
	// 入力クラスのポインタ
	Input* input;
	//
	
	// カメラ注視点までの距離
	float distance = 20;
	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	//
	float camera_posX = 0;
	float camera_posZ = 0;

	double angleX = 0;//カメラの角度X
	double angleY = 0;//カメラの角度Y
	double oldx = 0;
	float oldy = 0;

	const float Ras = 3.15;

	int Flag = 0;

	bool onGround_ ;

	bool redTeleport;
	bool blueTeleport;

	float angle_RedX;
	float angle_RedY;

	float angle_BlueX;
	float angle_BlueY;

	bool AngleRimit = false;


	
	XMVECTOR fallV{};
	XMVECTOR JumpV{};

	XMVECTOR move_;
	XMFLOAT3 Warp_;
	XMFLOAT3 eye_;
};

