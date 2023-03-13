#pragma once
#include "../../Engine/3d/Fbx3d.h"
#include "../../Engine/Input/Input.h"
#include "PlayerFbx.h"

class BulletFbx :
	public Fbx3d
{
public://�֐�


	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	/// 
	void OnCollision(const CollisionInfo& info) override;
	//������
	void Initialize2();
	//�|�C���^
	BulletFbx(Input* input);// ���̓N���X�̃|�C���^

	void BlueBulletUpdate(float angleX, float angleY);//�̋��X�V

	void RedBulletUpdate(float angleX, float angleY);//�Ԃ̋��X�V
	//�}�g���b�N�X�̃A�b�v�f�[�g
	void PostMatrixUpdate();

	//Setter
	//move
	void SetMove(XMVECTOR move) { move_ = move; }
	//pos
	void SetPos(XMFLOAT3 pos) { position_ = pos; }
	//Getter
	//
	XMVECTOR GetMemo2() { return memory; }

	XMVECTOR GetMemo() { return memory3; }

	float GetAngleX1() { return position.x; }

	float GetAngleY1() { return oldBlueY; }

	float GetAngleX2() { return oldRedX; }

	float GetAngleY2() { return oldRedY; }

	bool GetWarpFlag() { return warpFlag; }

	bool GetWarpFlag2() { return warpFlag2; }


	void SetCameraT(XMFLOAT3 z) { Target = z; }

	void SetGet(bool get) { getflag = get; }

	bool RedP_Flag() { return RedCollision; }
	bool RedP_Attack() { return RedAttack; }
	
	bool BlueP_Flag() { return BlueCollision; }
	bool BlueP_Attack() { return BlueAttack; }

	bool GetShot() { return ShotFlag; }
	bool GetShot2() { return ShotFlag2; }

	int GetCollisionFbxFlag() { return Collision_fbx_flag; }

	XMFLOAT2 GetMemoR() { return memoR; }
	XMFLOAT2 GetMemoB() { return memoB; }

private://�ϐ�

	Input* input;//���̓N���X�̃|�C���^

	int TriggerRe = 0;

	float angleX;//�p�xX

	float angleY;//�p�xY

	float oldRedX;//�Ԃ̗݌v�p�x

	float oldRedY;//�Ԃ̗݌v�p�x

	float oldBlueX;//�̗݌v�p�x

	float oldBlueY;//�̗݌v�p�x

	XMFLOAT2 memoB;

	XMFLOAT2 memoR;

	bool reset_Blue = true;//���̍��W���Z�b�g

	bool reset_Red = true;//���̍��W���Z�b�g

	bool warpFlag = false;//�����̋��������ꂽ�烏�[�v�ł���悤�ɂ���

	bool warpFlag2 = false;//�����̋��������ꂽ�烏�[�v�ł���悤�ɂ���

	bool getflag;

	bool RedCollision = false;

	bool RedAttack = false;

	bool BlueCollision = false;

	bool ShotFlag = false;
	bool ShotFlag2 = false;


	bool BlueAttack = false;

	int Collision_fbx_flag;

	float oldx2;

	float oldy2;
	XMVECTOR move_;//���@�̍��W

	XMVECTOR memory;//�p�x�̕ۑ�

	XMVECTOR memory3;//�p�x�̕ۑ�

	XMFLOAT3 position_ = position;

	XMFLOAT3 Target;

	XMFLOAT3 Target_;

	XMVECTOR Vector;
};
