#pragma once
#include "Camera.h"
#include "Input.h"
#include "GamePad.h"

/// <summary>
/// �f�o�b�O�p�J����
/// </summary>
class DebugCamera :
	public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <param name="input">����</param>
	DebugCamera(int window_width, int window_height, Input* input);

	// �X�V
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


private:
	// ���̓N���X�̃|�C���^
	Input* input;
	//
	
	// �J���������_�܂ł̋���
	float distance = 20;
	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	//
	float camera_posX = 0;
	float camera_posZ = 0;

	double angleX = 0;//�J�����̊p�xX
	double angleY = 0;//�J�����̊p�xY
	double oldx = 0;
	float oldy = 0;

	const float Ras = 3.15;

	int Flag = 0;

	bool onGround_ ;

	bool redTeleport;

	float angle_RedX;
	float angle_RedY;

	XMVECTOR move_;
	XMFLOAT3 Warp_;
};

