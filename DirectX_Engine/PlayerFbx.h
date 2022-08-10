#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "Physics.h"

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

	PlayerFbx(Input*input);
	// ���̓N���X�̃|�C���^

	void PlayerUpdate(double angleX, double angleY);

	XMVECTOR GetMove() { return moveCamera; }
private:
	Input* input;
	float angleX;
	float angleY;

	bool onGround = false;


	XMVECTOR fallV;
	XMVECTOR moveCamera;
	XMVECTOR memory;
};

