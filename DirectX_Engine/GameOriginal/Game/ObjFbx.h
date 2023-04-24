#pragma once
#include "../../Engine/3d/Fbx3d.h"
#include "Physics.h"
#include "../../Engine/Input/Input.h"
class ObjFbx :
    public Fbx3d
{
public:
    void OnCollision(const CollisionInfo& info) override;
    //Obj��Initialize
    void ObjInitialize();
    //�|�C���^
    ObjFbx(Input* input);
    //�A�b�v�f�[�g
    void ObjUpdate(float angleX, float angleY);
    //���C�̓����蔻��`�F�b�N
    void RayCheck();
    //Box�̃A�b�v�f�[�g
    void BoxObjUpdate(float angleX, float angleY);
    //Box�̃��C�`�F�b�N
    void BoxRayCheck();

    void TurettUpdate();

    void RazerRayCheck();
    //�d�͂̏���
    void ObjFall();
    //Matrix�̃A�b�v�f�[�g
    void PostMatrixUpdate(XMMATRIX matScale, XMMATRIX matRot, XMMATRIX matTrans);
    //�ǉ���]����
    void AddRotateMatrixUpdate(float angleX, float angleY, XMMATRIX matRot);

    //Setter
    //Z���Z�b�g
    void SetCameraAxisZ(XMVECTOR z) { CammeraZAxis = z; }
    //pos�̃Z�b�g
    void SetMyPosition(XMFLOAT3 pos) { MyPosition = pos; }
    //�����_�̃Z�b�g
    void SetTarget(XMFLOAT3 pos) { Target = pos; }
    //�V�[���̃Z�b�g
    void SetScene(int scene_) { scene = scene_; }
    //OBJ�̎�������̎Q��
    bool Getgetobj() { return cursorOn; }
    //�|�[�^�����������̎Q��1��
    void SetShotBlue(bool a) { Shot = a; }
    //�|�[�^�����������̎Q�ƂQ��
    void SetShotRed(bool a) { Shot2 = a; }
    //�����Ă邩�̃Z�b�g
    void SetWark(bool a) { wark = a; }
   
    void SetTutorial(bool Flag) { Tutorial = Flag; }

    void SetTutorialGun(bool Flag) { Tutorial_gun = Flag; }

    bool GetTutorialGun() { return Tutorial_gun; }

    bool GetPossible() { return Possible_get; }

    float GetRotationY() { return rotation.y; }

    void SetInitPosition(XMFLOAT3 pos) { Init_pos = pos; }

    XMFLOAT3 GetInitPosition() { return Init_pos; }

    void SetInitRotate(XMFLOAT3 rotate) { Init_rotate = rotate; }

    void SetCursorOn(bool flag) { cursorOn = flag; }

    XMFLOAT3 GetInitRotate() { return Init_rotate; }
protected:

    XMFLOAT3 Init_pos;
    XMFLOAT3 Init_rotate;

    //�N���X�̃|�C���^
    Input* input;//���̓N���X
    //�ϐ�
    //�擾�������̃t���O
    bool cursorOn = false;
    //�擾�������̃t���O
    bool cursorOn2 = false;
    //����������
    bool Shot = false;
    //����������
    bool Shot2 = false;
    //���R�C��
    bool Recoile = false;
    //�����h��
    bool Recoile2 = false;
    //�ǉ��̉�]
    XMFLOAT3  AddRotate;
    //�ǉ��̃|�W�V����
    XMFLOAT3  AddPosition;
    //�������̃t���O
    bool wark;
    //���邩�̓���̃t���O
    bool warkmove = true;
    //�����Ă鎞��
    int Wark_time = 0;
    //�n�ʂ̔���
    bool onGround = true;//�n�ʂ̔���

    bool Collision_Box = false;

    bool Possible_get = false;
    //������
    float Box_ray_length = 8.0f;

    XMFLOAT3 Old_stock_pos;

    //������x�N�g��
    XMVECTOR fallV;

    bool Tutorial_gun = false;

    bool Tutorial = true;

    //�V�[��
    int scene;
    //�J������Z��
    XMVECTOR CammeraZAxis;
    //�J�����̒����_
    XMFLOAT3 Target;
    //�����̃|�W�V����
    XMFLOAT3 MyPosition;
    //Vector
    XMVECTOR Vector;
    //angle�̗݌vY
    float oldangleY;
    //angle�̗݌vX
    float oldangleX;

    int Old_iPad_left = 0, Old_iPad_right = 0, iOld_Pad_up = 0, Old_iPad_down = 0;
    int Old_iPad_leftshoulder = 0, Old_iPad_rightshoulder = 0;
    int Old_iPad_A = 0, Old_iPad_B = 0, Old_iPad_X = 0, Old_iPad_Y = 0;
};

