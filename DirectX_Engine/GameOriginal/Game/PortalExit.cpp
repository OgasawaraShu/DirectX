#include "PortalExit.h"
#include "../../Engine/Collision/CollisionManager.h"
#include "../../Engine/Collision/BaseCollider.h"
#include "../../Engine/Collision/CollisionAttribute.h"
#include <d3dcompiler.h>
#include "../../Engine/Collision/Collision.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;


PortalExit::PortalExit(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void PortalExit::ExitUpdate(float angleX, float angleY)
{
	XMMATRIX matScale, matRot, matTrans;



	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//���s�ړ�
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//OBj�Ǝ��̊֐�
	OriginalUpdate(angleX, angleY);

	//�ǉ���]�s��
	AddMatrixUpdate(angleX, angleY, matRot);

	//Matrix��X�V
	PostMatrixUpdate(matScale, matRot, matTrans);

	
}

void PortalExit::OriginalUpdate(float angleX, float angleY)
{
	//�̂̉�]���o����
	OldAngle.x += angleX;
	OldAngle.y += angleY;

	scale = ScaleExit;

	//�t���O���������烏�[�v�̃X�P�[�����L����
	if (WarpFlag == true)
	{
		if (ScaleExit.x < 0.07)
		{
			ScaleExit.x += 0.001;
			ScaleExit.y += 0.001;
			ScaleExit.z += 0.001;


			//�����͊O�g
			if (MyPosition.z < position.z)
			{
				rotation.y = 0;
			}
			else
			{
				rotation.y = 180;
			}

			if (position.x <= -70)
			{
				rotation.y = 270;
			}
			else if (position.x >= 70)
			{
				rotation.y = 90;
			}

			//�����Ɍʂ�OBJ�ɓ����������̍��E�̉�]����
		}

		
	}
	else
	{
		//�t���O�������ĂȂ��̂Ȃ���Ă���
		ScaleExit.x = 0;
		ScaleExit.y = 0;
		ScaleExit.z = 0;
	}
}

void PortalExit::AddMatrixUpdate(float angleX, float angleY, XMMATRIX matRot)
{
	// �ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX);
	matRotNew *= XMMatrixRotationY(-angleY);
	// �ݐς̉�]�s�������
	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	// �N�H�[�^�j�I�����g�p��������]�܂���
	matRot = matRotNew * matRot;

	XMMATRIX matRot2;


	//���[�v���rotation�ۑ��p
	// �ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew2 = XMMatrixIdentity();
	matRotNew2 *= XMMatrixRotationZ(XMConvertToRadians(-rotation.z));
	matRotNew2 *= XMMatrixRotationX(-260.7);
	matRotNew2 *= XMMatrixRotationY(0);
	// �ݐς̉�]�s�������
	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	// �N�H�[�^�j�I�����g�p��������]�܂���
	matRot2 = matRotNew2 * matRot;
	a = matRot2;
}

void PortalExit::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
{
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
