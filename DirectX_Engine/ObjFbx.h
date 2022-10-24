#pragma once
#include "Fbx3d.h"
#include "Physics.h"
#include "Input.h"
class ObjFbx :
    public Fbx3d
{
public:
    ObjFbx(Input* input, Physics* physics);// ポインタ
    void ObjUpdate(float angleX, float angleY);
    void RayCheck();
    void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
    void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);//追加回転処理




    void SetCameraAxisZ(XMVECTOR z) { CammeraZAxis = z; }
    void SetMyPosition(XMFLOAT3 pos) { MyPosition = pos; }
    void SetTarget(XMFLOAT3 pos) { Target = pos; }
    void SetScene(int scene_) { scene = scene_; }
   // void SetCameraT(XMFLOAT3 z) { Target = z; }

private:
    //クラスのポインタ
    Input* input;//入力クラス
    Physics* physics;//物理クラス

    //変数
    bool cursorOn = false;
    int scene;
    XMVECTOR CammeraZAxis;
    XMFLOAT3 Target;
    XMFLOAT3 MyPosition;
    XMVECTOR Vector;

    float oldangleY;
    float oldangleX;
};

