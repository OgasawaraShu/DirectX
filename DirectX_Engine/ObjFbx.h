#pragma once
#include "Fbx3d.h"
#include "Physics.h"
#include "Input.h"
class ObjFbx :
    public Fbx3d
{
public:
    void Initialize2();
    ObjFbx(Input* input, Physics* physics);// �|�C���^
    void ObjUpdate(float angleX, float angleY);
    void RayCheck();
    void BoxObjUpdate(float angleX, float angleY);
    void BoxRayCheck();
    void ObjFall();
    void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
    void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);//�ǉ���]����




    void SetCameraAxisZ(XMVECTOR z) { CammeraZAxis = z; }
    void SetMyPosition(XMFLOAT3 pos) { MyPosition = pos; }
    void SetTarget(XMFLOAT3 pos) { Target = pos; }
    void SetScene(int scene_) { scene = scene_; }
    bool Getgetobj() { return cursorOn; }
    void SetShot(bool a) { Shot = a; }
    void SetShot2(bool a) { Shot2 = a; }
    void SetWark(bool a) { wark = a; }
   // void SetCameraT(XMFLOAT3 z) { Target = z; }

private:
    //�N���X�̃|�C���^
    Input* input;//���̓N���X
    Physics* physics;//�����N���X

    //�ϐ�
    bool cursorOn = false;
    bool cursorOn2 = false;

    bool Shot = false;
    bool Shot2 = false;
    bool Recoile = false;
    bool Recoile2 = false;
    XMFLOAT3  AddRotate;
    XMFLOAT3  AddPosition;


    bool wark;
    bool warkmove = true;
    int warktime = 0;
    bool onGround = true;//�n�ʂ̔���
    XMVECTOR fallV;//������x�N�g��

    int scene;
    XMVECTOR CammeraZAxis;
    XMFLOAT3 Target;
    XMFLOAT3 MyPosition;
    XMVECTOR Vector;

    float oldangleY;
    float oldangleX;
};

