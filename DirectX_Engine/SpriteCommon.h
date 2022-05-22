#pragma once
#include "PipeLineSet.h"
#include <DirectXMath.h>

class SpriteCommon
{
public:
	//スプライト用テクスチャ枚数
	static const int spriteSRVCount = 512;

	//初期化
	void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

	//初期化
	void Initialize_Post(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);


	void PreDraw();

	void PreDraw_Post();
	//読み込み
	void SpriteCommonLoadTexture(UINT texnumber, const wchar_t* filename);

	//取得
	ID3D12Resource* GetTexBuff(int texNumber);

	ID3D12Device* GetDevice() { return device_; }

	const DirectX::XMMATRIX& GetMatProjection() { return matProjection_; }

	ID3D12GraphicsCommandList* GetcmdList() { return cmdList_; }

	void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texnumber);

	void SetGraphicsRootDescriptorTable_Post(UINT rootParameterIndex, UINT texnumber);

	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState_Post;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature_Post;



	void CreateGraphicsPipelineState_Post();


	//パイプラインセット
	PipeLineSet pipelineSet_;


private:
	void CreateSprite2dpipe();

	void CreateSprite2dpipe_Post();


protected:

private:

	//テクスチャリソース(テクスチャバッファ)の配列
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_[spriteSRVCount];

	//射影行列
	DirectX::XMMATRIX matProjection_{};

	//テクスチャ用デスクリプタヒープの生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap_;

	ID3D12Device* device_ = nullptr;
	//コマンド仮
	ID3D12GraphicsCommandList* cmdList_ = nullptr;
};

