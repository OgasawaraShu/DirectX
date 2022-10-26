#pragma once
#include "Fbx3d.h"
#include "Input.h"

class PortalExit:
	public Fbx3d
{

public:

	PortalExit(Input* input);// ���̓N���X�̃|�C���^

    void ExitUpdate(float angleX, float angleY);

	void OriginalUpdate(float angleX, float angleY);

	void AddMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);

	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);//�s���X�V

	//Getter,Setter
	void GetExitPosition(XMFLOAT3 pos_) { position = pos_; }

	void GetFlag(bool flag) { WarpFlag = flag; }
private:
	//�N���X�̃|�C���^
	Input* input;//���̓N���X

	bool WarpFlag;

	XMFLOAT2 OldAngle;

	XMFLOAT3 ScaleExit;
};

