#include "BoxCollider.h"
using namespace DirectX;

void BoxCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX& matWorld = fbx3d->GetMatWorld();

	// 箱のメンバ変数を更新
	Box::minPos = matWorld.r[3] - offset;
	Box::maxPos = matWorld.r[3] + offset;
}
