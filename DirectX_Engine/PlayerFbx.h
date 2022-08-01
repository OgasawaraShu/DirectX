#pragma once
#include "Fbx3d.h"
#include "Input.h"

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
private:
	Input* input;
	float angleX;
	float angleY;

};

