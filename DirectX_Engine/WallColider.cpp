#include "WallColider.h"
using namespace DirectX;

void WallCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX& matWorld = fbx3d->GetMatWorld();

	// 箱のメンバ変数を更新
	Wall::minPos = matWorld.r[3] - offset;
	Wall::maxPos = matWorld.r[3] + offset;
}
