#pragma once

#include <DirectXMath.h>

class Fbx3d;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(Fbx3d* fbx, BaseCollider* collider, const DirectX::XMVECTOR& inter) {
		this->fbx = fbx;
		this->collider = collider;
		this->inter = inter;
	}

	// 衝突相手のオブジェクト
	Fbx3d* fbx = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
};

