#pragma once
#include "Fbx3d.h"
#include "Input.h"
#include "Physics.h"
#include "BulletFbx.h"


class PlayerFbx :
	public Fbx3d
{
public:
	void Initialize2();
	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	PlayerFbx(Input* input, Physics* physics);// 入力クラスのポインタ

	void Initialize_Bullet();

	void PlayerUpdate(float angleX,float angleY);//更新処理

	void FallJump();//落下処理

	void Landing();//着地

	void RayCheck(float angleX, float angleY);

	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);//行列後更新

	void WarpUpdate();//ワープ処理

	void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);//追加回転処理

	void MoveMatrixUpdate(XMMATRIX matRot, XMMATRIX matTrans);//キー入力による動作

	void CollisionAfter();

	void VectorChange();

	//Settter
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }

	void SetMemo2(XMVECTOR Memo) { Warpblue = Memo; }

	void SetWarpFlag(bool flag) { warpFlag = flag; }

	void SetWarpFlag2(bool flag) { warpFlag2 = flag; }


	void SetCameraAxis(XMVECTOR z) { CammeraZAxis = z; }

	void SetPos(XMFLOAT3 pos) { position = pos; }

	void SetWall(bool a) { WallCollision = a; }

	//Gettter
	XMVECTOR GetMove() { return moveCamera; }

	XMFLOAT3 GetPosition() { return Warp2; }

	XMFLOAT3 GetMyPosition() { return position; }

	bool Getground() { return onGround; }

	bool GetredTeleport() { return redTeleport; }

	bool GetblueTeleport() { return blueTeleport; }

	float GetPos() { return position.x; }

	void SetScene(int scene_) { scene = scene_; }

	bool GetWall() { return WallCollision;}

	bool GetWark() { return wark; }

	bool GetExit() { return Exit; }

	int aGet() { return a; }

	bool GetDebug() { return debugCheck; }

	bool GetColision() { return WallCollision; }

	XMVECTOR GetColisionVec() { return ColisionPoint; }

	void GetmoveOld(XMVECTOR a) { moveOld = a; }
private:

	//クラスのポインタ
	Input* input;//入力クラス

	Physics* physics;//物理クラス


	//変数
	bool debugCheck = false;

	int WarpTime = 0;

	bool TimeWarpF = true;

	float angleX_;//角度X

	float angleY_;//角度Y

	float Momentum;//

	int scene;

	bool onGround = true;//地面の判定

	bool redTeleport = false;//赤への転送

	bool blueTeleport = false;//青への転送

	bool blueCollision = false;//青への転送フラッグ

	bool redCollision = false;//赤への転送フラッグ

	bool warpFlag = false;//trueになったらwarp出来るようにする

	bool warpFlag2 = false;//trueになったらwarp出来るようにする

	bool ColOld;

	bool ColOld2;



	float acceleration_g = 9.81 / 60;//加速度

	float JumpVel = 2;//Jumpの初速

	bool WallCollision = false;

	bool wark = false;

	bool Exit = false;
	int a = 0;

	XMVECTOR fallV = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//落ちるベクトル

	XMVECTOR moveCamera=DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//動作ベクトル

	XMVECTOR moveOld = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;

	XMVECTOR moveOld_ = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;


	XMVECTOR ColisionPoint = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;

	XMVECTOR Vector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;

	XMVECTOR memory = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//記憶用変数

	XMVECTOR Warp = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//ワープするための移動変数（Vector）

	XMVECTOR Warpblue = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//ワープするための移動変数（Vector）

	XMFLOAT3 Warp2;//ワープするための移動変数（Float）

	XMVECTOR CammeraZAxis = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;

	XMFLOAT3 OldPos;

	XMMATRIX OldMatrot;


};

