#pragma once
#include "Fbx3d.h"
#include "Physics.h"
#include "Input.h"
class ObjFbx :
    public Fbx3d
{
public:
    void Initialize2();
    ObjFbx(Input* input, Physics* physics);// ポインタ
    void ObjUpdate(float angleX, float angleY);
    void RayCheck();
    void BoxObjUpdate(float angleX, float angleY);
    void BoxRayCheck();
    void ObjFall();
    void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
    void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);//追加回転処理




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
    //クラスのポインタ
    Input* input;//入力クラス
    Physics* physics;//物理クラス

    //変数
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
    bool onGround = true;//地面の判定
    XMVECTOR fallV;//落ちるベクトル

    int scene;
    XMVECTOR CammeraZAxis;
    XMFLOAT3 Target;
    XMFLOAT3 MyPosition;
    XMVECTOR Vector;

    float oldangleY;
    float oldangleX;
};

