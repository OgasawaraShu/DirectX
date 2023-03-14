#include "PlayerFbx.h"
#include "../../Engine/Collision/CollisionManager.h"
#include "../../Engine/Collision/BaseCollider.h"
#include "../../Engine/Collision/CollisionAttribute.h"
#include <d3dcompiler.h>
#include "../../Engine/Collision/Collision.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

void PlayerFbx::Initialize2()
{

	//�ϐ�
	 WarpTime = 0;

	 TimeWarpF = true;

	 onGround = true;//�n�ʂ̔���

	 redTeleport = false;//�Ԃւ̓]��

	 blueTeleport = false;//�ւ̓]��

	 blueCollision = false;//�ւ̓]���t���b�O

	 redCollision = false;//�Ԃւ̓]���t���b�O

	 warpFlag = false;//true�ɂȂ�����warp�o����悤�ɂ���

	 warpFlag2 = false;//true�ɂȂ�����warp�o����悤�ɂ���

	 acceleration_g = 9.81 / 60;//�����x

	 JumpVel = 2;//Jump�̏���

	 WallCollision = false;

	 wark = false;

	 Exit = false;
	 a = 0;
}

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
	

	
	if(info.collider->attribute == 32&&WallCollision==false&& info.collider->attribute != 16)
	{
		WallCollision = true;
		ColisionPoint=info.inter;
	}
	else if (info.collider->attribute == 128 && WallCollision == false && info.collider->attribute != 16)
	{
		WallCollision = true;
		ColisionPoint = info.inter;
	}
	else if (info.collider->attribute == 512 && WallCollision == false && info.collider->attribute != 16)
	{
		WallCollision = true;
		ColisionPoint = info.inter;
	}

	if (info.collider->attribute == 256)
	{
		Exit = true;
	}
}

PlayerFbx::PlayerFbx(Input* input,Physics* physics)
	:Fbx3d(input)
{
	assert(input);

	this->input = input;
	this->physics = physics;
}

void PlayerFbx::PlayerUpdate(float angleX, float angleY)
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


	//�����_�Ǝ����̈ʒu�̃x�N�g���v�Z
	Vector.m128_f32[0] = Target.x - position.x;
	Vector.m128_f32[1] = Target.y - position.y;
	Vector.m128_f32[2] = Target.z - position.z;
	//�x�N�g���̐��K��
	Vector = XMVector3Normalize(Vector) * 2.0f;

	//atan�ŏe�g��������������Z�o
	rotation.y = atan2(Vector.m128_f32[0], Vector.m128_f32[2]) * 60;



	if (input->PushKey(DIK_B))
	{
		if (Tutorial_time < 180)Tutorial_time += 1;
	}


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
	else
	{
		FallCount = 0;
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


	Ray ray2;
	ray2.start = { position.x,position.y-5.0f,position.z,0 };
	ray2.dir = { 0,-1,0,0 };
	RaycastHit raycastHit2;

	//�v���[���̐ݒ�
    Plane plane;
    plane.normal = XMVectorSet(0, 1, 0, 0);
    plane.distance = 0.0f;

	//sphere�̐ݒ�
	Sphere sphere;
	sphere.center= { position.x,position.y-6,position.z,0 };
	sphere.redius = 1;
   
        
    XMVECTOR inter;
    float distance;
    bool hit = Collision::CheckRay2Plane(ray, plane, &distance, &inter);
	

	debugCheck = 0;
        
        //OBJ�̏�ɍڂ��Ă�������
	if (CollisionManager::GetInstance()->Spherecast(sphere, COLLISION_ATTR_WALL, &raycastHit2, 5)) {
		onGround = true;
	}
	else
	{
		debugCheck += 1;
	}

	if (CollisionManager::GetInstance()->Spherecast(sphere, COLLISION_ATTR_BLOCK, &raycastHit2, 5)) {
		onGround = true;
	}
	else
	{
		debugCheck += 1;
	}

	if (debugCheck >= 2)
	{
		onGround = false;
	}

	
	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_OBJ, &raycastHit, 5.0 * 2.0f + adsDistance)) {
			position.y -= (raycastHit.distance - 5.0 * 2.0f);	
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,5.0 * 2.0f)) {
			// ���n
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
}

void PlayerFbx::PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans)
{
	
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;


//�r���[�v���W�F�N�V�����s��
	const XMMATRIX& matViewProjection =
		redcamera->GetViewProjectionMatrix();
	//���b�V��t�����X�t�H�[��
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//�J�������W
	const XMFLOAT3& cameraPos = redcamera->GetEye();

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
	//���[�v���Ă��悭�Ȃ����烏�[�v����
	if (TimeWarpF == false)
	{
		WarpTime += 1;
	}

	if (WarpTime > 60)
	{
		WarpTime = 0;
		TimeWarpF = true;
	}

	//�Ԃɓ������Ă��ǂ������΂��Ă���ꍇ
	if (redCollision == true && warpFlag == true&&TimeWarpF==true && warpFlag2 == true)
	{

		//�n���ꂽ�ϐ��������Ĉʒu���ړ�
		Warp2.x = Warp.m128_f32[0];
		Warp2.y = Warp.m128_f32[1];
		Warp2.z = Warp.m128_f32[2];

		redTeleport = true;

		TimeWarpF = false;

		//�����x�N�g�����v�Z���ēK�؂ȕ����ɕϊ�
		VectorChange();
	}
	else if (redCollision == false)
	{
		redTeleport = false;
	}


	redCollision = false;

	//�ɓ������Ă��ǂ������΂��Ă���ꍇ
	if (blueCollision == true && warpFlag == true && TimeWarpF == true&&warpFlag2==true)
	{
		//�n���ꂽ�ʒu�ɑ�����Ĉړ�
		Warp2.x = Warpblue.m128_f32[0];
		Warp2.y = Warpblue.m128_f32[1];
		Warp2.z = Warpblue.m128_f32[2];

		blueTeleport = true;
		TimeWarpF = false;
		//�����x�N�g�����v�Z���ēK�؂ȕ����ɕϊ�
		VectorChange();
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
	moveCamera = { 0,0,0,0 };

	// WASD��������Ă�������s�ړ�������
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		if (onGround == true&&WallCollision==false&&Tutorial==false)
		{
			isPlay = true;
			wark = true;
			if (input->PushKey(DIK_A))
			{
				dx -= 0.3f;

				if(Tutorial_time<180)Tutorial_time += 1;
			}

			if (input->PushKey(DIK_D))
			{
				dx += 0.3f;
				
				if (Tutorial_time < 180)Tutorial_time += 1;
			}

			if (input->PushKey(DIK_W))
			{
				dz += 0.3f;
			
				if (Tutorial_time < 180)Tutorial_time += 1;
			}

			if (input->PushKey(DIK_S))
			{
				dz -= 0.3f;

				if (Tutorial_time < 180)Tutorial_time += 1;
			}
		}
			
	}
	else
	{
		isPlay = false;
		//�����Ă��Ȃ��̂Ȃ�t���O��false�ɂ���
		wark = false;
	}



	if (Tutorial_time >= 180)
	{
		Tutorial_walk = true;
	}

    //�ϐ��ɑ��
	moveCamera = { dx += fallV.m128_f32[0], dy += fallV.m128_f32[1], dz += fallV.m128_f32[2], 0 };
}

void PlayerFbx::CollisionAfter()
{
	//�R���W������false�ɂ���
	WallCollision = false;
}

void PlayerFbx::VectorChange()
{
	FallCount += 1;
	//portal�ɓ�������x�N�g����ϊ�����
	fallV.m128_f32[2] -= ((fallV.m128_f32[1]*FallCount) + fallV.m128_f32[2]);
}


