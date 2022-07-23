#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class PlaneCollider : public BaseCollider, public Plane
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	PlaneCollider(XMVECTOR offset = { 0,0,0,0 }, float distance = 0.0f) :
		offset(offset),
		distance(distance)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_PLANE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	inline void SetDistance(float distance) { this->distance = distance; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float distance;
};

