#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Fbx3d.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(Fbx3d* fbx) {
		this->fbx3d = fbx;
	}

	inline Fbx3d* GetObject3d() {
		return fbx3d;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }
	
	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		fbx3d->OnCollision(info);
	}

protected:
	Fbx3d* fbx3d = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};

