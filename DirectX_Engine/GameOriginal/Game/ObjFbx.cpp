#include "ObjFbx.h"
#include "../../Engine/Collision/CollisionManager.h"
#include "../../Engine/Collision/BaseCollider.h"
#include "../../Engine/Collision/CollisionAttribute.h"
#include <d3dcompiler.h>
#include "../../Engine/Collision/Collision.h"


#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;




ObjFbx::ObjFbx(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void ObjFbx::OnCollision(const CollisionInfo& info)
{
	
}

void ObjFbx::ObjInitialize()
{
	//�ϐ��̏�����
	cursorOn = false;

	cursorOn2 = false;

	Shot = false;

	Shot2 = false;

	Recoile = false;

	Recoile2 = false;

	warkmove = true;

	Wark_time = 0;

	onGround = true;
}

void ObjFbx::ObjUpdate(float angleX, float angleY)
{
	//FBX�Ǝ��̍X�V
	oldangleY += angleY;
	oldangleX += angleX;

	XMMATRIX matScale, matRot, matTrans{};
	//Matrix�̕ω�
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	
	//�����_�Ǝ����̈ʒu�̃x�N�g���v�Z
	Vector.m128_f32[0] = Target.x- MyPosition.x;
	Vector.m128_f32[1] = Target.y- MyPosition.y;
	Vector.m128_f32[2] = Target.z+2 - MyPosition.z+2;
	//�x�N�g���̐��K��
	Vector = XMVector3Normalize(Vector) * 2.0f;
	//���C�̃`�F�b�N
	RayCheck();
	//���R�C���t���O��true�Ȃ烊�R�C����������
	if (Recoile == true)
	{
		//�t���O�؂�ւ��œ��������ς���
		if (Recoile2 == false)
		{
			AddRotate.x -= 6;
			AddPosition.z = -0.5;
		}
		else
		{
			AddRotate.x += 6;
			AddPosition.z = 0.5;
			if (AddRotate.x > 1)
			{
				Recoile = false;
				Recoile2 = false;

				AddPosition.z = 0;
				AddRotate.x = 0;
			}
		}

		//�e�g��������؂�����؂�ւ���
		if (AddRotate.x < -28)
		{
			Recoile2 = true;
		}
	}

	//�t���O���������玝���グ�ĒǏ]����
	if (cursorOn == true)
	{
		//�ǉ��̉�]�s�񏈗�
		AddRotateMatrixUpdate(angleX, angleY, matRot);
		//�ʒu���������炷�p
		move = { 2 + AddPosition.x,AddPosition.y,AddPosition.z,0 };
		//�|�W�V�����̃x�N�g��
		position.x = (MyPosition.x + +Vector.m128_f32[0] * 3);
		position.y = (MyPosition.y + Vector.m128_f32[1] * 3) - 2;
		position.z = (MyPosition.z + Vector.m128_f32[2] * 3);
		//�ǂ��炩�ł����̂Ȃ烊�R�C���I��
		if (Shot == true || Shot2 == true)
		{
			Recoile = true;
		}
		//�����Ă���̂Ȃ�����Ă郂�[�V����������
		if (wark == true)
		{
			//���L�����Ă�Ƃ��̃��[�V����������
			if (warkmove == true && Wark_time < 60)
			{
				Wark_time += 1;
				AddPosition.y += 0.007;
				AddPosition.x += 0.012;

				if (Wark_time >= 60)
				{
					Wark_time = 0;
					warkmove = false;
				}
			}
			else if (warkmove == false && Wark_time < 60)
			{
				Wark_time += 1;
				AddPosition.y -= 0.007;
				AddPosition.x -= 0.012;


				if (Wark_time >= 60)
				{
					Wark_time = 0;
					warkmove = true;
				}
			}
		}
		else
		{
			//�����ĂȂ��̂Ȃ�ʒu��߂�
			AddPosition.y = 0;
			AddPosition.x = 0;
		}

		//atan�ŏe�g��������������Z�o
		rotation.y = atan2(Vector.m128_f32[0], Vector.m128_f32[2]) * 60;

		//�����_�����ɍs�����甽�]����
		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}
		//���s�ړ�
		move = XMVector3Transform(move, matRot);
		matTrans = XMMatrixTranslation(position.x += move.m128_f32[0], position.y += move.m128_f32[1], position.z += move.m128_f32[2]);
        //�������������
		rotation.x = (-atan2(Vector.m128_f32[1], Vector.m128_f32[2]) * 60) + AddRotate.x;
		//�t���O�؂�ւ��œ��������ς���
		if (Recoile == false)
		{
			if (rotation.x > 90)
			{
				rotation.x = rotation.x - 180;
			}
			else if (rotation.x < -90)
			{
				rotation.x = rotation.x + 180;
			}
		}

	}

	//�s���X�V
	PostMatrixUpdate(matScale, matRot, matTrans);
}

void ObjFbx::RayCheck()
{
	// ���C�̃X�^�[�g�n�_��ݒ�
	Ray ray;
	ray.start = { MyPosition.x,MyPosition.y,MyPosition.z,0 };
	ray.start.m128_f32[1] += 2.5f;
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;

	ray.dir = CammeraZAxis;


	//�p�b�h�̃|�C���^
	GamePad* GP = nullptr;
	GP = new GamePad();

	//�p�b�h�̍X�V
	GP->Update();


	//���C�����������̂Ȃ�擾����
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit,20.0f)&&(input->TriggerKey(DIK_F)|| (GP->iPad_B == 1 && Old_iPad_B == 0)) && !cursorOn&&Tutorial == false) {
		cursorOn = true;
		Tutorial_gun = true;
		rotation.x = 0;
		rotation.z = 0;
		rotation.y = 180;
	}

	//�g���K�[�����̂��߂̋L��
	Old_iPad_left = GP->iPad_left, Old_iPad_right = GP->iPad_right, iOld_Pad_up = GP->iPad_up, Old_iPad_down = GP->iPad_down;
	Old_iPad_leftshoulder = GP->iPad_leftshoulder, Old_iPad_rightshoulder = GP->iPad_rightshoulder;
	Old_iPad_A = GP->iPad_A, Old_iPad_B = GP->iPad_B, Old_iPad_X = GP->iPad_X, Old_iPad_Y = GP->iPad_Y;

}

void ObjFbx::BoxObjUpdate(float angleX, float angleY)
{
	//FBX�Ǝ��̍X�V
	oldangleY += angleY;
	oldangleX += angleX;
	//matrix����
	XMMATRIX matScale, matRot, matTrans{};

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//�����_��pos�̃x�N�g���̌v�Z
	Vector.m128_f32[0] = Target.x - MyPosition.x;
	Vector.m128_f32[1] = Target.y - MyPosition.y;
	Vector.m128_f32[2] = Target.z - MyPosition.z;

	Vector = XMVector3Normalize(Vector) * 2.0f;
	//���C�̃`�F�b�N
	BoxRayCheck();
	//�d�͏���
	//ObjFall();
	//���s�ړ�
	matTrans = XMMatrixTranslation(position.x , position.y += fallV.m128_f32[1], position.z);

	//�擾���Ă���̂Ȃ�ǉ��̉�]�s��
	if (cursorOn2 == true)
	{
		//�ǉ��̉�]�s�񏈗�
		AddRotateMatrixUpdate(angleX, angleY, matRot);
	}
	//�s���X�V
	PostMatrixUpdate(matScale, matRot, matTrans);
}

void ObjFbx::BoxRayCheck()
{
	// ���C�̃X�^�[�g�n�_��ݒ�
	Ray ray;
	ray.start = { MyPosition.x,MyPosition.y,MyPosition.z,0 };
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;
	ray.dir = CammeraZAxis;


	//�p�b�h�̃|�C���^
	GamePad* GP = nullptr;
	GP = new GamePad();

	//�p�b�h�̍X�V
	GP->Update();

	//���C���������Ă���̂Ȃ�擾
	if (CollisionManager::GetInstance()->Raycast(ray,COLLISION_ATTR_OBJ2, &raycastHit, 35.0f) && (input->TriggerKey(DIK_F) || (GP->iPad_B == 1 && Old_iPad_B == 0)) && !cursorOn2&&Tutorial==false) {
		cursorOn2 = true;
	}
	else if ((input->TriggerKey(DIK_F)||(GP->iPad_B == 1 && Old_iPad_B == 0)) && cursorOn2 == true)
	{
		//��������
		cursorOn2 = false;
	}

	//�����Ă�Ƃ��͏������ŒǏ]������
	if (cursorOn2 == true)
	{
		move = { 2,0,0,0 };
		//�������Ŏ�������
		position.x = (MyPosition.x + Vector.m128_f32[0] * 8);
		position.y = (MyPosition.y + Vector.m128_f32[1] * 8);
		position.z = (MyPosition.z + Vector.m128_f32[2] * 8);

		//���]�������߂�
		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}
	}

	//�g���K�[�����̂��߂̋L��
	Old_iPad_left = GP->iPad_left, Old_iPad_right = GP->iPad_right, iOld_Pad_up = GP->iPad_up, Old_iPad_down = GP->iPad_down;
	Old_iPad_leftshoulder = GP->iPad_leftshoulder, Old_iPad_rightshoulder = GP->iPad_rightshoulder;
	Old_iPad_A = GP->iPad_A, Old_iPad_B = GP->iPad_B, Old_iPad_X = GP->iPad_X, Old_iPad_Y = GP->iPad_Y;
}

void ObjFbx::TurettUpdate()
{
	RazerRayCheck();

	BoxRayCheck();

	//FBX�̍X�V
	Update();

}

void ObjFbx::RazerRayCheck()
{
	// ���C�̃X�^�[�g�n�_��ݒ�
	Ray ray;
	ray.start = { position.x,position.y,position.z,0 };
	ray.start.m128_f32[1] += 2.5f;
	ray.dir = { 0,0,1,0 };
	RaycastHit raycastHit;

	XMMATRIX matRot;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	ray.dir = XMVector3Transform(ray.dir, matRot);

	//���C�����������̂Ȃ�擾����
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_PLAYER, &raycastHit, 20.0f)) {

	}
}

void ObjFbx::ObjFall()
{
	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	}

	// ���C�̃X�^�[�g�n�_��ݒ�
	Sphere sphere;
    sphere.center = { MyPosition.x,MyPosition.y,MyPosition.z,0 };
	sphere.redius = 5.0f;
	RaycastHit raycastHit;

	if (CollisionManager::GetInstance()->Spherecast(sphere, COLLISION_ATTR_WALL, &raycastHit, 35.0f)) {
		onGround = true;
	}
	else
	{
		onGround = false;
	}
}

void ObjFbx::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
{
	//���s�ړ�
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection =
		camera->GetViewProjectionMatrix();
	//���b�V��t�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;

	//�萔�o�b�t�@�֓]��

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//�A�j���[�V����
	if (isPlay) {
		//1�t���[���i�߂�
		currentTime += frameTime;

		//�Ō�܂ōs������擪�ɖ߂�
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}

	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	//�����蔻��X�V
	if (collider) {
		collider->Update();
	}
}

void ObjFbx::AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot)
{
	angleX1 = angleX;
	angleY1 = angleY;

	// �ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX1);
	matRotNew *= XMMatrixRotationY(-angleY1);
	// �ݐς̉�]�s�������
	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	// �N�H�[�^�j�I�����g�p��������]�܂���
	matRot = matRotNew * matRot;

    move = XMVector3Transform(move, matRot);
}
