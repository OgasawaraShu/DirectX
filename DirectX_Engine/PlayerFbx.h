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

	void Initialize_Bullet();

	PlayerFbx(Input*input, Physics* physics);
	// ���̓N���X�̃|�C���^

	void PlayerUpdate(double angleX, double angleY);

	XMVECTOR GetMove() { return moveCamera; }
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }
private:
	Input* input;
	Physics* physics;
	float angleX;
	float angleY;

	float Momentum;

	bool onGround = true;
	float acceleration_g = 9.81 / 60;//�����x
	float JumpVel = 2;

	XMVECTOR fallV;
	XMVECTOR moveCamera;
	XMVECTOR memory;

	XMVECTOR Warp;
	XMFLOAT3 Warp2;
};

