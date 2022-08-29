#include "DebugCamera.h"
#include "Fbx3d.h"
#include <cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height, Input* input)
	: Camera(window_width, window_height)
{
	assert(input);

	this->input = input;
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void DebugCamera::Update()
{
	bool dirty = false;
	float target_x = 0;

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	GamePad* GP = nullptr;
	GP = new GamePad();

	//�p�b�h�̍X�V
	GP->Update();

	float dy = mouseMove.lX * scaleY;
	float dx = mouseMove.lY * scaleX;

	angleX = -dx * XM_PI;
	angleY = -dy * XM_PI;

	dirty = true;

	oldx += angleX;
	oldy += angleY;

	//�J������X���x�N�g�������ɍs�������Ȃ牟���߂�
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

	

	//�Q�[���p�b�h�A�i���O�X�e�B�b�NR���͎�����(���_�ړ�)
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
	
	//onGround��false�Ȃ�Y�����Q�Ƃ���vector�ړ�����
	if (onGround_ != true)
	{
		XMVECTOR move = move_;
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	SetEye(eye_);
	
	//�Q�[���p�b�h�A�i���O�X�e�B�b�NL���͎�����(�ꏊ�ړ�)
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
	//�e���|�[�g��������W�����킹��
	if (blueTeleport == true || redTeleport == true)
	{
		SetEye(Warp_);
	}

	//�e���|�[�g������p�x�����̋t�ɂ���
	if (redTeleport == true)
	{
		angleX = angle_RedX;
		angleY = angle_RedY-Ras;

		dirty = true;
	}

	//�e���|�[�g������p�x�����̋t�ɂ���
	if (blueTeleport == true)
	{
		angleX = angle_BlueX;
		angleY = angle_BlueY - Ras;

		dirty = true;
	}

	if (dirty || viewDirty) {
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
	//	matRotNew *= XMMatrixRotationZ(0);
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��  ���_
		//XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		//vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
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


