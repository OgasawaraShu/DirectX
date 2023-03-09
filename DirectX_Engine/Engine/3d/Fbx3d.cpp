#include "Fbx3d.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisionAttribute.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;

ComPtr<ID3D12RootSignature>Fbx3d::rootsignature;
ComPtr<ID3D12PipelineState>Fbx3d::pipelinestate;



ID3D12Device* Fbx3d::device = nullptr;
Camera* Fbx3d::camera = nullptr;
BlueCamera* Fbx3d::bluecamera = nullptr;
RedCamera* Fbx3d::redcamera = nullptr;
LightGroup* Fbx3d::lightGroup = nullptr;
using namespace DirectX;

Fbx3d::~Fbx3d()
{
	if (collider)
	{
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

void Fbx3d::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0

		 },
		{//スキンウェイト
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];

	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

}

void Fbx3d::PortalCreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{//影響を受けるボーン番号
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0

		 },
		{//スキンウェイト
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0〜255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[4];

	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

}

void Fbx3d::DrawPortalWindowBlue(ID3D12GraphicsCommandList* cmdList)
{
	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection =
		bluecamera->GetViewProjectionMatrix();
	//メッシュtランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = bluecamera->GetEye();

	HRESULT result;

	//定数バッファへ転送

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//アニメーション
	//
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}



	std::vector<Model::Bone>& bones = model->GetBones();
	
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	//割り当てなければやらない
	if (model == nullptr) {
		return;
	}

	//パイプラインステート
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャ
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->SetGraphicsRootConstantBufferView(0,
		constBuffTransform->GetGPUVirtualAddress());

	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(2,
		constBuffSkin->GetGPUVirtualAddress());

	// ライトの描画
	lightGroup->Draw(cmdList, 3);
	//描画
	model->Draw3(cmdList);

}

void Fbx3d::DrawPortalWindow(int Select)
{
	//行列の更新
	UpdateWorldMatix();
	HRESULT result;

	if (Select == 1)
	{
		//ビュープロジェクション行列
		const XMMATRIX& matViewProjection =
			redcamera->GetViewProjectionMatrix();
		//メッシュtランスフォーム
		const XMMATRIX& modelTransform = model->GetModelTransform();
		//カメラ座標
		const XMFLOAT3& cameraPos = redcamera->GetEye();

		
		//定数バッファへ転送

		ConstBufferDataTransform* constMap = nullptr;
		result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matViewProjection;
			constMap->world = modelTransform * matWorld;
			constMap->cameraPos = cameraPos;
			constBuffTransform->Unmap(0, nullptr);
		}
	}
	else if (Select == 2)
	{
		//ビュープロジェクション行列
		const XMMATRIX& matViewProjection =
			bluecamera->GetViewProjectionMatrix();
		//メッシュtランスフォーム
		const XMMATRIX& modelTransform = model->GetModelTransform();
		//カメラ座標
		const XMFLOAT3& cameraPos = bluecamera->GetEye();


		//定数バッファへ転送

		ConstBufferDataTransform* constMap = nullptr;
		result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matViewProjection;
			constMap->world = modelTransform * matWorld;
			constMap->cameraPos = cameraPos;
			constBuffTransform->Unmap(0, nullptr);
		}
	}




	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}



	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);
}

void Fbx3d::DrawPortalWindowMove(XMFLOAT3 pos_, XMFLOAT3 rotate_,int Select)
{

	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	HRESULT result;

	if (Select == 1)
	{
		//ビュープロジェクション行列
		const XMMATRIX& matViewProjection =
			redcamera->GetViewProjectionMatrix();
		//メッシュtランスフォーム
		const XMMATRIX& modelTransform = model->GetModelTransform();
		//カメラ座標
		const XMFLOAT3& cameraPos = redcamera->GetEye();



		//定数バッファへ転送

		ConstBufferDataTransform* constMap = nullptr;
		result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matViewProjection;
			constMap->world = modelTransform * matWorld;
			constMap->cameraPos = cameraPos;
			constBuffTransform->Unmap(0, nullptr);
		}
	}
	else if (Select == 2)
	{
		//ビュープロジェクション行列
		const XMMATRIX& matViewProjection =
			bluecamera->GetViewProjectionMatrix();
		//メッシュtランスフォーム
		const XMMATRIX& modelTransform = model->GetModelTransform();
		//カメラ座標
		const XMFLOAT3& cameraPos = bluecamera->GetEye();



		//定数バッファへ転送

		ConstBufferDataTransform* constMap = nullptr;
		result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
		if (SUCCEEDED(result))
		{
			constMap->viewproj = matViewProjection;
			constMap->world = modelTransform * matWorld;
			constMap->cameraPos = cameraPos;
			constBuffTransform->Unmap(0, nullptr);
		}
	}


	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}



	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);
}



Fbx3d::Fbx3d(Input* input)
{
	this->input = input;
}

void Fbx3d::Initialize()
{

	name = typeid(*this).name();

	HRESULT result;

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));

	//定数バッファの作成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));
	
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);


	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void Fbx3d::UpdateWorldMatix()
{
	XMMATRIX matScale, matRot, matTrans;

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;
}

void Fbx3d::SetRotateScale(XMMATRIX matworld)
{
	const XMMATRIX& matViewProjection =
		camera->GetViewProjectionMatrix();
	//メッシュtランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;

	//定数バッファへ転送

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matworld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}



	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

}

void Fbx3d::Update()
{
	//行列の更新
	UpdateWorldMatix();

	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection =
		camera->GetViewProjectionMatrix();
	//メッシュtランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;

	//定数バッファへ転送

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}



	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	
	//当たり判定更新
	if (collider) {
		collider->Update();
	}
}

void Fbx3d::Update2(Camera* camera)
{
	//行列の更新
	UpdateWorldMatix();

	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection =
		camera->GetViewProjectionMatrix();
	//メッシュtランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;

	//定数バッファへ転送

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}

	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

}

void Fbx3d::RenderFbxInitialize()
{
	Initialize();
}

void Fbx3d::Draw2(ID3D12GraphicsCommandList* cmdList)
{
	//割り当てなければやらない
	if (model == nullptr) {
		return;
	}

	//パイプラインステート
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャ
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	cmdList->SetGraphicsRootConstantBufferView(0,
		constBuffTransform->GetGPUVirtualAddress());

	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(2,
		constBuffSkin->GetGPUVirtualAddress());
	
	// ライトの描画
	lightGroup->Draw(cmdList, 3);
	//描画
	model->Draw3(cmdList);
}

void Fbx3d::Draw2Ren(ID3D12GraphicsCommandList* cmdList, XMFLOAT3 pos)
{
	//行列の更新
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);
	matWorld *= matTrans;
	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection =
		redcamera->GetViewProjectionMatrix();
	//メッシュtランスフォーム
	const XMMATRIX& modelTransform = model->GetModelTransform();
	//カメラ座標
	const XMFLOAT3& cameraPos = redcamera->GetEye();

	HRESULT result;

	//定数バッファへ転送

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	//アニメーション
	if (isPlay) {
		//1フレーム進める
		currentTime += frameTime;

		//最後まで行ったら先頭に戻す
		if (currentTime > endTime) {
			currentTime = startTime;
		}

	}



	std::vector<Model::Bone>& bones = model->GetBones();

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		XMMATRIX matCurrentPose;

		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);

	//割り当てなければやらない
	if (model == nullptr) {
		return;
	}

	//パイプラインステート
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャ
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->SetGraphicsRootConstantBufferView(0,
		constBuffTransform->GetGPUVirtualAddress());

	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(2,
		constBuffSkin->GetGPUVirtualAddress());

	// ライトの描画
	//lightGroup->Draw(cmdList, 3);
	//描画
	model->Draw3(cmdList);
}

void Fbx3d::RenderFbxDraw(ID3D12GraphicsCommandList* cmdList)
{
	//割り当てなければやらない
	if (model == nullptr) {
		return;
	}

	//パイプラインステート
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャ
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->SetGraphicsRootConstantBufferView(0,
		constBuffTransform->GetGPUVirtualAddress());

	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(2,
		constBuffSkin->GetGPUVirtualAddress());

	//描画
	model->RenderDraw(cmdList);
}

void Fbx3d::RenderFbxDraw2(ID3D12GraphicsCommandList* cmdList)
{
	//割り当てなければやらない
	if (model == nullptr) {
		return;
	}

	//パイプラインステート
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャ
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->SetGraphicsRootConstantBufferView(0,
		constBuffTransform->GetGPUVirtualAddress());

	//定数バッファビュー
	cmdList->SetGraphicsRootConstantBufferView(2,
		constBuffSkin->GetGPUVirtualAddress());

	//描画
	model->RenderDraw2(cmdList);
}

void Fbx3d::PlayAnimation2()
{
	FbxScene* fbxScene = model->GetFbxScene();
	//0のアニメーションを取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
	//アニメーション名前を取得
	const char* animstackname = animstack->GetName();
	//時間取得
	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);



	//開始時間取得
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	//終了時間取得
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	//開始時間に合わせる
	currentTime = startTime;
	//再生中にする
	isPlay = true;
}

void Fbx3d::SetColider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//マネージャに登録
	CollisionManager::GetInstance()->AddCollider(collider);
	//行列の更新
	UpdateWorldMatix();
	//コライダー更新
	collider->Update();
}

void Fbx3d::OnCollision(const CollisionInfo& info)
{

}



void Fbx3d::SetVer()
{
	//自機
	collider->SetAttribute(COLLISION_ATTR_PLAYER);
}

void Fbx3d::SetVerBulletBlue()
{
	//青球
	collider->SetAttribute(COLLISION_ATTR_BULLET_BLUE);
}

void Fbx3d::SetVerBulletRed()
{
	//赤球
	collider->SetAttribute(COLLISION_ATTR_BULLET_RED);
}

void Fbx3d::SetVerLand()
{
	//地面
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
}

void Fbx3d::SetVerObj()
{
	//obj
	collider->SetAttribute(COLLISION_ATTR_OBJ);
}

void Fbx3d::SetVerObj2()
{
	//obj2
	collider->SetAttribute(COLLISION_ATTR_OBJ2);
}

void Fbx3d::SetVerWall()
{
	//壁
	collider->SetAttribute(COLLISION_ATTR_WALL);
}

void Fbx3d::SetVerSPHEREOBJ()
{
	//球
	collider->SetAttribute(COLLISION_ATTR_SPHEREOBJ);
}

void Fbx3d::SetVerExit()
{
	//出口
	collider->SetAttribute(COLLISION_ATTR_EXIT);
}

void Fbx3d::SetVerBlock()
{
	//ブロックする箱
	collider->SetAttribute(COLLISION_ATTR_BLOCK);
}

void Fbx3d::RenPreDraw(ID3D12GraphicsCommandList* cmdList)
{
	model->RenPreDrawScene(cmdList);
}

void Fbx3d::RenPostDraw(ID3D12GraphicsCommandList* cmdList)
{
	model->RenPostDrawScene(cmdList);
}

void Fbx3d::RenPreDraw2(ID3D12GraphicsCommandList* cmdList)
{
	model->RenPreDrawScene2(cmdList);
}

void Fbx3d::RenPostDraw2(ID3D12GraphicsCommandList* cmdList)
{
	model->RenPostDrawScene2(cmdList);
}





