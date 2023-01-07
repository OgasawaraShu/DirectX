#pragma once
//#include "3d/Object3d.h"
#include "Camera.h"

#include "Model.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include "3d/FbxLoader.h"
#include "DebugCamera.h"
#include "CollisionInfo.h"
#include "CollisionManager.h"
#include "LightGroup.h"
#include "BlueCamera.h"
#include "RedCamera.h"
//#include "Input.h"

class BaseCollider;

class Fbx3d //:
	//public Object3d
{



protected:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

	ComPtr<ID3D12Resource> constBuffTransform;



	XMFLOAT3 scale = { 1,1,1 };

	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 position = { 0,0,0 };

	XMFLOAT3 colisionsize = { 0,0,0 };

	XMMATRIX matWorld;

	Model* model = nullptr;

	XMFLOAT3 vec = { 0,0,1 };

	XMVECTOR move = { 0,0,1.0f,0 };

	XMVECTOR move2 = { 0,0,1.0f,0 };

	XMVECTOR memory = { 0,0,1.0f,0 };

	XMVECTOR memory2 = { 0,0,1.0f,0 };



	XMVECTOR Getmemo() { return memory; }
	XMVECTOR Getmemo2() { return memory2; }


	int Fbx_model_type;
	int TriggerFlag = 0;

	float dy;
    float dx;

	int TriggerFlag2 = 0;

	float dy2;
	float dx2;
	bool Dead_flag = false;

public:

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="lightGroup">ライトグループ</param>
	static void SetLightGroup(LightGroup* lightGroup) {
		Fbx3d::lightGroup = lightGroup;
	}
	static void SetDevice(ID3D12Device* device) { Fbx3d::device = device; }
	static void SetCamera(Camera*camera ){Fbx3d::camera=camera; }
	static void SetBlueCamera(BlueCamera* camera) { Fbx3d::bluecamera = camera; }
	static void SetRedCamera(RedCamera* camera) { Fbx3d::redcamera = camera; }

	Fbx3d(Input* input);
	virtual~Fbx3d();

	virtual void Initialize();

	virtual void Update();

	virtual void Update2(Camera*camera);


	virtual void RenderFbxInitialize();
	virtual void RenderFbxInitialize2();

	
	void SetModel(Model* model) { this->model = model; }

	virtual void Draw2(ID3D12GraphicsCommandList* cmdList);
	void Draw2Ren(ID3D12GraphicsCommandList* cmdList,XMFLOAT3 pos);
	virtual void RenderFbxDraw(ID3D12GraphicsCommandList* cmdList );
	virtual void RenderFbxDraw2(ID3D12GraphicsCommandList* cmdList);


	static void CreateGraphicsPipeline();

    void PortalCreateGraphicsPipeline();
	void PortalCreateGraphicsPipeline2();
	void DrawPortalWindow(ID3D12GraphicsCommandList* cmdList, XMMATRIX matWorld2);
	
	void DrawPortalWindowRed(ID3D12GraphicsCommandList* cmdList, XMMATRIX matWorld2);

	void PlayAnimation2();

	void SetPosition(const DirectX::XMFLOAT3& position_) { position = position_; }
	void SetScale(const DirectX::XMFLOAT3& scale_) { scale = scale_; }
	void SetRotate(const DirectX::XMFLOAT3& rotation_) { rotation = rotation_; }
	void SetColisionSize(const DirectX::XMFLOAT3& colisionsize_) { colisionsize = colisionsize_; }
	void SetFbxmodelType(const int &type_) { Fbx_model_type = type_; }
	void SetDeadFlag(const bool flag) { Dead_flag = flag; }

	const XMMATRIX& GetMatWorld() { return matWorld; }//ワールド行列の取得
	const XMFLOAT3& GetPosition() { return position; }//ポジションの取得
	const XMFLOAT3& GetScale() { return scale; }//スケールの取得
	const XMFLOAT3& GetRotation() { return rotation; }//ローテーションの取得
	const XMFLOAT3& GetColisionSize() { return colisionsize; }//ローテーションの取得
	const bool GetDeadFlag() { return Dead_flag; }

	const float& GetColision_x() { return colisionsize.x; }
	const float& GetColision_y() { return colisionsize.y; }
	const float& GetColision_z() { return colisionsize.z; }
	const int& GetFbxmodelType() { return Fbx_model_type; }
	void SetColider(BaseCollider* collider);//こらいだーのセット
	virtual void OnCollision(const CollisionInfo& info);//コールバック関数


	inline Model* GetModel() { return model; }

	void UpdateWorldMatix();

	void SetRotateScale(XMMATRIX matworld);

	void SetVer();

	void SetVerBulletBlue();

	void SetVerBulletRed();

	void SetVerLand();

	void SetVerObj();

	void SetVerObj2();


	void SetVerWall();

	void SetVerSPHEREOBJ();

	void SetVerExit();


	void SetWorld(XMMATRIX matrot_) { matrot = matrot_; }



	void RenPreDraw(ID3D12GraphicsCommandList* cmdList);

	void RenPostDraw(ID3D12GraphicsCommandList* cmdList);




	void RenPreDraw2(ID3D12GraphicsCommandList* cmdList);

	void RenPostDraw2(ID3D12GraphicsCommandList* cmdList);

	XMMATRIX matrot;

protected:
	FbxTime frameTime;

	FbxTime startTime;

	FbxTime endTime;

	FbxTime currentTime;

	bool isPlay = false;


	float angleX1;
	float angleY1;


	float angleX2;
	float angleY2;

	
	// 入力クラスのポインタ
	Input* input;

	const char* name = nullptr;

	BaseCollider* collider = nullptr;

public://定数
	
	static const int MAX_BONES = 32;

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};



	//定数バッファ
	ComPtr<ID3D12Resource>constBuffSkin;

	//デバック
	int debug = 0;
	int debug2 = 0;

protected:

	static ID3D12Device* device;

	static Camera* camera;

	static BlueCamera* bluecamera;

	static RedCamera* redcamera;

	// ライト
	static LightGroup* lightGroup;

	static ComPtr<ID3D12RootSignature>rootsignature;

	static ComPtr<ID3D12PipelineState>pipelinestate;

	
public:

	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMFLOAT3 cameraPos2;
	};
};

