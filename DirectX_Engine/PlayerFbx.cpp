#include "PlayerFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void PlayerFbx::OnCollision(const CollisionInfo& info)
{
	
}

void PlayerFbx::Initialize_Bullet()
{

}

PlayerFbx::PlayerFbx(Input* input)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
}

void PlayerFbx::PlayerUpdate(double angleX, double angleY)
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

	angleX = angleX;
	angleY = angleY;



	//�����ꂽ��t���O��true�ɂ��o����̂���߂�
	dirty = true;


	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	//false�̊Ԃ͊p�x��ς�������
	if (dirty == true)
	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
		// �N�H�[�^�j�I�����g�p��������]�܂���
		matRot = matRotNew * matRot;



		//�x�N�g���ƍs��̐�

		move2 = XMVector3Transform(move2, matRot);
	}
	float dx1 = 0;
	float dz = 0;
	float dy = 0;
	// WASD��������Ă�����J��������s�ړ�������
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{

		if (input->PushKey(DIK_A))
		{
			dx1 -= 0.6f;
		}

		if (input->PushKey(DIK_D))
		{
			dx1 += 0.6f;
		}

		if (input->PushKey(DIK_W))
		{
			dz += 0.6f;
		}

		if (input->PushKey(DIK_S))
		{
			dz -= 0.6f;
		}


	}


	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		moveCamera.m128_f32[1] += fallV.m128_f32[1];
	}


	// �ڒn���
	/*w
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			// �s��̍X�V�Ȃ�
			Object3d::Update();
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// ���n
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			// �s��̍X�V�Ȃ�
			Object3d::Update();
		}
	}
	*/



	moveCamera = { dx1, fallV.m128_f32[1], dz, 0};
	moveCamera = XMVector3Transform(moveCamera, matRot);


	//���s�ړ�
	matTrans = XMMatrixTranslation(position.x += moveCamera.m128_f32[0], position.y += moveCamera.m128_f32[1]+fallV.m128_f32[1], position.z += moveCamera.m128_f32[2]);


	memory.m128_f32[0] += moveCamera.m128_f32[0];
	memory.m128_f32[1] += moveCamera.m128_f32[1];
	memory.m128_f32[2] += moveCamera.m128_f32[2];


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


