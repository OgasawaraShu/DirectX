#include "DebugCamera.h"
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
void DebugCamera::Initialize2()
{
 camera_posX = 0;
	 camera_posZ = 0;

	 Colision = false;

	 angleX = 0;//�J�����̊p�xX
	 angleY = 0;//�J�����̊p�xY
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
	// First_ivent_flag = true;


	 if (Old_scene == 1)
	 {
		 eye = { 400,20,-20 };
	 }
	 else if (Old_scene == 2)
	 {
		 eye = { 800,20,-20 };
	 }
	 SetEye(eye);
	 SetTarget({ eye.x, eye.y, eye.z + 20 });
}
#pragma optimize("", off)
void DebugCamera::Update()
{
	if (scene == 1||scene==2||scene==3)
	{

		if (First_ivent_flag == false)
		{
			MainSceneUpdate();
		}
		else
		{
			IventMainUpdate();
		}

		
		if (time2 == 0)
		{
			oldx = 0;
			oldy = 0;
			time2 += 1;
		}

	}
	else if (scene == 0)
	{
		TitleSceneUpdate();
		//IventMainUpdate();
	}
	else if (scene == 99)
	{
		LoadUpdate();	

		SetTarget({ eye.x, eye.y, eye.z + 20 });
		SetUp({ 0, 1,0});
	}


	if (Menu_flag==false&&viewDirty) {

		if (scene == 1 || scene == 2 || scene == 3)
		{
			// �ǉ���]���̉�]�s��𐶐�
			XMMATRIX matRotNew = XMMatrixIdentity();

			matRotNew *= XMMatrixRotationX(-oldx);
			matRotNew *= XMMatrixRotationY(-oldy);

			// �ݐς̉�]�s�������
			// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
			// �Nb�H�[�^�j�I�����g�p��������]�܂���

			matRot = matRotNew;
		}
		else
		{
			// �ǉ���]���̉�]�s��𐶐�
			XMMATRIX matRotNew = XMMatrixIdentity();

			matRotNew *= XMMatrixRotationX(-angleX);
			matRotNew *= XMMatrixRotationY(-angleY);

			// �ݐς̉�]�s�������
			// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
			// �Nb�H�[�^�j�I�����g�p��������]�܂���

			matRot = matRotNew * matRot;
		}



		//�e���|�[�g������p�x�����̋t�ɂ���
		if (redTeleport == true)
		{
			matRot = RotRed;
			RotRedFall = matRot;

			if (Red_rotate.y == 180)
			{
				Red_rotate.y = 0;
			}
			else if (Red_rotate.y == 0)
			{
				Red_rotate.y = 180;
			}
			oldy = XMConvertToRadians(Red_rotate.y);
		
			XMVECTOR movewarp;
			movewarp = { 0,0,2,0 };
			movewarp = XMVector3Transform(movewarp, matRot);
			MoveVectorNotY(movewarp);
			redfall = true;
			
		}

		//�e���|�[�g������p�x�����̋t�ɂ���
		if (blueTeleport == true)
		{
			matRot = RotBlue;
			RotBlueFall = matRot;

			if (Blue_rotate.y == 180)
			{
				Blue_rotate.y = 0;
			}
			else if (Blue_rotate.y == 0)
			{
				Blue_rotate.y = 180;
			}

			oldy = XMConvertToRadians(Blue_rotate.y);

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

		

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��  ���_
		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		vect = vEyeTarget;
		// �����_���炸�炵���ʒu�Ɏ��_���W����
		const XMFLOAT3& target = GetEye();

		if (scene != 99)
		{
			if (vEyeTarget.m128_f32[1]<19 && vEyeTarget.m128_f32[1]>-19) {
				SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });
			}
		}
	}

	if(Colision==false)oldeye = eye;

	Camera::Update();
}

volatile void DebugCamera::MainSceneUpdate()
{
	oldeye = eye;



	float target_x = 0;

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	GamePad* GP = nullptr;
	GP = new GamePad();

	//�p�b�h�̍X�V
	GP->Update();

	if (Tutorial == true)
	{
		mouseMove.lX = 0;
		mouseMove.lY = 0;
		mouseMove.lZ = 0;
	}

	if (Menu_flag == false)
	{
		float dy = mouseMove.lX * scaleY*Mouse_sensitivity;
		float dx = mouseMove.lY * scaleX*Mouse_sensitivity;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;

	}

	//�Q�[���p�b�h�A�i���O�X�e�B�b�NR���͎�����(���_�ړ�)
	if ((GP->state.Gamepad.sThumbRX != 0 || GP->state.Gamepad.sThumbRY != 0) && Menu_flag == false && Tutorial == false)
	{
		float dy = static_cast<FLOAT>(GP->state.Gamepad.sThumbRX / 32767.0 * (Pad_sensitivity));
		float dx = static_cast<FLOAT>(GP->state.Gamepad.sThumbRY / 32767.0 * (Pad_sensitivity));

		if (Reverse_switch == false)
		{
			angleX = dx * XM_PI;
		}
		else
		{
			angleX = -dx * XM_PI;
		}
		angleY = -dy * XM_PI;
	}
	
	oldx += angleX;
	oldy += angleY;

	if (oldx<1.5 && oldx>-1.5)
	{
		Old_oldx = oldx;
	}
	else
	{
		oldx = Old_oldx;
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
	}

	if (Colision == false)
		MoveVectorNotY(move);
	else
		MoveVectorNotY2(move);
	moveOld = -1*move;
	moveOld2 =move;

	//onGround��false�Ȃ�Y�����Q�Ƃ���vector�ړ�����
	if (onGround_ != true)
	{
		XMVECTOR move = move_;

		MoveVector(move);
	}

	MoveVector(fall_);

	//�e���|�[�g��������W�����킹��
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
	First_ivent_flag = true;
	startIndex = 1;

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
	eye = { 0,7,-50 };
	SetEye(eye);

	//eye����target�ւ̋���
	float distance = 20.0f;

	//�C�x���g�̎��_�ړ��̓_
	std::vector<XMFLOAT3>points;

	//�Ō�ƏI����2����
	//eye�����distance��ǉ����ĕω�����
	points = {
		{eye.x             ,eye.y +  distance   ,eye.z             },
		{eye.x             ,eye.y +  distance   ,eye.z             },
		{eye.x             ,eye.y               ,eye.z+(distance/2)},
		{eye.x             ,eye.y               ,eye.z+  (distance)},
		{eye.x+(distance/2),eye.y +(distance/14),eye.z+(distance/2)},
		{eye.x+  (distance),eye.y               ,eye.z             },
		{eye.x+(distance/2),eye.y               ,eye.z+(distance/2)},
		{eye.x             ,eye.y               ,eye.z+  (distance)},
		{eye.x-(distance/2),eye.y               ,eye.z+(distance/2)},
		{eye.x-  (distance),eye.y               ,eye.z             },
		{eye.x-(distance/2),eye.y +(distance/20),eye.z+(distance/2)},
		{eye.x             ,eye.y               ,eye.z + (distance)},
		{eye.x             ,eye.y               ,eye.z + (distance)},
		{eye.x             ,eye.y               ,eye.z + (distance)},
		{eye.x             ,eye.y               ,eye.z + (distance)},
	};

	//����i�߂�
	timeRate += 0.01f;

	//1�ȏア�����玟�ɐi�߂�
	if (timeRate >= 1.0f)
	{
		if (startIndex < points.size() - 3)
		{
			if (startIndex != points.size() - 4)
			{
				startIndex += 1;
			}
			else
			{
				//�S�ẴX�v���C���Ȑ����ړ������玟�̃C�x���g
				//startIndex = 1;
				First_ivent_flag = false;
			}
			timeRate -= 1.0f;
		}
		else
		{
			timeRate = 1.0f;
		}
	}

	//�v�Z�����_�̍��W��target�̍��W�ɑ��
	XMFLOAT3 target_ = Physics::splinePosition(points, startIndex, timeRate);
	SetTarget(target_);


	Camera::Update();
}

void DebugCamera::MapEditUpdate()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	// �}�E�X�̍��{�^����������Ă�����J��������]������
	if (input->PushMouseLeft())
	{
		float dy = mouseMove.lX * scaleY;
		float dx = mouseMove.lY * scaleX;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;
		dirty = true;
	}

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�������
	if (input->PushMouseMid()&&input->PushKey(DIK_C))
	{
		float dx = mouseMove.lX / 10.0f;
		float dy = mouseMove.lY / 10.0f;

		XMVECTOR move = { -dx, +dy, 0, 0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	// �z�C�[�����͂ŋ�����ύX
	if (mouseMove.lZ != 0) {
		distance -= mouseMove.lZ / 20.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty || viewDirty) {
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		const XMFLOAT3& target = GetTarget();
		SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
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
		

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��  ���_
		XMVECTOR vEyeTarget = { 0.0f, 0.0f, distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// �x�N�g������]
		vEyeTarget = XMVector3Transform(vEyeTarget, matRot);

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		const XMFLOAT3& target = GetEye();

		if (scene != 99)
		{
		
		SetTarget({ target.x + vEyeTarget.m128_f32[0], target.y + vEyeTarget.m128_f32[1], target.z + vEyeTarget.m128_f32[2] });	
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


