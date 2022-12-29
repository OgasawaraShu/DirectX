#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include "Mesh.h"

#include<fbxsdk.h>

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};

class Model
{

public:
	//読み込む
	static Model* LoadFromOBJ(const std::string& modelname);

	friend class FbxLoader;

	static const int MAX_BONE_INDICES = 4;

	FbxScene* fbxScene = nullptr;

	FbxScene* GetFbxScene() { return fbxScene; }

	~Model();
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	std::string name;

	std::vector<Node>nodes;

	// DirectX::を省
	
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMettadata=DirectX::TexMetadata;
	using ScratchImage=DirectX::ScratchImage;
	
	using string = std::string;
//	template<class T> using vector =
		template <class T> using vector = std::vector<T>;



public: // サブクラス
// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

	//マテリアル
	struct Material
	{
		std::string name;//マテリアル名
		DirectX::XMFLOAT3 ambient;//アンビエント影響度
		DirectX::XMFLOAT3 diffuse;//ディフューズ影響度
		DirectX::XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//α
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//マテリアル
	 Material material;

	 //定数バッファ用データ構造体B１
	 struct ConstBufferDataB1
	 {
		 DirectX::XMFLOAT3 ambient;//アンビエント係数
		 float pad1;//パディング
		 DirectX::XMFLOAT3 diffuse;//ディフューズ係数
		 float pad2;//パディング
		 DirectX::XMFLOAT3 specular;//スぺキュラー係数
		 float alpha;//α
	 };


	 //ノード
	 /*
	 struct Node
	 {
		 //名前
		 std::string name;
		 //ローカルスケール
		 DirectX::XMVECTOR scaling = { 1,1,1,0 };
		 //回転角
		 DirectX::XMVECTOR rotation = { 0,0,0,0 };
		 //移動
		 DirectX::XMVECTOR translation = { 0,0,0,1 };
		 //変形行列
		 DirectX::XMMATRIX transform;
		 //グローバル変形行列
		 DirectX::XMMATRIX globalTransform;
		 //親ノード
		 Node* parent = nullptr;
	 };
	 */
/*
	 struct VetexPosNormalUvSkin
	 {
		 DirectX::XMFLOAT3 pos;
		 DirectX::XMFLOAT3 normal;
		 DirectX::XMFLOAT2 uv;
		 UINT boneIndex[MAX_BONE_INDICES];//番号
		 float boneWeight[MAX_BONE_INDICES];//重み
	 };
	 */
	 //ボーン構造体
	 struct Bone
	 {
		 //名前
		 std::string name;
		 //初期姿勢の逆
		 DirectX::XMMATRIX invInitialPose;
		 //クラスター
		 FbxCluster* fbxCluster;
		 //コンストラクタ
		 Bone(const std::string& name)
		 {
			 this->name = name;
		 }
	 };

	 //目sshすぉもつノード
	 Node* meshNode = nullptr;
	 //頂点データ配列
	 std::vector<VertexPosNormalUv>vertices;
	 //頂点インデックス配列
	 std::vector<unsigned short>indices;
public:


	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	 static ID3D12Device* device;

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

	//bool InitializeDescriptorHeap();

	//void CreateBuffers();

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
    void RenderInitialize(ID3D12Device* device);

	bool InitializeDescriptorHeap();

	void Draw3(ID3D12GraphicsCommandList* cmdList);

	void RenderDraw(ID3D12GraphicsCommandList* cmdList);

	void CreateBuffers2(ID3D12Device* device);

	void RenCreateBuffers2(ID3D12Device* device);



	void RenderInitialize2(ID3D12Device* device);

	void RenderDraw2(ID3D12GraphicsCommandList* cmdList);

	void RenCreateBuffers22(ID3D12Device* device);



	void Draw2(ID3D12GraphicsCommandList* cmdList);

	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

    //inline  const std::vector<Mesh>& GetMeshes() { return  ;}

	
	//std::vector<unsigned short>indices;

	// 頂点データ配列
	//std::vector<VertexPosNormalUv> vertices;
	//ボーン
	std::vector<Bone> bones;

	//getter
	std::vector<Bone>& GetBones() { return bones; }

	/// <summary>
/// 頂点配列を取得
/// </summary>
/// <returns>頂点配列</returns>
	inline const std::vector<VertexPosNormalUv>& GetVertices() { return vertices; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned short>& GetIndices() { return indices; }

	void RenPreDrawScene(ID3D12GraphicsCommandList* cmdList);

	void RenPostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void RenPreDrawScene2(ID3D12GraphicsCommandList* cmdList);

	void RenPostDrawScene2(ID3D12GraphicsCommandList* cmdList);

	static const float clearColor[4];

private
://メンバ変数
	
	
	//ID3D12Device* device;
	
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;


	//rendertexture
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> RentexBuff;

	ComPtr<ID3D12DescriptorHeap> RendescHeapSRV;

	ComPtr<ID3D12Resource> RendepthBuff;

	ComPtr<ID3D12DescriptorHeap> RendescHeapRTV;

	ComPtr<ID3D12DescriptorHeap> RendescHeapDSV;

	//rendertexture
	ComPtr<ID3D12DescriptorHeap> descHeapSRV2;

	ComPtr<ID3D12Resource> RentexBuff2;

	ComPtr<ID3D12DescriptorHeap> RendescHeapSRV2;

	ComPtr<ID3D12Resource> RendepthBuff2;

	ComPtr<ID3D12DescriptorHeap> RendescHeapRTV2;

	ComPtr<ID3D12DescriptorHeap> RendescHeapDSV2;


	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//ディヒューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//テクスチャデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};

	//非公開
	void LoadFromOBJInternal(const std::string& modelname);

	
	
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1; // 定数バッファ

	

	//public:

		
	
};


