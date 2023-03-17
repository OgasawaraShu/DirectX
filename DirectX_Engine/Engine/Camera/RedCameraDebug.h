#pragma once
#include "RedCamera.h"

/// <summary>
/// デバッグ用カメラ
/// </summary>
class RedCameraDebug :
	public RedCamera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <param name="input">入力</param>
	RedCameraDebug(int window_width, int window_height);

	// 更新
	void Update() override;

	void SetEyePos(XMFLOAT3 eye) { eye_ = eye; }

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

	void SetRot(XMMATRIX a) { matRotPortal = a; }

	void SetAngle(XMFLOAT2 angle_) { angle = angle_; }

	XMFLOAT3 GetPos() { return eye; }

	XMMATRIX GetRot() { return matRot; }



	void SetMatRot(XMMATRIX matrot) { Cha_matrot = matrot; }
private:
	// カメラ注視点までの距離
	float distance = 20;
	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	XMMATRIX matRotPortal;
	XMMATRIX Cha_matrot;

	XMFLOAT2 angle;
	XMFLOAT3 eye_;
};

