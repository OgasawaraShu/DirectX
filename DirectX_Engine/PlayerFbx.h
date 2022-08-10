#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "Physics.h"

class PlayerFbx :
	public Fbx3d
{
public:


	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	void Initialize_Bullet();

	PlayerFbx(Input*input);
	// 入力クラスのポインタ

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

