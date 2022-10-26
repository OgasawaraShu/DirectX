#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "PlayerFbx.h"

class BulletFbx :
	public Fbx3d
{
public://関数


	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	BulletFbx(Input* input);// 入力クラスのポインタ

	void BlueBulletUpdate(double angleX, double angleY);//青の球更新

	void RedBulletUpdate(double angleX, double angleY);//赤の球更新

	void PostMatrixUpdate();

	///Getter,Setter
	void SetMove(XMVECTOR move) { move_ = move; }

	void SetPos(XMFLOAT3 pos) { position_ = pos; }

	XMVECTOR GetMemo2() { return memory; }

	XMVECTOR GetMemo() { return memory3; }

	float GetAngleX1() { return position.x; }

	float GetAngleY1() { return oldBlueY; }

	float GetAngleX2() { return oldRedX; }

	float GetAngleY2() { return oldRedY; }

	bool GetWarpFlag() { return warpFlag; }

	bool GetWarpFlag2() { return warpFlag2; }


	void SetCameraT(XMFLOAT3 z) { Target = z; }


private://変数

	Input* input;//入力クラスのポインタ

	int TriggerRe = 0;

	float angleX;//角度X

	float angleY;//角度Y

	float oldRedX;//赤の累計角度

	float oldRedY;//赤の累計角度

	float oldBlueX;//青の累計角度

	float oldBlueY;//青の累計角度

	bool reset_Blue = true;//球の座標リセット

	bool reset_Red = true;//球の座標リセット

	bool warpFlag = false;//両方の球が撃たれたらワープできるようにする

	bool warpFlag2 = false;//両方の球が撃たれたらワープできるようにする

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
