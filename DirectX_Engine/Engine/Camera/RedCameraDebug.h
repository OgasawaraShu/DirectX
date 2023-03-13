#pragma once
#include "RedCamera.h"

/// <summary>
/// �f�o�b�O�p�J����
/// </summary>
class RedCameraDebug :
	public RedCamera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <param name="input">����</param>
	RedCameraDebug(int window_width, int window_height);

	// �X�V
	void Update() override;

	void SetEyePos(XMFLOAT3 eye) { eye_ = eye; }

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

	void SetRot(XMMATRIX a) { matRotPortal = a; }

	void SetAngle(XMFLOAT2 angle_) { angle = angle_; }

	XMFLOAT3 GetPos() { return eye; }

	XMMATRIX GetRot() { return matRot; }


private:
	// �J���������_�܂ł̋���
	float distance = 20;
	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	XMMATRIX matRotPortal;

	XMFLOAT2 angle;
	XMFLOAT3 eye_;
};

