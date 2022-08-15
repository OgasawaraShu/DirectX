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
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	PlayerFbx(Input* input, Physics* physics);// 入力クラスのポインタ

	void Initialize_Bullet();

	void PlayerUpdate(double angleX, double angleY);//更新処理

	//Settter
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }

	//Gettter
	XMVECTOR GetMove() { return moveCamera; }

	XMFLOAT3 GetPosition() { return Warp2; }

	bool Getground() { return onGround; }

	bool GetredTeleport() { return redTeleport; }

private:

	//クラスのポインタ
	Input* input;//入力クラス

	Physics* physics;//物理クラス


	//変数
	float angleX;//角度X

	float angleY;//角度Y

	float Momentum;//

	bool onGround = true;//地面の判定

	bool redTeleport = false;//赤への転送

	float acceleration_g = 9.81 / 60;//加速度

	float JumpVel = 2;//Jumpの初速

	XMVECTOR fallV;//落ちるベクトル

	XMVECTOR moveCamera;//動作ベクトル

	XMVECTOR memory;//記憶用変数

	XMVECTOR Warp;//ワープするための移動変数（Vector）

	XMFLOAT3 Warp2;//ワープするための移動変数（Float）
};

