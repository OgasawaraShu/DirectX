#include "Portal.h"

void Portal::Initialize()
{
	//�� ��portal������
	Blue portal_b = { 0,0,0,0 };
	Red portal_r = { 0,0,0,0 };
}

void Portal::Update()
{                

}

void Portal::Teleport(float x, float y, float z, float x2, float y2, float z2, float rotate_x, float rotate_x2 , float rotate_y, float rotate_y2, float rotate_z, float rotate_z2)//1�����@�@2�����[�v��̍��W�i�I�����W�jrotate�͔��]
{
	x = x2;
	y = y2;
	z = z2;

	rotate_x = rotate_x2;
	rotate_y = rotate_y2;
	rotate_z = rotate_z2;
}
