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
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;
	//�|�C���^
	PlayerFbx(Input* input, Physics* physics);
	//�A�b�v�f�[�g
	void PlayerUpdate(float angleX,float angleY);
	//������
	void FallJump();
	//
	void Landing();
	//���C�`�F�b�N
	void RayCheck(float angleX, float angleY);
	//�ēxmatrix����
	void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
	//���[�v����
	void WarpUpdate();
	//�ǉ��̉�]
	void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);
	//���s�ړ�
	void MoveMatrixUpdate(XMMATRIX matRot, XMMATRIX matTrans);
	//�����蔻���̏���
	void CollisionAfter();
	//�x�N�g�����`�F���W����
	void VectorChange();

	//Settter
	//���[�v
	void SetMemo(XMVECTOR Memo) { Warp = Memo; }
	//���[�v��
	void SetMemo2(XMVECTOR Memo) { Warpblue = Memo; }
	//���[�v�̃t���O
	void SetWarpFlag(bool flag) { warpFlag = flag; }
	//���[�v�̃t���O
	void SetWarpFlag2(bool flag) { warpFlag2 = flag; }
	//�J�����̂���
	void SetCameraAxis(XMVECTOR z) { CammeraZAxis = z; }
	//pos
	void SetPos(XMFLOAT3 pos) { position = pos; }
	//�ǂ̓����蔻��
	void SetWall(bool a) { WallCollision = a; }

	void SetRotationY(float rotation_) { rotation.y = rotation_; }

	//Gettter
	XMVECTOR GetMove() { return moveCamera; }

	XMVECTOR GetFall() { return fallV; }
	//pos
	XMFLOAT3 GetPosition() { return Warp2; }
	//pos
	XMFLOAT3 GetMyPosition() { return position; }
	//���n
	bool Getground() { return onGround; }
	//�ԃe���|�[�g
	bool GetredTeleport() { return redTeleport; }
	//�e���|�[�g
	bool GetblueTeleport() { return blueTeleport; }
	//debug
	float GetPos() { return position.x; }
	//�V�[���̃Z�b�g
	void SetScene(int scene_) { scene = scene_; }
	//�ǂ̓����蔻��
	bool GetWall() { return WallCollision;}
	//�����Ă邩
	bool GetWark() { return wark; }
	//�o����
	bool GetExit() { return Exit; }

	bool GetIsPlay() { return isPlay; }

	//debug
	int aGet() { return a; }
	//debug
	//bool GetDebug() { return debugCheck; }
	//�������Ă��邩
	bool GetColision() { return WallCollision; }
	//�R���W�����̃x�N�g��
	XMVECTOR GetColisionVec() { return ColisionPoint; }
	//��t���[���O�̓���
	void GetmoveOld(XMVECTOR a) { moveOld = a; }

	void SetTutorial(bool flag) { Tutorial = flag; }

	void SetTarget(XMFLOAT3 flag) { Target = flag; }

	bool GetTutorialWalk() { return Tutorial_walk; }

	float GetPositionZ() { return position.z; }

	void SetMenu(bool flag) { Menu_flag = flag; }

	void PlayerResetTitle();
private:

	//�N���X�̃|�C���^
	Input* input;//���̓N���X
	//
	Physics* physics;//�����N���X

	float FallCount = 0;

	bool Tutorial=true;
	//�ϐ�
	int debugCheck;
	//���[�v�̎���
	int WarpTime = 0;
	//���[�v�̃t���b�O
	bool TimeWarpF = true;
	//���A���O��
	float angleX_;
	//���A���O��
	float angleY_;
	//
	float Momentum;
	//�V�[��
	int scene;
	
	bool onGround = false;//�n�ʂ̔���

	bool redTeleport = false;//�Ԃւ̓]��

	bool blueTeleport = false;//�ւ̓]��

	bool blueCollision = false;//�ւ̓]���t���b�O

	bool redCollision = false;//�Ԃւ̓]���t���b�O

	bool warpFlag = false;//true�ɂȂ�����warp�o����悤�ɂ���

	bool warpFlag2 = false;//true�ɂȂ�����warp�o����悤�ɂ���
	//�R���W�����̂P�t���[���O
	bool ColOld;
	//�R���W�����̂P�t���[���O
	bool ColOld2;

	int Tutorial_time = 0;

	bool Tutorial_walk = false;

	bool Menu_flag = false;

	//
	float acceleration_g = 9.81 / 60;//�����x
	//
	float JumpVel = 2;//Jump�̏���
	//������͂���Ă�
	bool WallCollision = false;
	//�����̃t���O
	bool wark = false;
	//�o���̃t���O
	bool Exit = false;
	int a = 0;
	//
	XMVECTOR fallV = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//������x�N�g��
	//
	XMVECTOR moveCamera=DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//����x�N�g��
	//�Pframe�O�̓���
	XMVECTOR moveOld = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//�Pframe�O�̓���
	XMVECTOR moveOld_ = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//���������ꏊ
	XMVECTOR ColisionPoint = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//�x�N�g��
	XMVECTOR Vector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//
	XMVECTOR memory = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//�L���p�ϐ�
	//
	XMVECTOR Warp = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//���[�v���邽�߂̈ړ��ϐ��iVector�j
	//
	XMVECTOR Warpblue = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;//���[�v���邽�߂̈ړ��ϐ��iVector�j
	//
	XMFLOAT3 Warp2;//���[�v���邽�߂̈ړ��ϐ��iFloat�j
	//�J�����̂���
	XMVECTOR CammeraZAxis = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);;
	//�̂̈ʒu
	XMFLOAT3 OldPos;
	//�̂̉�]
	XMMATRIX OldMatrot;

	XMFLOAT3 Target;

	int Old_iPad_left = 0, Old_iPad_right = 0, iOld_Pad_up = 0, Old_iPad_down = 0;
	int Old_iPad_leftshoulder = 0, Old_iPad_rightshoulder = 0;
	int Old_iPad_A = 0, Old_iPad_B = 0, Old_iPad_X = 0, Old_iPad_Y = 0;
};

