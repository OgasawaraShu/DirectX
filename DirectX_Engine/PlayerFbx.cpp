#include "PlayerFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void PlayerFbx::OnCollision(const CollisionInfo& info)
{
	//2�i�@�ŕ\�����Ă��邽��4�͐ԋ�8�͑���
	if(info.collider->attribute==4&&warpFlag==true)
	{
		//blueCollision = true;
	}
	else if (info.collider->attribute == 8 && warpFlag == true)
	{
		redCollision = true;
	}
	else
	{
		//onGround = true;
	}
}

void PlayerFbx::Initialize_Bullet()
{

}

PlayerFbx::PlayerFbx(Input* input,Physics* physics)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
	this->physics = physics;
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

	// WASD��������Ă�������s�ړ�������
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

	
	//�Ԃɓ������Ă��ǂ������΂��Ă���ꍇ
	if (redCollision==true && warpFlag == true)
	{
		
		Warp2.x = Warp.m128_f32[0];
		Warp2.y = Warp.m128_f32[1];
		Warp2.z = Warp.m128_f32[2];

		
		position = Warp2;

		redTeleport = true;

		onGround = false;
		//fallV.m128_f32[2] = 2.0f;

	}
	else if(redCollision == false)
	{
		redTeleport = false;
	}


	redCollision = false;

	//�ɓ������Ă��ǂ������΂��Ă���ꍇ
	if (blueCollision==true && warpFlag == true)
	{

		Warp2.x = Warpblue.m128_f32[0];
		Warp2.y = Warpblue.m128_f32[1];
		Warp2.z = Warpblue.m128_f32[2];


		position = Warp2;

		blueTeleport = true;
	}
	else if (blueCollision == false)
	{
		blueTeleport = false;
	}

	blueCollision = false;
	
	//onGround��true�Ȃ痎���x�N�g����0
	//false�Ȃ珙�X�ɏd�͂ɏ]���ĉ�������
	if (onGround != true)
	{
		fallV.m128_f32[1] = physics->Gravity(0, fallV.m128_f32[1]);
	}
	else
	{
		fallV.m128_f32[1] = 0;
	}

	moveCamera = { dx1+=fallV.m128_f32[0], dy+=fallV.m128_f32[1], dz+=fallV.m128_f32[2], 0};

	moveCamera = XMVector3Transform(moveCamera, matRot);
	
	//position��Vector�𑫂�
	position.x += moveCamera.m128_f32[0];
	position.y += moveCamera.m128_f32[1];
	position.z += moveCamera.m128_f32[2];



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

void PlayerFbx::Fall()
{
	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	// �W�����v����
	else if (input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}
}

void PlayerFbx::Landing()
{

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);	
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
		}
	}
}


