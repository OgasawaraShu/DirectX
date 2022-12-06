#include "PlayerFbx.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include <d3dcompiler.h>
#include "PortalfallV.h"
#include "Collision.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void PlayerFbx::OnCollision(const CollisionInfo& info)
{
	//2�i�@�ŕ\�����Ă��邽��4�͐ԋ�8�͑���
	if(info.collider->attribute==4&&warpFlag==true)
	{
		blueCollision = true;
	}
	else if (info.collider->attribute == 8 && warpFlag == true)
	{
		redCollision = true;
	}
	
	
	if(info.collider->attribute == 32&&WallCollision==false)
	{
		WallCollision = true;
		ColisionPoint=info.inter;
	}
	else if(info.collider->attribute == 64 && WallCollision == false)
	{
		WallCollision = true;
		ColisionPoint = info.inter;
	}
	else if (info.collider->attribute == 128 && WallCollision == false)
	{
		WallCollision = true;
		ColisionPoint = info.inter;
	}

	if (info.collider->attribute == 256)
	{
		Exit = true;
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

	//�O�̍��W���L�^
	OldPos = position;
	// �}�E�X�̓��͂��擾
	Input::MouseMove mouseMove = input->GetMouseMove();

	//�p�b�h�̃|�C���^
	GamePad* GP = nullptr;
	GP = new GamePad();

	//�p�b�h�̍X�V
	GP->Update();
	//�}�E�X�p�x�o��Ver
	angleX_ = angleX;
	angleY_ = angleY;
	
	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//���s�ړ�
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//�ǉ��̉�]�s�񏈗�
	AddRotateMatrixUpdate(angleX_, angleY_, matRot);

    Landing();
	//���[�v����
	WarpUpdate();
	//Jump�Ɨ����̏���
	FallJump();
	//��������
	MoveMatrixUpdate(matRot,matTrans);
	//Matrix��X�V
	PostMatrixUpdate(matScale, matRot, matTrans);
	//
	//RayCheck(angleX, angleY);

}

void PlayerFbx::FallJump()
{
	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	}
	// �W�����v����
	else if (input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 6.0f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}
}

void PlayerFbx::Landing()
{
	// ���C�̃X�^�[�g�n�_��ݒ�
	Ray ray;
	ray.start = { position.x,position.y+1.0f,position.z,0 };
	ray.start.m128_f32[1] += 5.0f;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	
    Plane plane;

    plane.normal = XMVectorSet(0, 1, 0, 0);
    plane.distance = 0.0f;
   
        
        XMVECTOR inter;
        float distance;
        bool hit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);
	
        if (hit&&distance<=5.0f) {

			onGround = true;
        }
		else
		{
			onGround = false;
		}
        

	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit, 5.0 * 2.0f + adsDistance)) {
			//onGround = false;
			position.y -= (raycastHit.distance - 5.0 * 2.0f);	
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
		//	onGround = true;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,5.0 * 2.0f)) {
			// ���n
		//	onGround = true;
			position.y -= (raycastHit.distance - 5.0 * 2.0f);
		}
	}
}

void PlayerFbx::RayCheck(float angleX, float angleY)
{
	// ���C�̃X�^�[�g�n�_��ݒ�
	Ray ray;
	ray.start = { position.x,position.y,position.z,0 };
	ray.start.m128_f32[1] += 2.5f;
	ray.dir = { 0,0,1,0};
	RaycastHit raycastHit;

	XMMATRIX matRot;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	// �ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX);
	matRotNew *= XMMatrixRotationY(-angleY);
	// �ݐς̉�]�s�������
	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	// �N�H�[�^�j�I�����g�p��������]�܂���
	matRot = matRotNew * matRot;

	ray.dir = CammeraZAxis;


	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit)) {
		//onGround = false;
	}
}

void PlayerFbx::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
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

void PlayerFbx::WarpUpdate()
{
	if (TimeWarpF == false)
	{
		WarpTime += 1;
	}

	if (WarpTime > 120)
	{
		WarpTime = 0;
		TimeWarpF = true;
	}

	//�Ԃɓ������Ă��ǂ������΂��Ă���ꍇ
	if (redCollision == true && warpFlag == true&&TimeWarpF==true && warpFlag2 == true)
	{

		Warp2.x = Warp.m128_f32[0];
		Warp2.y = Warp.m128_f32[1];
		Warp2.z = Warp.m128_f32[2];


		//position = Warp2;

		redTeleport = true;

		//onGround = false;
		TimeWarpF = false;
	}
	else if (redCollision == false)
	{
		redTeleport = false;
	}


	redCollision = false;

	//�ɓ������Ă��ǂ������΂��Ă���ꍇ
	if (blueCollision == true && warpFlag == true && TimeWarpF == true&&warpFlag2==true)
	{

		Warp2.x = Warpblue.m128_f32[0];
		Warp2.y = Warpblue.m128_f32[1];
		Warp2.z = Warpblue.m128_f32[2];

		blueTeleport = true;
		TimeWarpF = false;
	}
	else if (blueCollision == false)
	{
		blueTeleport = false;
	}

	blueCollision = false;
}

void PlayerFbx::AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot)
{
	// �ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationX(-angleX);
	matRotNew *= XMMatrixRotationY(-angleY);
	// �ݐς̉�]�s�������
	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	// �N�H�[�^�j�I�����g�p��������]�܂���
	matRot = matRotNew * matRot;

}

void PlayerFbx::MoveMatrixUpdate(XMMATRIX matRot,XMMATRIX matTrans)
{
	float dx = 0;
	float dz = 0;
	float dy = 0;

//	wark = false;
	// WASD��������Ă�������s�ړ�������
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		if (onGround == true&&WallCollision==false)
		{
			wark = true;
			if (input->PushKey(DIK_A))
			{
				dx -= 0.3f;
			}

			if (input->PushKey(DIK_D))
			{
				dx += 0.3f;
				
			}

			if (input->PushKey(DIK_W))
			{
				dz += 0.3f;
			
			}

			if (input->PushKey(DIK_S))
			{
				dz -= 0.3f;
			}
		}
			
	}
	else
	{
		wark = false;
	}

	//moveCamera.m128_f32[0] = 0;
	//moveCamera.m128_f32[2] = 0;
	

	moveCamera = { dx += fallV.m128_f32[0], dy += fallV.m128_f32[1], dz += fallV.m128_f32[2], 0 };

	if (WallCollision==false)
	{
	//	moveCamera = { dx += fallV.m128_f32[0], dy += fallV.m128_f32[1], dz += fallV.m128_f32[2], 0 };
	//	moveOld_ = moveOld;

	//	ColOld = false;
	//	ColOld2 = false;
	}
	else
	{
		
		//if (moveOld_.m128_f32[0] != 0.00f || moveOld_.m128_f32[2] != 0.00f)
		//{
			//if (ColOld == false)
			//{
			//	moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
			//	moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);


			//}
	//	}
		
		/*

		if (ColOld == true)
		{
			
		//	moveCamera.m128_f32[0] = -1.0f * (moveOld.m128_f32[0]);
		//	moveCamera.m128_f32[2] = -1.0f * (moveOld.m128_f32[2]);
			if (moveOld.m128_f32[3] > 0&&ColOld2==false)
			{
				moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
				moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);
			}
			else if(moveOld.m128_f32[3] <= 0 && ColOld2 == false)
			{
				moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
				moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);
			}
			ColOld2 = true;

		}
		
		if (moveOld.m128_f32[0] == 0.00f || moveOld.m128_f32[2] == 0.00f)
		{
			if (moveOld_.m128_f32[0] <= 0)
			{
				moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
				moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);
			}
			else
			{
				if (moveOld_.m128_f32[2] < 0)
				{
					moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
					moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);

					if (ColisionPoint.m128_f32[2]>position.z)
					{
						moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
						moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);
					}

					if (ColisionPoint.m128_f32[0] < position.x)
					{
					
						moveCamera.m128_f32[0] = 1.0f * (moveOld_.m128_f32[0]);
						moveCamera.m128_f32[2] = 1.0f * (moveOld_.m128_f32[2]);

						if (moveOld.m128_f32[2] < 0)
						{
							moveCamera.m128_f32[0] = -1.0f * (moveOld_.m128_f32[0]);
							moveCamera.m128_f32[2] = -1.0f * (moveOld_.m128_f32[2]);
						}
					}
				}
				
			}
		}
		*/
		ColOld=true;
		//WallCollision = false;
	}

	
	//moveCamera = XMVector3Transform(moveCamera, matRot);
}

void PlayerFbx::CollisionAfter()
{
	WallCollision = false;
}


