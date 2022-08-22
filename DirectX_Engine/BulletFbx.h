#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "PlayerFbx.h"

class BulletFbx :
	public Fbx3d
{
public://�֐�


	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	BulletFbx(Input* input);// ���̓N���X�̃|�C���^

	void BlueBulletUpdate(double angleX, double angleY);//�̋��X�V

	void RedBulletUpdate(double angleX, double angleY);//�Ԃ̋��X�V

	void PostMatrixUpdate();

	///Getter,Setter
	void SetMove(XMVECTOR move) { move_ = move; }

	XMVECTOR GetMemo2() { return memory; }

	XMVECTOR GetMemo() { return memory3; }

	float GetAngleX1() { return oldBlueX; }

	float GetAngleY1() { return oldBlueY; }

	float GetAngleX2() { return oldRedX; }

	float GetAngleY2() { return oldRedY; }

	bool GetWarpFlag() { return warpFlag; }


private://�ϐ�

	Input* input;//���̓N���X�̃|�C���^

	float angleX;//�p�xX

	float angleY;//�p�xY

	float oldRedX;//�Ԃ̗݌v�p�x

	float oldRedY;//�Ԃ̗݌v�p�x

	float oldBlueX;//�̗݌v�p�x

	float oldBlueY;//�̗݌v�p�x

	bool reset_Blue = true;//���̍��W���Z�b�g

	bool reset_Red = true;//���̍��W���Z�b�g

	bool warpFlag = false;//�����̋��������ꂽ�烏�[�v�ł���悤�ɂ���

	XMVECTOR move_;//���@�̍��W

	XMVECTOR memory;//�p�x�̕ۑ�

	XMVECTOR memory3;//�p�x�̕ۑ�
};
