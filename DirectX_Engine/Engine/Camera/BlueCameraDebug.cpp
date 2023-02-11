#include "BlueCameraDebug.h"
#include <cassert>

using namespace DirectX;

BlueCameraDebug::BlueCameraDebug(int window_width, int window_height)
	: BlueCamera(window_width, window_height)
{
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void BlueCameraDebug::Update()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	dirty = true;

	
	SetEye(eye_);

	if (dirty || viewDirty) {
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;

		matRot = matRotPortal;

		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
		vUp = XMVector3Transform(vUp, matRot);


		// �����_���炸�炵���ʒu�Ɏ��_���W������
		const XMFLOAT3& target = GetEye();
		SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });
		SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
	}

	BlueCamera::Update();
}
