#pragma once
#include "../Camera/Camera.h"
#include "Model.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include "../3d/FbxLoader.h"
#include "../Camera/DebugCamera.h"
#include "../Collision/CollisionInfo.h"
#include "../Collision/CollisionManager.h"
#include "../Light/LightGroup.h"
#include "../Camera/BlueCamera.h"
#include "../Camera/RedCamera.h"
#include "../Input/Input.h"


class BaseCollider;

class Fbx3d 
{



protected:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

	ComPtr<ID3D12Resource> constBuffTransform;
	//�X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//��]
	XMFLOAT3 rotation = { 0,0,0 };
	//pos
	XMFLOAT3 position = { 0,0,0 };
	//�R���W�����T�C�Y
	XMFLOAT3 colisionsize = { 0,0,0 };
	//�}�b�g���[���h
	XMMATRIX matWorld;
	//���f��
	Model* model = nullptr;
	//�x�N�g��
	XMFLOAT3 vec = { 0,0,1 };
	//����
	XMVECTOR move = { 0,0,1.0f,0 };
	//
	XMVECTOR move2 = { 0,0,1.0f,0 };
	//���f���^�C�v
	int Fbx_model_type;
	//���˂�����
	int TriggerFlag = 0;
	//�͂����Ⴕ����
	int TriggerFlag2 = 0;
	//����
	int ver=0;
	//�폜�ϐ�
	bool Dead_flag = false;
	//�I�����ꂽ��
	bool Select_obj_flag = false;

public:

	/// <summary>
	/// ���C�g�O���[�v�̃Z�b�g
	/// </summary>
	/// <param name="lightGroup">���C�g�O���[�v</param>
	static void SetLightGroup(LightGroup* lightGroup) {
		Fbx3d::lightGroup = lightGroup;
	}
	//�f�o�C�X�Z�b�g
	static void SetDevice(ID3D12Device* device) { Fbx3d::device = device; }
	//�J�����Z�b�g
	static void SetCamera(Camera*camera ){Fbx3d::camera=camera; }
	//�J�����Z�b�g
	static void SetBlueCamera(BlueCamera* camera) { Fbx3d::bluecamera = camera; }
	//�ԃJ�����Z�b�g
	static void SetRedCamera(RedCamera* camera) { Fbx3d::redcamera = camera; }

	Fbx3d(Input* input);
	virtual~Fbx3d();
	//������
	virtual void Initialize();
	//�X�V
	virtual void Update();
	//�X�V�J������
	virtual void Update2(Camera*camera);
	//render������
	virtual void RenderFbxInitialize();
	//���f���Z�b�g
	void SetModel(Model* model) { this->model = model; }
	//�`��
	virtual void Draw2(ID3D12GraphicsCommandList* cmdList);
	//render�`��
	void Draw2Ren(ID3D12GraphicsCommandList* cmdList,XMFLOAT3 pos);
	//model�`��
	virtual void RenderFbxDraw(ID3D12GraphicsCommandList* cmdList );
	//
	virtual void RenderFbxDraw2(ID3D12GraphicsCommandList* cmdList);
    //�O���t�B�b�N�X�p�C�v���C��
	static void CreateGraphicsPipeline();
	//reder�O���t�B�b�N�X�p�C�v���C��
    void PortalCreateGraphicsPipeline();
	void PortalCreateGraphicsPipeline2();
	//portal�̕`��
	void DrawPortalWindow(ID3D12GraphicsCommandList* cmdList, XMMATRIX matWorld2);
	//�Ԃ̃|�[�^���`��
	void DrawPortalWindowRed(ID3D12GraphicsCommandList* cmdList, XMMATRIX matWorld2);
	//fbx�̃A�j���[�V����
	void PlayAnimation2();
	//setter
	void SetPosition(const DirectX::XMFLOAT3& position_) { position = position_; }
	void SetScale(const DirectX::XMFLOAT3& scale_) { scale = scale_; }
	void SetRotate(const DirectX::XMFLOAT3& rotation_) { rotation = rotation_; }
	void SetColisionSize(const DirectX::XMFLOAT3& colisionsize_) { colisionsize = colisionsize_; }
	void SetFbxmodelType(const int &type_) { Fbx_model_type = type_; }
	void SetFbxVer(const int& ver_) { ver = ver_; }
	void SetDeadFlag(const bool flag) { Dead_flag = flag; }
	void SetSelectFlag(const bool flag) { Select_obj_flag = flag; }
	//getter
	const XMMATRIX& GetMatWorld() { return matWorld; }//���[���h�s��̎擾
	const XMFLOAT3& GetPosition() { return position; }//�|�W�V�����̎擾
	const XMFLOAT3& GetScale() { return scale; }//�X�P�[���̎擾
	const XMFLOAT3& GetRotation() { return rotation; }//���[�e�[�V�����̎擾
	const XMFLOAT3& GetColisionSize() { return colisionsize; }//���[�e�[�V�����̎擾
	const bool GetDeadFlag() { return Dead_flag; }
	const bool GetSelectFlag() { return Select_obj_flag; }
	const float& GetColision_x() { return colisionsize.x; }
	const float& GetColision_y() { return colisionsize.y; }
	const float& GetColision_z() { return colisionsize.z; }
	const int& GetFbxmodelType() { return Fbx_model_type; }
	const int& GetVer() { return ver; }

	void SetColider(BaseCollider* collider);//���炢���[�̃Z�b�g
	virtual void OnCollision(const CollisionInfo& info);//�R�[���o�b�N�֐�
	inline Model* GetModel() { return model; }
	//matrix�X�V
	void UpdateWorldMatix();
	//��]�Z�b�g
	void SetRotateScale(XMMATRIX matworld);
	//�����Z�b�g
	void SetVer();
	//����
	void SetVerBulletBlue();
	//�ԑ���
	void SetVerBulletRed();
	//���ʑ���
	void SetVerLand();
	//OBJ����
	void SetVerObj();
	//OBJ����
	void SetVerObj2();
    //�Ǒ���
	void SetVerWall();
	//������
	void SetVerSPHEREOBJ();
	//�o������
	void SetVerExit();
	//�u���b�N����
	void SetVerBlock();
    //
	void SetWorld(XMMATRIX matrot_) { matrot = matrot_; }
    //render�O����
	void RenPreDraw(ID3D12GraphicsCommandList* cmdList);
	//�㏈��
	void RenPostDraw(ID3D12GraphicsCommandList* cmdList);
    //�Ԃ̑O����
	void RenPreDraw2(ID3D12GraphicsCommandList* cmdList);
	//�̑O����
	void RenPostDraw2(ID3D12GraphicsCommandList* cmdList);
	//��]
	XMMATRIX matrot;

protected:
	//frame�^�C��
	FbxTime frameTime;
	//�X�^�[�g�^�C��
	FbxTime startTime;
	//�G���h�^�C��
	FbxTime endTime;
	//
	FbxTime currentTime;
	//�Đ����邩
	bool isPlay = false;

	//�A���O��
	float angleX1;
	float angleY1;

	//�A���O��
	float angleX2;
	float angleY2;

	
	// ���̓N���X�̃|�C���^
	Input* input;
	//���O
	const char* name = nullptr;
	//�R���C�_�[�|�C���^
	BaseCollider* collider = nullptr;

public://�萔
	//�{�[����
	static const int MAX_BONES = 32;
	//�X�L��
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffSkin;

	//�f�o�b�N
	int debug = 0;
	int debug2 = 0;

protected:
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//�J����
	static BlueCamera* bluecamera;
	//�ԃJ����
	static RedCamera* redcamera;

	// ���C�g
	static LightGroup* lightGroup;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12PipelineState>pipelinestate;

	
public:

	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
};

