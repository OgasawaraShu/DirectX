#pragma once
#include "BlueCamera.h"

/// <summary>
/// �f�o�b�O�p�J����
/// </summary>
class BlueCameraDebug :
	public BlueCamera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <param name="input">����</param>
	BlueCameraDebug(int window_width, int window_height);

	// �X�V
	void Update() override;
	//pos�̃Z�b�g
	void SetEyePos(XMFLOAT3 eye) { eye_ = eye; }
	//�����̃Z�b�g
	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}
	//��]�̃Z�b�g
	void SetRot(XMMATRIX a) { matRotPortal = a; }

	void SetMatRot(XMMATRIX matrot) { Cha_matrot = matrot; }

	//Getter
	//pos
	XMFLOAT3 GetPos() { return eye; }
	//rot
	XMMATRIX GetRot() { return matRot; }
private:
	// �J���������_�܂ł̋���
	float distance = 20;
	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	//�ԃ|�[�^���̉�]
	XMMATRIX matRotPortal;
	//pos�̓��ꕨ�ϐ�
	XMFLOAT3 eye_;
	XMMATRIX Cha_matrot;

};

