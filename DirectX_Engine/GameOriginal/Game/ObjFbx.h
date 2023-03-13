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

    bool GetTutorialGun() { return Tutorial_gun; }

    float GetRotationY() { return rotation.y; }
protected:
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
};

