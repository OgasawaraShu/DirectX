#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>
#include "../Model.h"
//#include ""

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	void Initialize(ID3D12Device* device);

	void Finalize();

	using string = std::string;

	//void LoadModelFromFile(const string& modelName);
	Model* LoadModelFromFile(const string& modelName);

	void ParseNodeRecursive(Model* model, FbxNode* fbxNode,Node*parent=nullptr);

	void ParseMesh(Model* model, FbxNode* fbxNode);

	//頂点座標読み取り
	void ParseMeshVectices(Model* model, FbxMesh* fbxMesh);
	//画面情報
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	//マテリアル
	void ParseMaterial(Model* model, FbxNode* fbxNode);
	//テクスチャ
	void LoadTexture(Model* model, const std::string& fullpath);

	std::string ExtractFileName(const std::string& path);




private:
	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャー
	FbxManager* fbxManager = nullptr;
	//インポータ
	FbxImporter* fbxImporter = nullptr;
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	

public://定数
	//モデル格納√パス
	static const string baseDirectory;

	static const string defaultTextureFileName;
};