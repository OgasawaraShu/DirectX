#pragma once
//#include "BaseCollider.h"
#include <DirectXMath.h>

class Fbx3d;
class BaseCollider;

/// <summary>
/// レイキャストによる情報を得る為の構造体
/// </summary>

struct RaycastHit
{
	// 衝突相手のオブジェクト
	Fbx3d* fbx = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
	// 衝突点までの距離
	float distance = 0.0f;
};

