#pragma once
#include "Fbx3d.h"
#include "Input.h"

class PortalExit:
	public Fbx3d
{

public:

	PortalExit(Input* input);// 入力クラスのポインタ

    void ExitUpdate(float angleX, float angleY);

	void OriginalUpdate(float angleX, float angleY);

	void AddMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);

	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);//行列後更新

	//Getter,Setter
	void GetExitPosition(XMFLOAT3 pos_) { position = pos_; }

	XMFLOAT3 GetMyPosition() { return position; }

	XMFLOAT3 GetMyRotate() { return rotation; }


	XMFLOAT3 GetMyScale() { return scale; }
	void GetFlag(bool flag) { WarpFlag = flag; }

	XMMATRIX GetRot() { return a; }

	void SetMyposition(XMFLOAT3 pos_) { MyPosition = pos_; }
private:
	//クラスのポインタ
	Input* input;//入力クラス

	bool WarpFlag;

	XMFLOAT3 MyPosition;

	XMFLOAT2 OldAngle;

	XMFLOAT3 ScaleExit;

	XMMATRIX a;
};

