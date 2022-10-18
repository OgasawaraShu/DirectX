#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "Physics.h"
#include "BulletFbx.h"


class PlayerFbx :
	public Fbx3d
{
public:

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	PlayerFbx(Input* input, Physics* physics);// ���̓N���X�̃|�C���^

	void Initialize_Bullet();

	void PlayerUpdate(double angleX, double angleY);//�X�V����

	void FallJump();//��������

	void Landing();//���n

	void RayCheck(float angleX, float angleY);

	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);//�s���X�V

	void WarpUpdate();//���[�v����

	void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);//�ǉ���]����

	void MoveMatrixUpdate(XMMATRIX matRot, XMMATRIX matTrans);//�L�[���͂ɂ�铮��

	//Settter
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }

	void SetMemo2(XMVECTOR Memo) { Warpblue = Memo; }

	void SetWarpFlag(bool flag) { warpFlag = flag; }

	void SetWarpFlag2(bool flag) { warpFlag2 = flag; }


	void SetCameraAxis(XMVECTOR z) { CammeraZAxis = z; }

	void SetPos(XMFLOAT3 pos) { position = pos; }

	void SetWall(bool a) { WallCollision = a; }

	//Gettter
	XMVECTOR GetMove() { return moveCamera; }

	XMFLOAT3 GetPosition() { return Warp2; }

	XMFLOAT3 GetMyPosition() { return position; }

	bool Getground() { return onGround; }

	bool GetredTeleport() { return redTeleport; }

	bool GetblueTeleport() { return blueTeleport; }

	float GetPos() { return position.x; }

	void SetScene(int scene_) { scene = scene_; }

	bool GetWall() { return WallCollision;}
private:

	//�N���X�̃|�C���^
	Input* input;//���̓N���X

	Physics* physics;//�����N���X


	//�ϐ�
	int WarpTime = 0;

	bool TimeWarpF = true;

	float angleX_;//�p�xX

	float angleY_;//�p�xY

	float Momentum;//

	int scene;

	bool onGround = true;//�n�ʂ̔���

	bool redTeleport = false;//�Ԃւ̓]��

	bool blueTeleport = false;//�ւ̓]��

	bool blueCollision = false;//�ւ̓]���t���b�O

	bool redCollision = false;//�Ԃւ̓]���t���b�O

	bool warpFlag = false;//true�ɂȂ�����warp�o����悤�ɂ���

	bool warpFlag2 = false;//true�ɂȂ�����warp�o����悤�ɂ���


	float acceleration_g = 9.81 / 60;//�����x

	float JumpVel = 2;//Jump�̏���

	bool WallCollision = false;

	XMVECTOR fallV;//������x�N�g��

	XMVECTOR moveCamera;//����x�N�g��

	XMVECTOR moveOld;

	XMVECTOR memory;//�L���p�ϐ�

	XMVECTOR Warp;//���[�v���邽�߂̈ړ��ϐ��iVector�j

	XMVECTOR Warpblue;//���[�v���邽�߂̈ړ��ϐ��iVector�j

	XMFLOAT3 Warp2;//���[�v���邽�߂̈ړ��ϐ��iFloat�j

	XMVECTOR CammeraZAxis;

	XMFLOAT3 OldPos;
};

