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

	//Settter
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }

	//Gettter
	XMVECTOR GetMove() { return moveCamera; }

	XMFLOAT3 GetPosition() { return Warp2; }

	bool Getground() { return onGround; }

	bool GetredTeleport() { return redTeleport; }

private:

	//�N���X�̃|�C���^
	Input* input;//���̓N���X

	Physics* physics;//�����N���X


	//�ϐ�
	float angleX;//�p�xX

	float angleY;//�p�xY

	float Momentum;//

	bool onGround = true;//�n�ʂ̔���

	bool redTeleport = false;//�Ԃւ̓]��

	float acceleration_g = 9.81 / 60;//�����x

	float JumpVel = 2;//Jump�̏���

	XMVECTOR fallV;//������x�N�g��

	XMVECTOR moveCamera;//����x�N�g��

	XMVECTOR memory;//�L���p�ϐ�

	XMVECTOR Warp;//���[�v���邽�߂̈ړ��ϐ��iVector�j

	XMFLOAT3 Warp2;//���[�v���邽�߂̈ړ��ϐ��iFloat�j
};

