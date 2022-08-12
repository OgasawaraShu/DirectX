#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "PlayerFbx.h"

class BulletFbx :
	public Fbx3d
{
public:


	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	BulletFbx(Input* input);
	// ���̓N���X�̃|�C���^

	void BlueBulletUpdate(double angleX, double angleY);
	void RedBulletUpdate(double angleX, double angleY);

	void SetMove(XMVECTOR move) { move_ = move; }

	XMVECTOR GetMemo() { return memory3; }


private:
	Input* input;
	float angleX;
	float angleY;
	XMVECTOR move_;

	XMVECTOR memory;
	XMVECTOR memory3;
};
