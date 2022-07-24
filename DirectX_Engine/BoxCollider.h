#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class BoxCollider : public BaseCollider, public Box
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	BoxCollider(XMVECTOR offset = { 0,0,0,0 }) :
		offset(offset)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_BOX;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	inline void SetX(float x) { this->x = x; }
	inline void SetY(float y) { this->y = y; }
	inline void SetZ(float z) { this->z = z; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 中心点からの最大値最小値参照
    float x;
	float y;
	float z;
};

