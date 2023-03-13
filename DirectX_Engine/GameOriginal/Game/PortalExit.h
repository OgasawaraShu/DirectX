#pragma once
#include "../../Engine/3d/RenderTexture.h"
#include "../../Engine/Input/Input.h"

class PortalExit:
	public RenderTexture
{

public:
	//ポインタ
	PortalExit(Input* input);
	//更新
    void ExitUpdate(float angleX, float angleY,float colorPattern);
	//広げる処理
	void OriginalUpdate(float angleX, float angleY);
	//追加の回転処理
	void AddMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);
	//matrix処理
	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans, float colorPattern);

	//Getter,Setter
	//弾の当たった位置をセット
	void GetExitPosition(XMFLOAT3 pos_) { position = pos_; }
	//自分の位置を渡す
	XMFLOAT3 GetMyPosition() { return position; }
	//回転を渡す
	XMFLOAT3 GetMyRotate() { return rotation; }
    //スケールを渡す
	XMFLOAT3 GetMyScale() { return scale; }
	//ワープの準備ができたか
	void GetFlag(bool flag) { WarpFlag = flag; }
	//
	XMMATRIX GetRot() { return a; }
	//球のポジションをもらう
	void SetMyposition(XMFLOAT3 pos_) { MyPosition = pos_; }

	void SetPortalFlag(bool flag) { PortalFlag = flag; }

	void SetRotateFlag(int flag) { Rotate_exit_flag = flag; }
private:
	//クラスのポインタ
	Input* input;//入力クラス
	//ワープできるかのフラグ
	bool WarpFlag;
	//ポジション
	XMFLOAT3 MyPosition;
	//１frame前のアングル
	XMFLOAT2 OldAngle;
	//出口の開き具合
	XMFLOAT3 ScaleExit;
	//
	XMMATRIX a;

	bool PortalFlag = false;

	int Rotate_exit_flag;
};

