#include "BulletFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void BulletFbx::OnCollision(const CollisionInfo& info)
{
	if (debug == 0 && TriggerFlag == 1)debug = 1;
	if (debug2 == 0 && TriggerFlag2 == 1)debug2 = 1;
}

BulletFbx::BulletFbx(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void BulletFbx::BlueBulletUpdate(double angleX, double angleY)
{
	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	//�p�b�h�̃|�C���^
	GamePad* GP = nullptr;
	GP = new GamePad();
	//�p�b�h�̍X�V
	GP->Update();


	//�p�x�̃t���O
	bool dirty = false;

	//�}�E�X�p�x�o��Ver
	if (TriggerFlag == 0)
	{
		//�}�E�X�̍���������Ȃ�����J�����̊p�x���o��������
		angleX1 = angleX;
		angleY1 = angleY;
	}
	else
	{
		//�����ꂽ��t���O��true�ɂ��o����̂���߂�
		dirty = true;
	}

	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//false�̊Ԃ͊p�x��ς�������
	if (dirty == false)
	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX1);
		matRotNew *= XMMatrixRotationY(-angleY1);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;



		//�x�N�g���ƍs��̐�

		move = XMVector3Transform(move, matRot);
	}
	

	XMVECTOR moveCamera = move_;
	moveCamera = XMVector3Transform(moveCamera, matRot);


	if (input->TriggerMouseLeft())
	{
		TriggerFlag = 1;
	}


	if (TriggerFlag == 1 && debug == 0)
	{
		//���s�ړ�(���������ꂽ�狅���J�����̕����ɍ��킹�đO�ɏo�Ă���)
		matTrans = XMMatrixTranslation(position.x += move.m128_f32[0], position.y += move.m128_f32[1], position.z += move.m128_f32[2]);
	}
	else if (TriggerFlag == 0 && debug == 0)
	{
		//���s�ړ�
		matTrans = XMMatrixTranslation(position.x += moveCamera.m128_f32[0], position.y += moveCamera.m128_f32[1], position.z += moveCamera.m128_f32[2]);
	}
	else
	{
		//���s�ړ�
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}

	memory.m128_f32[0] += move.m128_f32[0];
	memory.m128_f32[1] += move.m128_f32[1];
	memory.m128_f32[2] += move.m128_f32[2];


	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//�J�����̍s���������
	if (TriggerFlag == 0 && debug == 0)
	{
		matWorld *= matrot;
	}

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

void BulletFbx::RedBulletUpdate(double angleX, double angleY)
{
	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	//�p�b�h�̃|�C���^
	GamePad* GP = nullptr;
	GP = new GamePad();
	//�p�b�h�̍X�V
	GP->Update();


	//�p�x�̃t���O
	bool dirty = false;

	//�}�E�X�p�x�o��Ver
	if (TriggerFlag2 == 0)
	{
		//�}�E�X�̍���������Ȃ�����J�����̊p�x���o��������
		angleX2 = angleX;
		angleY2 = angleY;


	}
	else
	{
		//�����ꂽ��t���O��true�ɂ��o����̂���߂�
		dirty = true;
	}

	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//false�̊Ԃ͊p�x��ς�������
	if (dirty == false)
	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX2);
		matRotNew *= XMMatrixRotationY(-angleY2);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;



		//�x�N�g���ƍs��̐�

		move2 = XMVector3Transform(move2, matRot);
	}
	

	XMVECTOR moveCamera = move_;
	moveCamera = XMVector3Transform(moveCamera, matRot);


	if (input->TriggerMouseRight())
	{
		TriggerFlag2 = 1;
	}


	if (TriggerFlag2 == 1 && debug2 == 0)
	{
		//���s�ړ�(���������ꂽ�狅���J�����̕����ɍ��킹�đO�ɏo�Ă���)
		matTrans = XMMatrixTranslation(position.x += move2.m128_f32[0], position.y += move2.m128_f32[1], position.z += move2.m128_f32[2]);
	}
	else if (TriggerFlag2 == 0 && debug2 == 0)
	{
		//���s�ړ�
		matTrans = XMMatrixTranslation(position.x +=moveCamera.m128_f32[0], position.y += moveCamera.m128_f32[1], position.z += moveCamera.m128_f32[2]);
	}
	else
	{
		//���s�ړ�
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}

	memory3.m128_f32[0] = position.x;
	memory3.m128_f32[1] = position.y;
	memory3.m128_f32[2] = position.z;


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
