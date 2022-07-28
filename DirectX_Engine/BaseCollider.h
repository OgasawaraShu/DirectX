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

	friend class CollisionManager;
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

	/// <summary>
	/// 当たり判定属性をセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SetAttribute(unsigned short attribute) {
		this->attribute = attribute;
	}

	/// <summary>
	/// 当たり判定属性を追加
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void AddAttribute(unsigned short attribute) {
		this->attribute |= attribute;
	}

	/// <summary>
	/// 当たり判定属性を削除
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void RemoveAttribute(unsigned short attribute) {
		this->attribute &= !attribute;
	}



protected:
	unsigned short attribute= 0b1111111111111111;

	Fbx3d* fbx3d = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};

