#pragma once
#include "../../Engine/3d/Fbx3d.h"
#include "../../Engine/Input/Input.h"
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
	//ポインタ
	PlayerFbx(Input* input, Physics* physics);
	//アップデート
	void PlayerUpdate(float angleX,float angleY);
	//じゃんぷ
	void FallJump();
	//
	void Landing();
	//レイチェック
	void RayCheck(float angleX, float angleY);
	//再度matrix処理
	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
	//ワープ処理
	void WarpUpdate();
	//追加の回転
	void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);
	//平行移動
	void MoveMatrixUpdate(XMMATRIX matRot, XMMATRIX matTrans);
	//当たり判定後の処理
	void CollisionAfter();
	//ベクトルをチェンジする
	void VectorChange();

	//Settter
	//ワープ
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }
	//ワープ青
	void SetMemo2(XMVECTOR Memo) { Warpblue = Memo; }
	//ワープのフラグ
	void SetWarpFlag(bool flag) { warpFlag = flag; }
	//ワープのフラグ
	void SetWarpFlag2(bool flag) { warpFlag2 = flag; }
	//カメラのｚ軸
	void SetCameraAxis(XMVECTOR z) { CammeraZAxis = z; }
	//pos
	void SetPos(XMFLOAT3 pos) { position = pos; }
	//壁の当たり判定
	void SetWall(bool a) { WallCollision = a; }

	void SetRotationY(float rotation_) { rotation.y = rotation_; }

	//Gettter
	XMVECTOR GetMove() { return moveCamera; }

	XMVECTOR GetFall() { return fallV; }
	//pos
	XMFLOAT3 GetPosition() { return Warp2; }
	//pos
	XMFLOAT3 GetMyPosition() { return position; }
	//着地
	bool Getground() { return onGround; }
	//赤テレポート
	bool GetredTeleport() { return redTeleport; }
	//青テレポート
	bool GetblueTeleport() { return blueTeleport; }
	//debug
	float GetPos() { return position.x; }
	//シーンのセット
	void SetScene(int scene_) { scene = scene_; }
	//壁の当たり判定
	bool GetWall() { return WallCollision;}
	//歩いてるか
	bool GetWark() { return wark; }
	//出口か
	bool GetExit() { return Exit; }

	bool GetIsPlay() { return isPlay; }

	//debug
	int aGet() { return a; }
	//debug
	//bool GetDebug() { return debugCheck; }
	//当たっているか
	bool GetColision() { return WallCollision; }
	//コリジョンのベクトル
	XMVECTOR GetColisionVec() { return ColisionPoint; }
	//一フレーム前の動き
	void GetmoveOld(XMVECTOR a) { moveOld = a; }

	void SetTutorial(bool flag) { Tutorial = flag; }

	void SetTarget(XMFLOAT3 flag) { Target = flag; }

	bool GetTutorialWalk() { return Tutorial_walk; }

	float GetPositionZ() { return position.z; }

	void SetMenu(bool flag) { Menu_flag = flag; }

	void PlayerResetTitle();
private:

	//クラスのポインタ
	Input* input;//入力クラス
	//
	Physics* physics;//物理クラス

	float FallCount = 0;

	bool Tutorial=true;
	//変数
	int debugCheck;
	//ワープの時間
	int WarpTime = 0;
	//ワープのフラッグ
	bool TimeWarpF = true;
	//ｘアングル
	float angleX_;
	//ｙアングル
	float angleY_;
	//
	float Momentum;
	//シーン
	int scene;
	
	bool onGround = false;//地面の判定

	bool redTeleport = false;//赤への転送

	bool blueTeleport = false;//青への転送

	bool blueCollision = false;//青への転送フラッグ

	bool redCollision = false;//赤への転送フラッグ

	bool warpFlag = false;//trueになったらwarp出来るようにする

	bool warpFlag2 = false;//trueになったらwarp出来るようにする
	//コリジョンの１フレーム前
	bool ColOld;
	//コリジョンの１フレーム前
	bool ColOld2;

	int Tutorial_time = 0;

	bool Tutorial_walk = false;

	bool Menu_flag = false;

	//
	float acceleration_g = 9.81 / 60;//加速度
	//
	float JumpVel = 2;//Jumpの初速
	//当たりはんってい
	bool WallCollision = false;
	//歩きのフラグ
	bool wark = false;
	//出口のフラグ
	bool Exit = false;
	int a = 0;
	//
	XMVECTOR fallV = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//落ちるベクトル
	//
	XMVECTOR moveCamera=DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//動作ベクトル
	//１frame前の動き
	XMVECTOR moveOld = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//１frame前の動き
	XMVECTOR moveOld_ = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//当たった場所
	XMVECTOR ColisionPoint = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//ベクトル
	XMVECTOR Vector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//
	XMVECTOR memory = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//記憶用変数
	//
	XMVECTOR Warp = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//ワープするための移動変数（Vector）
	//
	XMVECTOR Warpblue = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//ワープするための移動変数（Vector）
	//
	XMFLOAT3 Warp2;//ワープするための移動変数（Float）
	//カメラのｚ軸
	XMVECTOR CammeraZAxis = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//昔の位置
	XMFLOAT3 OldPos;
	//昔の回転
	XMMATRIX OldMatrot;

	XMFLOAT3 Target;

	int Old_iPad_left = 0, Old_iPad_right = 0, iOld_Pad_up = 0, Old_iPad_down = 0;
	int Old_iPad_leftshoulder = 0, Old_iPad_rightshoulder = 0;
	int Old_iPad_A = 0, Old_iPad_B = 0, Old_iPad_X = 0, Old_iPad_Y = 0;
};

