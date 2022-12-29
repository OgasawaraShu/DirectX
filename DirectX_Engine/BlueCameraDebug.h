#pragma once
#include "BlueCamera.h"

/// <summary>
/// デバッグ用カメラ
/// </summary>
class BlueCameraDebug :
	public BlueCamera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <param name="input">入力</param>
	BlueCameraDebug(int window_width, int window_height);

	// 更新
	void Update() override;

	void SetEyePos(XMFLOAT3 eye) { eye_ = eye; }

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

	void SetRot(XMMATRIX a) { matRotPortal = a; }

	XMFLOAT3 GetPos() { return eye; }

	XMMATRIX GetRot() { return matRot; }
private:
	// カメラ注視点までの距離
	float distance = 20;
	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// 回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	XMMATRIX matRotPortal;

	XMFLOAT3 eye_;
};

