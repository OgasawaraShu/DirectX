#pragma once
#include "Fbx3d.h"
#include "Input.h"

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

private:
	Input* input;
	float angleX;
	float angleY;

};
