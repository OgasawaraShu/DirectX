#pragma once
#include <DirectXMath.h>

class Portal
{
public://�����o�֐�
	void Initialize();//������
	void Update();//�X�V
	void Teleport(float x, float y, float z, float x2, float y2, float z2, float rotate_x,float rotate_x2, float rotate_y, float rotate_y2, float rotate_z, float rotate_z2);//���[�v����

protected://�����o�ϐ�

	struct Blue {
		float x;
		float y;
		float z;
		int flag;
	};

	struct Red {
		float x;
		float y;
		float z;
		int flag;
	};
};

