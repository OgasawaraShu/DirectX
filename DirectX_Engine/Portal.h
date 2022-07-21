#pragma once
#include <DirectXMath.h>

class Portal
{
public://メンバ関数
	void Initialize();//初期化
	void Update();//更新
	void Teleport(float x, float y, float z, float x2, float y2, float z2, float rotate_x,float rotate_x2, float rotate_y, float rotate_y2, float rotate_z, float rotate_z2);//ワープ処理

protected://メンバ変数

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

