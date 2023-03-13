#pragma once
#include "../../Engine/3d/Fbx3d.h"
#include "Physics.h"
#include "../../Engine/Input/Input.h"
class ObjFbx :
    public Fbx3d
{
public:
    void OnCollision(const CollisionInfo& info) override;
    //ObjのInitialize
    void ObjInitialize();
    //ポインタ
    ObjFbx(Input* input);
    //アップデート
    void ObjUpdate(float angleX, float angleY);
    //レイの当たり判定チェック
    void RayCheck();
    //Boxのアップデート
    void BoxObjUpdate(float angleX, float angleY);
    //Boxのレイチェック
    void BoxRayCheck();
    //重力の処理
    void ObjFall();
    //Matrixのアップデート
    void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
    //追加回転処理
    void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);

    //Setter
    //Z軸セット
    void SetCameraAxisZ(XMVECTOR z) { CammeraZAxis = z; }
    //posのセット
    void SetMyPosition(XMFLOAT3 pos) { MyPosition = pos; }
    //注視点のセット
    void SetTarget(XMFLOAT3 pos) { Target = pos; }
    //シーンのセット
    void SetScene(int scene_) { scene = scene_; }
    //OBJの取ったかの参照
    bool Getgetobj() { return cursorOn; }
    //ポータル撃ったかの参照1青
    void SetShotBlue(bool a) { Shot = a; }
    //ポータル撃ったかの参照２赤
    void SetShotRed(bool a) { Shot2 = a; }
    //歩いてるかのセット
    void SetWark(bool a) { wark = a; }
   
    void SetTutorial(bool Flag) { Tutorial = Flag; }

    bool GetTutorialGun() { return Tutorial_gun; }

    float GetRotationY() { return rotation.y; }
protected:
    //クラスのポインタ
    Input* input;//入力クラス
    //変数
    //取得したかのフラグ
    bool cursorOn = false;
    //取得したかのフラグ
    bool cursorOn2 = false;
    //撃ったか青
    bool Shot = false;
    //撃ったか赤
    bool Shot2 = false;
    //リコイル
    bool Recoile = false;
    //歩く揺れ
    bool Recoile2 = false;
    //追加の回転
    XMFLOAT3  AddRotate;
    //追加のポジション
    XMFLOAT3  AddPosition;
    //歩くかのフラグ
    bool wark;
    //あるかの動作のフラグ
    bool warkmove = true;
    //歩いてる時間
    int Wark_time = 0;
    //地面の判定
    bool onGround = true;//地面の判定
    //落ちるベクトル
    XMVECTOR fallV;

    bool Tutorial_gun = false;

    bool Tutorial = true;

    //シーン
    int scene;
    //カメラのZ軸
    XMVECTOR CammeraZAxis;
    //カメラの注視点
    XMFLOAT3 Target;
    //自分のポジション
    XMFLOAT3 MyPosition;
    //Vector
    XMVECTOR Vector;
    //angleの累計Y
    float oldangleY;
    //angleの累計X
    float oldangleX;
};

