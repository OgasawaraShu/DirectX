#include "ObjFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;


ObjFbx::ObjFbx(Input* input, Physics* physics)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
	this->physics = physics;
}

void ObjFbx::ObjUpdate(float angleX, float angleY)
{
	//FBX�Ǝ��̍X�V
	oldangleY += angleY;
	oldangleX += angleX;

	XMMATRIX matScale, matRot, matTrans{};

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	


	Vector.m128_f32[0] = Target.x- MyPosition.x;
	Vector.m128_f32[1] = Target.y- MyPosition.y;
	Vector.m128_f32[2] = Target.z+2 - MyPosition.z+2;

	Vector = XMVector3Normalize(Vector) * 2.0f;

	RayCheck();

	if (cursorOn == true)
	{
		//�ǉ��̉�]�s�񏈗�
		AddRotateMatrixUpdate(angleX, angleY, matRot);

		move = { (4,0,0,0) };

		position.x = (MyPosition.x + Vector.m128_f32[0] * 3)+move.m128_f32[1];
		position.y = (MyPosition.y + Vector.m128_f32[1] * 3);
		position.z = (MyPosition.z + Vector.m128_f32[2] * 3);


		
		rotation.y = atan2(Vector.m128_f32[0], Vector.m128_f32[2])*60;

		if (Target.z < MyPosition.z)
		{
			Vector.m128_f32[2] = MyPosition.z + 2 - Target.z + 2;
		}

		rotation.x = -atan2(Vector.m128_f32[1], Vector.m128_f32[2])*60;

		if (rotation.x > 90)
		{
			rotation.x = rotation.x - 180;
		}
		else if (rotation.x < -90)
		{
			rotation.x = rotation.x + 180;
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

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit,1)&&input->TriggerMouseMid()&&!cursorOn) {
		cursorOn = true;

		rotation.x = 0;
		rotation.z = 0;
		rotation.y = 180;
	}
	else if(input->TriggerMouseMid() &&cursorOn==true)
	{
		cursorOn = false;
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
