#include "WallColider.h"
using namespace DirectX;

void WallCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX& matWorld = fbx3d->GetMatWorld();

	// ���̃����o�ϐ����X�V
	Wall::minPos = matWorld.r[3] - offset;
	Wall::maxPos = matWorld.r[3] + offset;
}
