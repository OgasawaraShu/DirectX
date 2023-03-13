#pragma once
#include "../../Engine/3d/Fbx3d.h"
#include "../../Engine/Input/Input.h"
#include "PlayerFbx.h"

class BulletFbx :
	public Fbx3d
{
public://関数


	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	/// 
	void OnCollision(const CollisionInfo& info) override;
	//初期化
	void Initialize2();
	//ポインタ
	BulletFbx(Input* input);// 入力クラスのポインタ

	void BlueBulletUpdate(float angleX, float angleY);//青の球更新

	void RedBulletUpdate(float angleX, float angleY);//赤の球更新
	//マトリックスのアップデート
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

private://変数

	Input* input;//入力クラスのポインタ

	int TriggerRe = 0;

	float angleX;//角度X

	float angleY;//角度Y

	float oldRedX;//赤の累計角度

	float oldRedY;//赤の累計角度

	float oldBlueX;//青の累計角度

	float oldBlueY;//青の累計角度

	XMFLOAT2 memoB;

	XMFLOAT2 memoR;

	bool reset_Blue = true;//球の座標リセット

	bool reset_Red = true;//球の座標リセット

	bool warpFlag = false;//両方の球が撃たれたらワープできるようにする

	bool warpFlag2 = false;//両方の球が撃たれたらワープできるようにする

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
	XMVECTOR move_;//自機の座標

	XMVECTOR memory;//角度の保存

	XMVECTOR memory3;//角度の保存

	XMFLOAT3 position_ = position;

	XMFLOAT3 Target;

	XMFLOAT3 Target_;

	XMVECTOR Vector;
};
