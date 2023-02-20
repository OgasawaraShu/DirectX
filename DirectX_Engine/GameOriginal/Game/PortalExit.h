#pragma once
#include "../../Engine/3d/Fbx3d.h"
#include "../../Engine/Input/Input.h"

class PortalExit:
	public Fbx3d
{

public:
	//�|�C���^
	PortalExit(Input* input);
	//�X�V
    void ExitUpdate(float angleX, float angleY);
	//�L���鏈��
	void OriginalUpdate(float angleX, float angleY);
	//�ǉ��̉�]����
	void AddMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);
	//matrix����
	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);

	//Getter,Setter
	//�e�̓��������ʒu���Z�b�g
	void GetExitPosition(XMFLOAT3 pos_) { position = pos_; }
	//�����̈ʒu��n��
	XMFLOAT3 GetMyPosition() { return position; }
	//��]��n��
	XMFLOAT3 GetMyRotate() { return rotation; }
    //�X�P�[����n��
	XMFLOAT3 GetMyScale() { return scale; }
	//���[�v�̏������ł�����
	void GetFlag(bool flag) { WarpFlag = flag; }
	//
	XMMATRIX GetRot() { return a; }
	//���̃|�W�V���������炤
	void SetMyposition(XMFLOAT3 pos_) { MyPosition = pos_; }
private:
	//�N���X�̃|�C���^
	Input* input;//���̓N���X
	//���[�v�ł��邩�̃t���O
	bool WarpFlag;
	//�|�W�V����
	XMFLOAT3 MyPosition;
	//�Pframe�O�̃A���O��
	XMFLOAT2 OldAngle;
	//�o���̊J���
	XMFLOAT3 ScaleExit;
	//
	XMMATRIX a;
};
