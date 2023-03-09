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

class RenderTexture
{


protected:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

	ComPtr<ID3D12Resource> constBuffTransform;
	//スケール
	XMFLOAT3 scale = { 1,1,1 };
	//回転
	XMFLOAT3 rotation = { 0,0,0 };
	//pos
	XMFLOAT3 position = { 0,0,0 };
	//コリジョンサイズ
	XMFLOAT3 colisionsize = { 0,0,0 };
	//マットワールド
	XMMATRIX matWorld;
	//モデル
	Model* model = nullptr;
	//ベクトル
	XMFLOAT3 vec = { 0,0,1 };
	//動き
	XMVECTOR move = { 0,0,1.0f,0 };
	//
	XMVECTOR move2 = { 0,0,1.0f,0 };

	//発射したか
	int TriggerFlag = 0;
	//はっしゃしたか
	int TriggerFlag2 = 0;

public:

	//デバイスセット
	static void SetDevice(ID3D12Device* device) { RenderTexture::device = device; }
	//カメラセット
	static void SetCamera(Camera* camera) { RenderTexture::camera = camera; }

	RenderTexture(Input* input);
	virtual~RenderTexture();
	//更新
	virtual void Update();

	//render初期化
	virtual void RenderFbxInitialize();
	//モデルセット
	void SetModel(Model* model) { this->model = model; }
	//描画
	virtual void Draw2(ID3D12GraphicsCommandList* cmdList);
	//render描画
	void Draw2Ren(ID3D12GraphicsCommandList* cmdList, XMFLOAT3 pos);
	//model描画
	virtual void RenderFbxDraw(ID3D12GraphicsCommandList* cmdList);
	//
	virtual void RenderFbxDraw2(ID3D12GraphicsCommandList* cmdList);

	//rederグラフィックスパイプライン
	static void PortalCreateGraphicsPipeline();

	//fbxのアニメーション
	void PlayAnimation2();
	//setter
	void SetPosition(const DirectX::XMFLOAT3& position_) { position = position_; }
	void SetScale(const DirectX::XMFLOAT3& scale_) { scale = scale_; }
	void SetRotate(const DirectX::XMFLOAT3& rotation_) { rotation = rotation_; }
	void SetColisionSize(const DirectX::XMFLOAT3& colisionsize_) { colisionsize = colisionsize_; }

	//getter
	const XMMATRIX& GetMatWorld() { return matWorld; }//ワールド行列の取得
	const XMFLOAT3& GetPosition() { return position; }//ポジションの取得
	const XMFLOAT3& GetScale() { return scale; }//スケールの取得
	const XMFLOAT3& GetRotation() { return rotation; }//ローテーションの取得
	const XMFLOAT3& GetColisionSize() { return colisionsize; }//ローテーションの取得

	const float& GetColision_x() { return colisionsize.x; }
	const float& GetColision_y() { return colisionsize.y; }
	const float& GetColision_z() { return colisionsize.z; }


	inline Model* GetModel() { return model; }
	//matrix更新
	void UpdateWorldMatix();


	//
	void SetWorld(XMMATRIX matrot_) { matrot = matrot_; }
	//render前処理
	void RenPreDraw(ID3D12GraphicsCommandList* cmdList);
	//後処理
	void RenPostDraw(ID3D12GraphicsCommandList* cmdList);
	//赤の前処理
	void RenPreDraw2(ID3D12GraphicsCommandList* cmdList);
	//青の前処理
	void RenPostDraw2(ID3D12GraphicsCommandList* cmdList);
	//回転
	XMMATRIX matrot;

protected:
	//frameタイム
	FbxTime frameTime;
	//スタートタイム
	FbxTime startTime;
	//エンドタイム
	FbxTime endTime;
	//
	FbxTime currentTime;
	//再生するか
	bool isPlay = false;

	// 入力クラスのポインタ
	Input* input;
	//名前
	const char* name = nullptr;

public://定数
	//ボーン数
	static const int MAX_BONES = 32;
	//スキン
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	//定数バッファ
	ComPtr<ID3D12Resource>constBuffSkin;

protected:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;

	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステート
	static ComPtr<ID3D12PipelineState>pipelinestate;


public:

	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		float scale;
		float Color;
		bool Flag;
	};
};

