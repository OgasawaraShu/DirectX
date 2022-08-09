#include "Portal.h"

void Portal::Initialize()
{
	//ê‘ ëìportalèâä˙âª
	Blue portal_b = { 0,0,0,0 };
	Red portal_r = { 0,0,0,0 };
}

void Portal::Update()
{                

}

void Portal::Teleport(DirectX::XMVECTOR m1, DirectX::XMVECTOR m2, float rotate_x, float rotate_x2, float rotate_y, float rotate_y2, float rotate_z, float rotate_z2)
{
	m1 = m2;

	rotate_x = rotate_x2;
	rotate_y = rotate_y2;
	rotate_z = rotate_z2;
}

