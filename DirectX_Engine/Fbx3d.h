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
//#include "Input.h"


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

	XMMATRIX matWorld;

	Model* model = nullptr;

	XMFLOAT3 vec = { 0,0,1 };

	XMVECTOR move = { 0,0,1.0f,0 };

	int TriggerFlag = 0;

	float dy;
    float dx;
public:

	static void SetDevice(ID3D12Device* device) { Fbx3d::device = device; }
	static void SetCamera(Camera*camera ){Fbx3d::camera=camera; }


	Fbx3d(Input* input);

	void Initialize();

	void Update();
	void Update_CameraVec(double angleX, double angleY, int Move);




	void SetModel(Model* model) { this->model = model; }

	void Draw2(ID3D12GraphicsCommandList* cmdList);

	static void CreateGraphicsPipeline();

	

	void PlayAnimation2();

	//void SetVec(XMVECTOR) {};;

	void SetPosition(const DirectX::XMFLOAT3& position_) { position = position_; }
	void SetScale(const DirectX::XMFLOAT3& scale_) { scale = scale_; }

private:
	FbxTime frameTime;

	FbxTime startTime;

	FbxTime endTime;

	FbxTime currentTime;

	bool isPlay = false;


	float angleX1;
	float angleY1;

	
	// 入力クラスのポインタ
	Input* input;

public://定数
	
	static const int MAX_BONES = 32;

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};



	//定数バッファ
	ComPtr<ID3D12Resource>constBuffSkin;

private:

	static ID3D12Device* device;

	static Camera* camera;

	static ComPtr<ID3D12RootSignature>rootsignature;

	static ComPtr<ID3D12PipelineState>pipelinestate;

public:

	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
};

